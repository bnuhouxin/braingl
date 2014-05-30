/*
 * combinednavglwidget.cpp
 *
 * Created on: May 03, 2012
 * @author Ralph Schurade
 */
#include "combinednavglwidget.h"

#include "../../data/models.h"

#include "../gl/combinednavrenderer.h"

#include <QtGui>
#include "../core_3_3_context.h"

CombinedNavGLWidget::CombinedNavGLWidget( QString name, QWidget *parent, const QGLWidget *shareWidget ) :
    QGLWidget( new core_3_3_context(QGLFormat::defaultFormat()), parent, shareWidget ),
	m_visible( true )
{
    m_renderer = new CombinedNavRenderer( name );
    connect( Models::g(), SIGNAL( dataChanged( QModelIndex, QModelIndex ) ), this, SLOT( update() ) );
    connect( Models::d(), SIGNAL( dataChanged( QModelIndex, QModelIndex ) ), this, SLOT( update() ) );
}

CombinedNavGLWidget::~CombinedNavGLWidget()
{
}

QSize CombinedNavGLWidget::minimumSizeHint() const
{
    return QSize( 50, 50 );
}

QSize CombinedNavGLWidget::sizeHint() const
{
    return QSize( 400, 400 );
}

void CombinedNavGLWidget::initializeGL()
{
    // needed per OpenGL context and so per QGLWidget
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    m_renderer->initGL();
}

void CombinedNavGLWidget::paintGL()
{
    if ( m_visible )
    {
        m_renderer->draw();
    }
}

void CombinedNavGLWidget::resizeGL( int width, int height )
{
	m_renderer->resizeGL( width, height );
}

void CombinedNavGLWidget::mousePressEvent( QMouseEvent *event )
{
    if ( event->buttons() & Qt::LeftButton )
    {
        m_renderer->leftMouseDown( event->x(), event->y() );
    }
    Models::g()->submit();
    updateGL();
}

void CombinedNavGLWidget::mouseMoveEvent( QMouseEvent *event )
{
    if ( event->buttons() & Qt::LeftButton )
    {
        m_renderer->leftMouseDrag( event->x(), event->y() );
    }
    Models::g()->submit();
    updateGL();
}

void CombinedNavGLWidget::mouseReleaseEvent( QMouseEvent *event )
{
    Models::g()->submit();
    updateGL();
}

void CombinedNavGLWidget::update()
{
    if ( !m_visible )
    {
        return;
    }
    updateGL();
}

void CombinedNavGLWidget::setWidgetVisible( bool visible )
{
    m_visible = visible;
}
