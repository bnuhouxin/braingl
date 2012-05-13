#ifndef NAVGLWIDGET_H
#define NAVGLWIDGET_H

#include <QtOpenGL/QGLWidget>

class NavRenderer;
class DataStore;

class NavGLWidget: public QGLWidget
{
    Q_OBJECT

public:
    NavGLWidget( DataStore* dataStore, QString name, QWidget *parent = 0, const QGLWidget *shareWidget = 0 );
    ~NavGLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

public slots:
    void update();

signals:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL( int width, int height );
    void mousePressEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );

private:
    NavRenderer* m_navRenderer;
};

#endif
