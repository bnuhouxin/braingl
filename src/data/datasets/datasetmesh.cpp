/*
 * datasetmesh.cpp
 *
 * Created on: Jul 19, 2012
 * @author Ralph Schurade
 */
#include "datasetmesh.h"

#include "../models.h"

#include "../mesh/trianglemesh2.h"
#include "../../gui/gl/meshrenderer.h"

DatasetMesh::DatasetMesh( TriangleMesh2* mesh, QDir fileName ) :
    Dataset( fileName, Fn::DatasetType::MESH_BINARY ),
    m_renderer( 0 )
{
    m_mesh.push_back( mesh );
    m_properties["maingl"]->set( Fn::Property::COLORMODE, { "per mesh", "mri", "per vertex", "vertex data" }, 0, "general" );
    m_properties["maingl"]->set( Fn::Property::COLORMAP, 1, "general" );
    m_properties["maingl"]->set( Fn::Property::COLOR, QColor( 255, 255, 255 ), "general" );
    m_properties["maingl"]->set( Fn::Property::ALPHA, 1.f, 0.f, 1.f, "general" );

    m_properties["maingl"]->set( Fn::Property::RENDER_COLORMAP, false, "colormap" );
    m_properties["maingl"]->set( Fn::Property::COLORMAP_X, 50, 1, 2000, "colormap" );
    m_properties["maingl"]->set( Fn::Property::COLORMAP_Y, 50, 1, 2000, "colormap" );
    m_properties["maingl"]->set( Fn::Property::COLORMAP_DX, 400, 1, 2000, "colormap" );
    m_properties["maingl"]->set( Fn::Property::COLORMAP_DY, 20, 1, 100, "colormap" );
    m_properties["maingl"]->set( Fn::Property::COLORMAP_TEXT_SIZE, 30, 1, 100, "colormap" );

    m_properties["maingl"]->set( Fn::Property::PAINTMODE, { "off", "paint", "erase" }, 0, "paint" );
    m_properties["maingl"]->set( Fn::Property::PAINTSIZE, 2.f, 1.f, 100.f, "paint" );
    m_properties["maingl"]->set( Fn::Property::PAINTCOLOR, QColor( 255, 0, 0 ), "paint" );

    float min = 0; //std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();
/*
    for ( int i = 0; i < mesh->numVerts(); ++i )
    {
        float value = mesh->getVertexData( i );
        min = qMin( min, value );
        max = qMax( max, value );
    }
*/
    m_properties["maingl"]->set( Fn::Property::MIN, min );
    m_properties["maingl"]->set( Fn::Property::MAX, max );
    m_properties["maingl"]->set( Fn::Property::SELECTED_MIN, min, min, max );
    m_properties["maingl"]->set( Fn::Property::SELECTED_MAX, max, min, max );
    m_properties["maingl"]->set( Fn::Property::LOWER_THRESHOLD, min + (max-min)/1000., min, max );
    m_properties["maingl"]->set( Fn::Property::UPPER_THRESHOLD, max, min, max );

    connect( m_properties["maingl"]->getProperty( Fn::Property::PAINTMODE ), SIGNAL( valueChanged( int ) ), this, SLOT( paintModeChanged( int ) ) );

    m_properties["maingl2"]->set( Fn::Property::COLORMODE, { "per mesh", "mri", "per vertex", "vertex data" }, 0, "general" );
    m_properties["maingl2"]->set( Fn::Property::COLORMAP, 1, "general" );
    m_properties["maingl2"]->set( Fn::Property::COLOR, QColor( 255, 255, 255 ), "general" );
    m_properties["maingl2"]->set( Fn::Property::ALPHA, 1.f, 0.f, 1.f, "general" );

    m_properties["maingl2"]->set( Fn::Property::RENDER_COLORMAP, false, "colormap" );
    m_properties["maingl2"]->set( Fn::Property::COLORMAP_X, 50, 1, 2000, "colormap" );
    m_properties["maingl2"]->set( Fn::Property::COLORMAP_Y, 50, 1, 2000, "colormap" );
    m_properties["maingl2"]->set( Fn::Property::COLORMAP_DX, 400, 1, 2000, "colormap" );
    m_properties["maingl2"]->set( Fn::Property::COLORMAP_DY, 20, 1, 100, "colormap" );
    m_properties["maingl2"]->set( Fn::Property::COLORMAP_TEXT_SIZE, 30, 1, 100, "colormap" );

    m_properties["maingl2"]->set( Fn::Property::MIN, min );
    m_properties["maingl2"]->set( Fn::Property::MAX, max );
    m_properties["maingl2"]->set( Fn::Property::SELECTED_MIN, min, min, max );
    m_properties["maingl2"]->set( Fn::Property::SELECTED_MAX, max, min, max );
    m_properties["maingl2"]->set( Fn::Property::LOWER_THRESHOLD, min + (max-min)/1000., min, max );
    m_properties["maingl2"]->set( Fn::Property::UPPER_THRESHOLD, max, min, max );
}

