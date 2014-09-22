/*
 * LoopSubdivsionivision.cpp
 *
 *  Created on: Sep 3, 2013
 *      Author: schurade
 */

#include "loopsubdivision.h"

#include "../data/mesh/trianglemesh2.h"

#include <QDebug>

#include <cmath>

LoopSubdivision::~LoopSubdivision()
{
}

LoopSubdivision::LoopSubdivision( TriangleMesh2* mesh ) :
    m_mesh( mesh )
{
    m_mesh = new TriangleMesh2( mesh );

    m_numVerts = m_mesh->numVerts();
    m_numTris = m_mesh->numTris();

    m_mesh->resize( m_numVerts * 4, m_numTris * 4 );

    std::vector<QVector3D> newVertexPositions( m_numVerts );

    for ( unsigned int i = 0; i < m_numVerts; ++i )
    {
        newVertexPositions[i] = calcNewPosition( i );
    }

    for ( unsigned int i = 0; i < m_numTris; ++i )
    {
        insertCenterTriangle( i );
    }

    for ( unsigned int i = 0; i < m_numTris; ++i )
    {
        insertCornerTriangles( i );
    }

    for ( unsigned int i = 0; i < m_numVerts; ++i )
    {
        m_mesh->setVertex( i, newVertexPositions[i] );
    }

    m_numVerts = m_mesh->numVerts();
    m_numTris = m_mesh->numTris();

    m_mesh->finalize();

    newVertexPositions.clear();
}

QVector3D LoopSubdivision::calcNewPosition( unsigned int vertNum )
{
    std::vector<unsigned int> starP = m_mesh->getStar( vertNum );
    unsigned int starSize = starP.size();

    QVector3D oldPos = m_mesh->getVertex( vertNum );
    double alpha = getAlpha( starSize );
    oldPos *= ( 1.0 - ( (double) starSize * alpha ) );

    QVector3D newPos;
    unsigned int edgeV = 0;
    for ( unsigned int i = 0; i < starSize; i++ )
    {
        edgeV = m_mesh->getNextVertex( starP[i], vertNum );
        newPos += m_mesh->getVertex( edgeV );
    }
    newPos *= alpha;

    return oldPos + newPos;
}

void LoopSubdivision::insertCenterTriangle( unsigned int triNum )
{

    std::vector<unsigned int> intP = m_mesh->getTriangle( triNum );
    unsigned int edgeVerts[3];

    for ( unsigned int i = 0; i < 3; ++i )
    {
        edgeVerts[i] = calcEdgeVert( triNum, intP[i], intP[( i + 1 ) % 3], intP[( i + 2 ) % 3] );
    }
    m_mesh->addTriangle( edgeVerts[0], edgeVerts[1], edgeVerts[2] );
}

void LoopSubdivision::insertCornerTriangles( unsigned int triNum )
{

    // comment:     center are twisted from the orignal vertices.
    // original:    0, 1, 2
    // center:      a, b, c
    // reAsgnOrig:  0, a, c
    // addTris:     1, b, a
    // addTris:     2, c, b
    //
    std::vector<unsigned int> originalTri = m_mesh->getTriangle( triNum );
    std::vector<unsigned int> centerTri = m_mesh->getTriangle( triNum + m_numTris );

    m_mesh->addTriangle( originalTri[1], centerTri[1], centerTri[0] );
    m_mesh->addTriangle( originalTri[2], centerTri[2], centerTri[1] );
    m_mesh->setTriangle( triNum, originalTri[0], centerTri[0], centerTri[2] );
}

unsigned int LoopSubdivision::calcEdgeVert( unsigned int triNum, unsigned int edgeV1, unsigned int edgeV2, unsigned int V3 )
{

    unsigned int vertNum = 0;

    unsigned  neighborVert = 0;
    unsigned  neighborFaceNum = 0;
    QVector3D edgeVert;

    neighborFaceNum = m_mesh->getNeighbor( edgeV1, edgeV2, triNum );

    if ( neighborFaceNum == triNum )
    {
        QVector3D edgeVert = ( m_mesh->getVertex( edgeV1 ) + m_mesh->getVertex( edgeV2 ) ) / 2.0;
        vertNum = m_mesh->currentNumVerts();
        m_mesh->addVertex( edgeVert.x(), edgeVert.y(), edgeVert.z() );
    }

    else if ( neighborFaceNum > triNum )
    {
        neighborVert = m_mesh->getThirdVert( edgeV1, edgeV2, neighborFaceNum );

        QVector3D edgePart = m_mesh->getVertex( edgeV1 ) + m_mesh->getVertex( edgeV2 );
        QVector3D neighborPart = m_mesh->getVertex( neighborVert ) + m_mesh->getVertex( V3 );

        edgeVert = ( ( edgePart * ( 3.0 / 8.0 ) ) + ( neighborPart * ( 1.0 / 8.0 ) ) );
        vertNum = m_mesh->currentNumVerts();
        m_mesh->addVertex( edgeVert.x(), edgeVert.y(), edgeVert.z() );
    }
    else
    {
        std::vector<unsigned int> neighborCenterP = m_mesh->getTriangle( neighborFaceNum + m_numTris );
        std::vector<unsigned int> neighborP = m_mesh->getTriangle( neighborFaceNum );

        if ( neighborP[0] == edgeV2 )
        {
            vertNum = neighborCenterP[0];
        }
        else if ( neighborP[1] == edgeV2 )
        {
            vertNum = neighborCenterP[1];
        }
        else
        {
            vertNum = neighborCenterP[2];
        }
    }
    return vertNum;
}

double LoopSubdivision::getAlpha( unsigned int n )
{
    double answer;
    if ( n > 3 )
    {
        double center = ( 0.375 + ( 0.25 * std::cos( ( 2.0 * 3.14159265358979 ) / (double) n ) ) );
        answer = ( 0.625 - ( center * center ) ) / (double) n;
    }
    else
    {
        answer = 3.0 / 16.0;
    }
    return answer;
}
