/*
 * kdtree.cpp
 *
 *  Created on: 05.02.2013
 *      Author: Ralph Schurade
 */

#include "kdtree.h"

KdTree::KdTree( int size, float *pointArray, bool useThreads )
:   m_size( size ),
    m_pointArray( pointArray )
{
    m_tree.clear();
    m_tree.resize( size );
    for (int i = 0 ; i < size ;  ++i)
        m_tree[i] = i;

    if( useThreads )
    {

        int root = ( size - 1 )/2;
        std::nth_element( m_tree.begin(), m_tree.begin()+root, m_tree.end(), lessy( m_pointArray, 0 ) );

        int rootLeft = (root - 1) / 2;
        std::nth_element( m_tree.begin(), m_tree.begin()+rootLeft, m_tree.begin()+root-1, lessy( m_pointArray, 1 ) );

        int rootRight = (size + root) / 2;
        std::nth_element( m_tree.begin()+root+1, m_tree.begin()+rootRight, m_tree.end(), lessy( m_pointArray, 1 ) );

        KdTreeThread *thread1 = new KdTreeThread( m_pointArray, &m_tree, 0, rootLeft-1, 2 );
        thread1->start();

        KdTreeThread *thread2 = new KdTreeThread( m_pointArray, &m_tree, rootLeft+1, root-1, 2 );
        thread2->start();

        KdTreeThread *thread3 = new KdTreeThread( m_pointArray, &m_tree, root+1, rootRight-1, 2 );
        thread3->start();

        KdTreeThread *thread4 = new KdTreeThread( m_pointArray, &m_tree, rootRight+1, size-1, 2 );
        thread4->start();

        thread1->wait();
        thread2->wait();
        thread3->wait();
        thread4->wait();
    }
    else
    {
        buildTree( 0, size - 1, 0 );
    }
}

void KdTree::buildTree(int left, int right, int axis)
{
    if (left >= right) return;

    int div = ( left+right )/2;
    std::nth_element( m_tree.begin()+left, m_tree.begin()+div, m_tree.begin()+right, lessy( m_pointArray, axis ) );

    buildTree(left, div-1, (axis+1)%3);
    buildTree(div+1, right, (axis+1)%3);
}



KdTreeThread::KdTreeThread(float *pointArray, std::vector<int>* tree, int left, int right, int axis )
:   m_tree(tree),
    m_pointArray(pointArray),
    m_left(left),
    m_right(right),
    m_axis(axis)
{
}

void KdTreeThread::run()
{
    buildTree(m_left, m_right, m_axis);

}

void KdTreeThread::buildTree(int left, int right, int axis)
{
    if (left >= right) return;

    int div = ( left+right )/2;
    std::nth_element( m_tree->begin()+left, m_tree->begin()+div, m_tree->begin()+right, lessy( m_pointArray, axis ) );

    buildTree(left, div-1, (axis+1)%3);
    buildTree(div+1, right, (axis+1)%3);
}