DatasetMesh::DatasetMesh( QDir fileName, Fn::DatasetType type ) :
    Dataset( fileName, type ),
    m_renderer( 0 )
{
    PropertyGroup* props = new PropertyGroup();
    props->set( Fn::Property::ACTIVE, true, "general" );
    props->set( Fn::Property::RENDER_TARGET, "maingl2" );
    m_properties.insert( "maingl2", props );

    m_properties["maingl"]->set( Fn::Property::COLORMODE, { "per mesh", "mri", "per vertex", "vertex data" }, 0, "general" );
    m_properties["maingl"]->set( Fn::Property::COLORMAP, 1, "general" );
    m_properties["maingl"]->set( Fn::Property::COLOR, QColor( 255, 255, 255 ), "general" );
    m_properties["maingl"]->set( Fn::Property::ALPHA, 1.f, 0.f, 1.f, "general" );

    m_properties["maingl"]->set( Fn::Property::RENDER_COLORMAP, false, "colormap" );
    m_properties["maingl"]->set( Fn::Property::COLORMAP_X, 50, 1, 2000, "colormap" );
    m_properties["maingl"]->set( Fn::Property::COLORMAP_Y, 50, 1, 2000, "colormap" );
    m_properties["maingl"]->set( Fn::Property::COLORMAP_DX, 400, 1, 2000, "colormap" );
    m_properties["maingl"]->set( Fn::Property::COLORMAP_DY, 20, 1, 100, "colormap" );
    m_properties["maingl"]->set( Fn::Property::COLORMAP_TEXT_SIZE, 30, 1, 100, "colormap" );

    m_properties["maingl"]->set( Fn::Property::PAINTMODE, { "off", "paint", "erase" }, 0, "paint" );
    m_properties["maingl"]->set( Fn::Property::PAINTSIZE, 2.f, 1.f, 100.f, "paint" );
    m_properties["maingl"]->set( Fn::Property::PAINTCOLOR, QColor( 255, 0, 0 ), "paint" );

    float min = 0; //std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();
/*
    for ( int i = 0; i < mesh->numVerts(); ++i )
    {
        float value = mesh->getVertexData( i );
        min = qMin( min, value );
        max = qMax( max, value );
    }
*/
    m_properties["maingl"]->set( Fn::Property::MIN, min );
    m_properties["maingl"]->set( Fn::Property::MAX, max );
    m_properties["maingl"]->set( Fn::Property::SELECTED_MIN, min, min, max );
    m_properties["maingl"]->set( Fn::Property::SELECTED_MAX, max, min, max );
    m_properties["maingl"]->set( Fn::Property::LOWER_THRESHOLD, min + (max-min)/1000., min, max );
    m_properties["maingl"]->set( Fn::Property::UPPER_THRESHOLD, max, min, max );

    connect( m_properties["maingl"]->getProperty( Fn::Property::PAINTMODE ), SIGNAL( valueChanged( int ) ), this, SLOT( paintModeChanged( int ) ) );

    m_properties["maingl2"]->set( Fn::Property::COLORMODE, { "per mesh", "mri", "per vertex", "vertex data" }, 0, "general" );
    m_properties["maingl2"]->set( Fn::Property::COLORMAP, 1, "general" );
    m_properties["maingl2"]->set( Fn::Property::COLOR, QColor( 255, 255, 255 ), "general" );
    m_properties["maingl2"]->set( Fn::Property::ALPHA, 1.f, 0.f, 1.f, "general" );

    m_properties["maingl2"]->set( Fn::Property::RENDER_COLORMAP, false, "colormap" );
    m_properties["maingl2"]->set( Fn::Property::COLORMAP_X, 50, 1, 2000, "colormap" );
    m_properties["maingl2"]->set( Fn::Property::COLORMAP_Y, 50, 1, 2000, "colormap" );
    m_properties["maingl2"]->set( Fn::Property::COLORMAP_DX, 400, 1, 2000, "colormap" );
    m_properties["maingl2"]->set( Fn::Property::COLORMAP_DY, 20, 1, 100, "colormap" );
    m_properties["maingl2"]->set( Fn::Property::COLORMAP_TEXT_SIZE, 30, 1, 100, "colormap" );

    m_properties["maingl2"]->set( Fn::Property::MIN, min );
    m_properties["maingl2"]->set( Fn::Property::MAX, max );
    m_properties["maingl2"]->set( Fn::Property::SELECTED_MIN, min, min, max );
    m_properties["maingl2"]->set( Fn::Property::SELECTED_MAX, max, min, max );
    m_properties["maingl2"]->set( Fn::Property::LOWER_THRESHOLD, min + (max-min)/1000., min, max );
    m_properties["maingl2"]->set( Fn::Property::UPPER_THRESHOLD, max, min, max );
}
DatasetMesh::~DatasetMesh()
{
}

