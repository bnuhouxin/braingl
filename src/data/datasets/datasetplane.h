/*
 * datasetplane.h
 *
 *  Created on: Apr 10, 2014
 *      Author: schurade
 */

#ifndef DATASETPLANE_H_
#define DATASETPLANE_H_

#include "dataset.h"

class DatasetPlane : public Dataset
{
    Q_OBJECT

public:
    DatasetPlane();
    virtual ~DatasetPlane();

    virtual void draw( QMatrix4x4 pMatrix, QMatrix4x4 mvMatrix, int width, int height, int renderMode, QString target );

    //bool mousePick( int pickId, QVector3D pos, Qt::KeyboardModifiers modifiers, QString target );
    bool rightMouseDrag( int pickId, QVector3D dir, Qt::KeyboardModifiers modifiers, QString target );

private:
    GLuint vbo0;

    int m_handle0;
    int m_handle1;
    int m_handle2;

    bool dirty;

    void initGeometry();


};

#endif /* DATASETPLANE_H_ */
