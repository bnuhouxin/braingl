/*
 * scenerenderer.cpp
 *
 *  Created on: 09.05.2012
 *      Author: Ralph
 */
#include "../../thirdparty/glew/include/glew.h"

#include "scenerenderer.h"
#include "arcball.h"
#include "slicerenderer.h"

#include "../../data/datastore.h"
#include "../../data/enums.h"
#include "../../data/datasets/dataset.h"
#include "../../data/vptr.h"

#include <QtCore/QDebug>

#include <math.h>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

SceneRenderer::SceneRenderer( QAbstractItemModel* dataModel, QAbstractItemModel* globalModel, QAbstractItemModel* selectionBoxModel ) :
    m_dataModel( dataModel ),
    m_globalModel( globalModel ),
    m_selectionBoxModelModel( selectionBoxModel ),
    m_boundingbox( 200 ),
    m_nx( 160 ),
    m_ny( 200 ),
    m_nz( 160 ),
    m_width( 1 ),
    m_height( 1 ),
    m_ratio( 1.0 )
{
    m_sliceRenderer = new SliceRenderer( dataModel );
    m_sliceRenderer->setModel( globalModel );

    m_arcBall = new ArcBall( 400, 400 );

    m_mvMatrix.setToIdentity();
}

SceneRenderer::~SceneRenderer()
{
}

void SceneRenderer::initGL()
{
    qDebug() << "gl init main widget";
    GLenum errorCode = glewInit();
    if ( GLEW_OK != errorCode )
    {
        qDebug() << "Problem: glewInit failed, something is seriously wrong.";
        qDebug() << glewGetErrorString( errorCode );
        exit( false );
    }
    else
    {
        qDebug() << "OpenGL initialized.";
    }

    glClearColor( 1.0, 1.0, 1.0, 1.0 );

    glEnable( GL_DEPTH_TEST );

    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_BLEND );

    glShadeModel( GL_SMOOTH );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    glEnable( GL_MULTISAMPLE );

//    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
//    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
//    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
//
//    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
//    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    static GLfloat lightPosition[ 4 ] =
    { 0.5, 5.0, -3000.0, 1.0 };
    glLightfv( GL_LIGHT0, GL_POSITION, lightPosition );

    calcMVPMatrix();
    m_sliceRenderer->init();
}

void SceneRenderer::resizeGL( int width, int height )
{
    m_width = width;
    m_height = height;
    m_arcBall->set_win_size( width, height );

    m_ratio = static_cast<float>( width )/ static_cast<float>(height);
    glViewport( 0, 0, width, height );

    calcMVPMatrix();
}

void SceneRenderer::calcMVPMatrix()
{
    m_nx = m_globalModel->data( m_globalModel->index( FNGLOBAL_MAX_SAGITTAL, 0 ) ).toFloat();
    m_ny = m_globalModel->data( m_globalModel->index( FNGLOBAL_MAX_CORONAL, 0 ) ).toFloat();
    m_nz = m_globalModel->data( m_globalModel->index( FNGLOBAL_MAX_AXIAL, 0 ) ).toFloat();
    float dx = m_globalModel->data( m_globalModel->index( FNGLOBAL_SLICE_DX, 0 ) ).toFloat();
    float dy = m_globalModel->data( m_globalModel->index( FNGLOBAL_SLICE_DY, 0 ) ).toFloat();
    float dz = m_globalModel->data( m_globalModel->index( FNGLOBAL_SLICE_DZ, 0 ) ).toFloat();
    m_nx *= dx;
    m_ny *= dy;
    m_nz *= dz;

    m_arcBall->setRotCenter( m_nx / 2., m_ny / 2., m_nz / 2. );

    m_boundingbox = qMax ( m_nx, qMax( m_ny, m_nz ) );

    // Reset projection
    QMatrix4x4 pMatrix;
    pMatrix.setToIdentity();

    float halfBB = m_boundingbox / 2.0;

    float bbx = m_boundingbox;
    float bby = m_boundingbox;

    if ( m_ratio >= 1.0 )
    {
        pMatrix.ortho( -halfBB * m_ratio, halfBB * m_ratio, -halfBB, halfBB, -3000, 3000 );
        bbx = m_boundingbox * m_ratio;
    }
    else
    {
        pMatrix.ortho( -halfBB, halfBB, -halfBB / m_ratio, halfBB / m_ratio, -3000, 3000 );
        bby = m_boundingbox / m_ratio;
    }

    m_mvMatrix = m_arcBall->getMVMat();
    m_mvMatrixInverse = m_mvMatrix.inverted();
    m_mvpMatrix = pMatrix * m_mvMatrix;

    m_globalModel->setData( m_globalModel->index( FNGLOBAL_ZOOM, 0 ), m_arcBall->getZoom() );
    m_globalModel->setData( m_globalModel->index( FNGLOBAL_MOVEX, 0 ), m_arcBall->getMoveX() );
    m_globalModel->setData( m_globalModel->index( FNGLOBAL_MOVEY, 0 ), m_arcBall->getMoveY() );
    m_globalModel->setData( m_globalModel->index( FNGLOBAL_BBX, 0 ), bbx );
    m_globalModel->setData( m_globalModel->index( FNGLOBAL_BBY, 0 ), bby );

    //m_shRenderer->setSceneStats( m_arcBall->getZoom(), m_arcBall->getMoveX(), m_arcBall->getMoveY(), bbx, bby );
}

void SceneRenderer::draw()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    m_sliceRenderer->draw( m_mvpMatrix );

    int countDatasets = m_dataModel->rowCount();
    for ( int i = 0; i < countDatasets; ++i )
    {
        QModelIndex index = m_dataModel->index( i, FNPROP_ACTIVE );
        if ( m_dataModel->data( index, Qt::DisplayRole ).toBool() )
        {
            Dataset* ds = VPtr<Dataset>::asPtr( m_dataModel->data( m_dataModel->index( i, FNPROP_DATASET_POINTER ), Qt::DisplayRole ) );
            ds->draw( m_mvpMatrix, m_mvMatrixInverse, m_globalModel );
        }
    }
}

void SceneRenderer::setView( int view )
{
    m_arcBall->setView( view );
    int countDatasets = m_dataModel->rowCount();
    for ( int i = 0; i < countDatasets; ++i )
    {
        QModelIndex index = m_dataModel->index( i, FNPROP_ACTIVE );
        if ( m_dataModel->data( index, Qt::DisplayRole ).toBool() )
        {
            Dataset* ds = VPtr<Dataset>::asPtr( m_dataModel->data( m_dataModel->index( i, FNPROP_DATASET_POINTER ), Qt::DisplayRole ) );
            ds->properties()->set( FNPROP_RENDER_SLICE, view );
        }
    }
}

void SceneRenderer::leftMouseDown( int x, int y )
{
    m_arcBall->click( x, y );
}

void SceneRenderer::leftMouseDrag( int x, int y )
{
    m_arcBall->drag( x, y );
    calcMVPMatrix();
}

void SceneRenderer::middleMouseDown( int x, int y )
{
    m_arcBall->midClick( x, y );
    calcMVPMatrix();
}

void SceneRenderer::middleMouseDrag( int x, int y )
{
    m_arcBall->midDrag( x, y );
    calcMVPMatrix();
}

void SceneRenderer::mouseWheel( int step )
{
    m_arcBall->mouseWheel( step );
    calcMVPMatrix();
}
