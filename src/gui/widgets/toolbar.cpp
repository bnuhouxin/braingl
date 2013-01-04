/*
 * toolbar.cpp
 *
 *  Created on: 14.06.2012
 *      Author: Ralph
 */
#include "../views/toolbarview.h"
#include "../../data/datasets/dataset.h"



#include "toolbar.h"

ToolBar::ToolBar( const QString &title, QWidget* parent ) :
    QToolBar( title, parent )
{
    setObjectName( title );
    m_toolBarView = new ToolBarView();

    connect( m_toolBarView, SIGNAL( sigSelectionChanged( int ) ), this, SLOT( slotSelectionChanged( int ) ) );

    createActions();
}

ToolBar::~ToolBar()
{
}

void ToolBar::setModel( QAbstractItemModel* model )
{
    m_toolBarView->setModel( model );

}

void ToolBar::setSelectionModel( QItemSelectionModel* selectionModel )
{
    m_toolBarView->setSelectionModel( selectionModel );
}

void ToolBar::createActions()
{
    m_isosurfaceAct = new FNAction( QIcon( ":/icons/tmp1.png" ), tr( "Scalar 1" ), this, FNALGO_ISOSURFACE );
    m_isosurfaceAct->setStatusTip( tr( "Scalar Action 1" ) );
    connect( m_isosurfaceAct, SIGNAL( sigTriggered( FN_ALGO ) ), this, SLOT( slot( FN_ALGO ) ) );

    m_qball4Act = new FNAction( QIcon( ":/icons/qball4.png" ), tr( "QBall 4th order" ), this, FNALGO_QBALL4 );
    m_qball4Act->setStatusTip( tr( "QBall 4th order" ) );
    connect( m_qball4Act, SIGNAL( sigTriggered( FN_ALGO ) ), this, SLOT( slot( FN_ALGO ) ) );

    m_qball6Act = new FNAction( QIcon( ":/icons/qball6.png" ), tr( "QBall 6th order" ), this, FNALGO_QBALL6 );
    m_qball6Act->setStatusTip( tr( "QBall 6th order" ) );
    connect( m_qball6Act, SIGNAL( sigTriggered( FN_ALGO ) ), this, SLOT( slot( FN_ALGO ) ) );

    m_qball8Act = new FNAction( QIcon( ":/icons/qball8.png" ), tr( "QBall 8th order" ), this, FNALGO_QBALL8 );
    m_qball8Act->setStatusTip( tr( "QBall 8th order" ) );
    connect( m_qball8Act, SIGNAL( sigTriggered( FN_ALGO ) ), this, SLOT( slot( FN_ALGO ) ) );

    m_tensorAct = new FNAction( QIcon( ":/icons/fittensor.png" ), tr( "Tensor fit" ), this, FNALGO_TENSORFIT );
    m_tensorAct->setStatusTip( tr( "tensor fit" ) );
    connect( m_tensorAct, SIGNAL( sigTriggered( FN_ALGO ) ), this, SLOT( slot( FN_ALGO ) ) );

    m_faAct = new FNAction( QIcon( ":/icons/calcfa.png" ), tr( "Calc FA" ), this, FNALGO_FA );
    m_faAct->setStatusTip( tr( "calc FA" ) );
    connect( m_faAct, SIGNAL( sigTriggered( FN_ALGO ) ), this, SLOT( slot( FN_ALGO ) ) );

    m_evAct = new FNAction( QIcon( ":/icons/eigenvector.png" ), tr( "Calc Eigen Vector" ), this, FNALGO_EV );
    m_evAct->setStatusTip( tr( "calc Eigen Vector" ) );
    connect( m_evAct, SIGNAL( sigTriggered( FN_ALGO ) ), this, SLOT( slot( FN_ALGO ) ) );

    m_binghamAction = new FNAction( QIcon( ":/icons/tmpx.png" ), tr( "Fit Bingham" ), this, FNALGO_BINGHAM );
    m_binghamAction->setStatusTip( tr( "fit bingham distribution" ) );
    connect( m_binghamAction, SIGNAL( sigTriggered( FN_ALGO ) ), this, SLOT( slot( FN_ALGO ) ) );

    m_fiberAction1 = new FNAction( QIcon( ":/icons/tmpf.png" ), tr( "Fiber 1" ), this, FNALGO_TENSOR_TRACK );
    m_fiberAction1->setStatusTip( tr( "Fiber Action 1" ) );
    connect( m_fiberAction1, SIGNAL( sigTriggered( FN_ALGO ) ), this, SLOT( slot( FN_ALGO ) ) );
}

void ToolBar::slot( FN_ALGO algo )
{
    m_toolBarView->activateAlgo( algo );
}

void ToolBar::slotSelectionChanged( int type )
{
    this->clear();
    switch ( type )
    {
        case FNDT_NIFTI_SCALAR:
        {
            this->addAction( m_isosurfaceAct );
            break;
        }
        case FNDT_NIFTI_VECTOR:
        {
            //this->addAction( m_vectorAction1 );
            break;
        }
        case FNDT_NIFTI_TENSOR:
        {
            this->addAction( m_faAct );
            this->addAction( m_evAct );
            this->addAction( m_fiberAction1 );
            break;
        }
        case FNDT_NIFTI_SH:
        {
            this->addAction( m_binghamAction );
            break;
        }
        case FNDT_NIFTI_DWI:
        {
            this->addAction( m_qball4Act );
            this->addAction( m_qball6Act );
            this->addAction( m_qball8Act );
            this->addAction( m_tensorAct );
            this->addAction( m_faAct );
            this->addAction( m_evAct );
            break;
        }
        default:
        {
            break;
        }
    }
}