TriangleMesh2* DatasetMesh::getMesh()
{
    return m_mesh[0];
}

void DatasetMesh::draw( QMatrix4x4 pMatrix, QMatrix4x4 mvMatrix, int width, int height, int renderMode, QString target )
{
    if ( !properties( target )->get( Fn::Property::ACTIVE ).toBool() )
    {
        return;
    }
    if ( m_renderer == 0 )
    {
        m_renderer = new MeshRenderer( m_mesh[0] );
        m_renderer->setModel( Models::g() );
        m_renderer->init();
    }


    m_renderer->draw( pMatrix, mvMatrix, width, height, renderMode, properties( target ) );
}

QString DatasetMesh::getValueAsString( int x, int y, int z )
{
    return QString( "" );
}

void DatasetMesh::mousePick( int pickId, QVector3D pos )
{
    if ( pickId == 0 )
   {
       return;
   }

   int paintMode = m_properties["maingl"]->get( Fn::Property::PAINTMODE ).toInt();
   if (  paintMode != 0 )
   {
       QColor color;
       if ( paintMode == 1 )
       {
           color = m_properties["maingl"]->get( Fn::Property::PAINTCOLOR ).value<QColor>();
       }
       else if ( paintMode == 2 )
       {
           color = m_properties["maingl"]->get( Fn::Property::COLOR ).value<QColor>();
       }

       QVector<int>picked = getMesh()->pick( pos, m_properties["maingl"]->get( Fn::Property::PAINTSIZE ).toFloat() );

       if ( picked.size() > 0 )
       {
           m_renderer->beginUpdateColor();
           for ( int i = 0; i < picked.size(); ++i )
           {
               m_renderer->updateColor( picked[i], color.redF(), color.greenF(), color.blueF(), 1.0 );
               m_mesh[0]->setVertexColor( picked[i], color );
           }
           m_renderer->endUpdateColor();
           Models::d()->submit();
       }
   }
}

void DatasetMesh::paintModeChanged( int mode )
{
    if ( mode > 0 )
    {
        m_properties["maingl"]->set( Fn::Property::COLORMODE, 2 );
    }
}
