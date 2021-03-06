/*
 * toolbar.cpp
 *
 * Created on: 14.06.2012
 * @author Ralph Schurade
 */
#include "toolbar.h"

#include "../views/toolbarview.h"
#include "../widgets/algoStarterWidgets/tensortrackwidget.h"
#include "../widgets/algoStarterWidgets/crossingtrackwidget.h"
#include "../widgets/algoStarterWidgets/sdwidget.h"
#include "../widgets/algoStarterWidgets/correlationwidget.h"
#include "../widgets/algoStarterWidgets/datasetselectionwidget.h"
#include "../widgets/algoStarterWidgets/bundlingwidget.h"
#include "../widgets/algoStarterWidgets/fiberbundlewidget.h"
#include "../widgets/algoStarterWidgets/mathwidget.h"

#include "../../algos/dwialgos.h"
#include "../../algos/scalaralgos.h"
#include "../../algos/fiberalgos.h"
#include "../../algos/meshalgos.h"
#include "../../data/vptr.h"
#include "../../data/models.h"
#include "../../data/datasets/dataset.h"
#include "../../data/datasets/datasetscalar.h"
#include "../../data/datasets/datasetcons.h"
#include "../../data/datasets/datasetglyphset.h"
#include "../../data/datasets/datasetconglyphs.h"

#include <QVariant>

ToolBar::ToolBar( const QString &title, QWidget* parent ) :
    QToolBar( title, parent )
{
    setObjectName( title );
    m_toolBarView = new ToolBarView( this );
    m_toolBarView->setModel( Models::d() );

    connect( m_toolBarView, SIGNAL( sigSelectionChanged( int ) ), this, SLOT( slotSelectionChanged( int ) ) );

    createActions();
}

ToolBar::~ToolBar()
{
}

void ToolBar::setSelectionModel( QItemSelectionModel* selectionModel )
{
    m_toolBarView->setSelectionModel( selectionModel );
}

void ToolBar::createActions()
{
    m_isosurfaceAct = new FNAction( QIcon( ":/icons/iso_surface.png" ), tr( "iso surface" ), this, Fn::Algo::ISOSURFACE );
    m_isosurfaceAct->setStatusTip( tr( "iso surface" ) );
    connect( m_isosurfaceAct, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_isoLineAct = new FNAction( QIcon( ":/icons/isoline.png" ), tr( "iso line" ), this, Fn::Algo::ISOLINE );
    m_isoLineAct->setStatusTip( tr( "iso line" ) );
    connect( m_isoLineAct, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_distanceMapAct = new FNAction( QIcon( ":/icons/distance.png" ), tr( "distance map" ), this, Fn::Algo::DISTANCE_MAP );
    m_distanceMapAct->setStatusTip( tr( "distance map" ) );
    connect( m_distanceMapAct, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_gaussAct = new FNAction( QIcon( ":/icons/gauss.png" ), tr( "gauss filter" ), this, Fn::Algo::GAUSS );
    m_gaussAct->setStatusTip( tr( "gauss filter" ) );
    connect( m_gaussAct, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_medianAct = new FNAction( QIcon( ":/icons/median.png" ), tr( "median filter" ), this, Fn::Algo::MEDIAN );
    m_medianAct->setStatusTip( tr( "median filter" ) );
    connect( m_medianAct, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_createROIAction = new FNAction( QIcon( ":/icons/tmp1.png" ), tr( "create ROI" ), this, Fn::Algo::CREATE_ROI );
    m_createROIAction->setStatusTip( tr( "create ROI" ) );
    connect( m_createROIAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_createNewAct = new FNAction( QIcon( ":/icons/tmp1.png" ), tr( "create new" ), this, Fn::Algo::CREATE_NEW_DATASET );
    m_createNewAct->setStatusTip( tr( "create a new empty dataset with the dimensions of the selected dataset" ) );
    connect( m_createNewAct, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );


    m_testAct = new FNAction( QIcon( ":/icons/tmp3.png" ), tr( "Test" ), this, Fn::Algo::TEST );
    m_testAct->setStatusTip( tr( "Test Action 1" ) );
    connect( m_testAct, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_qball4Act = new FNAction( QIcon( ":/icons/qball4.png" ), tr( "QBall 4th order" ), this, Fn::Algo::QBALL4 );
    m_qball4Act->setStatusTip( tr( "QBall 4th order" ) );
    connect( m_qball4Act, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_qball6Act = new FNAction( QIcon( ":/icons/qball6.png" ), tr( "QBall 6th order" ), this, Fn::Algo::QBALL6 );
    m_qball6Act->setStatusTip( tr( "QBall 6th order" ) );
    connect( m_qball6Act, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_qball8Act = new FNAction( QIcon( ":/icons/qball8.png" ), tr( "QBall 8th order" ), this, Fn::Algo::QBALL8 );
    m_qball8Act->setStatusTip( tr( "QBall 8th order" ) );
    connect( m_qball8Act, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_tensorAct = new FNAction( QIcon( ":/icons/fittensor.png" ), tr( "Tensor fit" ), this, Fn::Algo::TENSORFIT );
    m_tensorAct->setStatusTip( tr( "tensor fit" ) );
    connect( m_tensorAct, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_sDAct = new FNAction( QIcon( ":/icons/sd.png" ), tr( "Spherical deconvolution" ), this, Fn::Algo::SD );
    m_sDAct->setStatusTip( tr( "spherical deconvolution" ) );
    connect( m_sDAct, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_faAct = new FNAction( QIcon( ":/icons/calcfa.png" ), tr( "Calc FA" ), this, Fn::Algo::FA );
    m_faAct->setStatusTip( tr( "calc FA" ) );
    connect( m_faAct, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_evAct = new FNAction( QIcon( ":/icons/eigenvector.png" ), tr( "Calc Eigen Vector" ), this, Fn::Algo::EV );
    m_evAct->setStatusTip( tr( "calc Eigen Vector" ) );
    connect( m_evAct, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_binghamAction = new FNAction( QIcon( ":/icons/tmpx.png" ), tr( "Fit Bingham" ), this, Fn::Algo::BINGHAM );
    m_binghamAction->setStatusTip( tr( "fit bingham distribution" ) );
    connect( m_binghamAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_fiberTrackingAct = new FNAction( QIcon( ":/icons/tmpf.png" ), tr( "tensor tracking" ), this, Fn::Algo::TENSOR_TRACK );
    m_fiberTrackingAct->setStatusTip( tr( "tensor tracking" ) );
    connect( m_fiberTrackingAct, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_crossingTrackingAct = new FNAction( QIcon( ":/icons/tmpf.png" ), tr( "tensor tracking with fiber crossings" ), this, Fn::Algo::CROSSING_TRACK );
    m_crossingTrackingAct->setStatusTip( tr( "tensor tracking with crossings" ) );
    connect( m_crossingTrackingAct, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_fiberThinningAct = new FNAction( QIcon( ":/icons/tmpf.png" ), tr( "fiber thinning" ), this, Fn::Algo::FIBER_THINNING );
    m_fiberThinningAct->setStatusTip( tr( "fiber thinning" ) );
    connect( m_fiberThinningAct, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_fiberTractDensityAct = new FNAction( QIcon( ":/icons/tmpf.png" ), tr( "tract density" ), this, Fn::Algo::TRACT_DENSITY );
    m_fiberTractDensityAct->setStatusTip( tr( "tract density" ) );
    connect( m_fiberTractDensityAct, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_fiberTractColorAct = new FNAction( QIcon( ":/icons/palette_brush.png" ), tr( "tract color" ), this, Fn::Algo::TRACT_COLOR );
    m_fiberTractColorAct->setStatusTip( tr( "tract color" ) );
    connect( m_fiberTractColorAct, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );


    m_bingham2tensorAction = new FNAction( QIcon( ":/icons/tmpx.png" ), tr( "Bingham 2 Tensor" ), this, Fn::Algo::BINGHAM_2_TENSOR );
    m_bingham2tensorAction->setStatusTip( tr( "create tensors from bingham fit" ) );
    connect( m_bingham2tensorAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_cutSelectedFibersAction = new FNAction( QIcon( ":/icons/cut.png" ), tr( "Cut selected fibers" ), this, Fn::Algo::CUT_SELECTED_FIBERS );
    m_cutSelectedFibersAction->setStatusTip( tr( "create new fiber dataset from selected fibers" ) );
    connect( m_cutSelectedFibersAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_bundleAction = new FNAction( QIcon( ":/icons/bundle.png" ), tr( "Bundle connections" ), this, Fn::Algo::BUNDLE );
    m_bundleAction->setStatusTip( tr( "create new fiber dataset from bundling of selected connections" ) );
    connect( m_bundleAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_makeConsAction = new FNAction( QIcon( ":/icons/new_cons.png" ), tr( "Make new connections" ), this, Fn::Algo::MAKECONS );
    m_makeConsAction->setStatusTip( tr( "create new connections for bundling from thresholded connectivity" ) );
    connect( m_makeConsAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_avgConAction = new FNAction( QIcon( ":/icons/tmpx.png" ), tr( "Calculate average connectivity map" ), this, Fn::Algo::AVG_CON );
    m_avgConAction->setStatusTip( tr( "Calculate average connectivity between ROI in glyphset and put them in the value buffer of the surfaces" ) );
    connect( m_avgConAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_avgConRZAction = new FNAction( QIcon( ":/icons/tmpx.png" ), tr( "Calculate average connectivity map with r-to-z transfrom" ), this, Fn::Algo::AVG_CON_RZ );
    m_avgConRZAction->setStatusTip( tr( "Calculate average connectivity between r-to-z transformed values, result is transfromed back z-to-r" ) );
    connect( m_avgConRZAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_littleBrainsAction = new FNAction( QIcon( ":/icons/tmpx.png" ), tr( "Create little brains" ), this, Fn::Algo::LITTLE_BRAINS );
    m_littleBrainsAction->setStatusTip( tr( "Create little brains at all points painted (different from background color) in the current color layer" ) );
    connect( m_littleBrainsAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_sh2meshAction = new FNAction( QIcon( ":/icons/tmpx.png" ), tr( "convert sh 2 mesh" ), this, Fn::Algo::SH_2_MESH );
    m_sh2meshAction->setStatusTip( tr( "create a mesh dataset from the currently displayed SH glyphs" ) );
    connect( m_sh2meshAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_loopSubDAction = new FNAction( QIcon( ":/icons/tmpx.png" ), tr( "loop subdivision" ), this, Fn::Algo::LOOP_SUBDIVISION );
    m_loopSubDAction->setStatusTip( tr( "loop subdivision" ) );
    connect( m_loopSubDAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_meshTimeSeriesAction = new FNAction( QIcon( ":/icons/tmpx.png" ), tr( "create time series mesh" ), this, Fn::Algo::MESH_TIME_SERIES );
    m_meshTimeSeriesAction->setStatusTip( tr( "mesh time series" ) );
    connect( m_meshTimeSeriesAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_meshCorrelationAction = new FNAction( QIcon( ":/icons/tmpx.png" ), tr( "correlation" ), this, Fn::Algo::MESH_CORRELATION );
    m_meshCorrelationAction->setStatusTip( tr( "correlation" ) );
    connect( m_meshCorrelationAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_meshBiggestComponentAction = new FNAction( QIcon( ":/icons/tmpx.png" ), tr( "biggest component" ), this, Fn::Algo::MESH_BIGGEST_COMPONENT );
    m_meshBiggestComponentAction->setStatusTip( tr( "biggest component" ) );
    connect( m_meshBiggestComponentAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_meshDecimateAction = new FNAction( QIcon( ":/icons/tmpx.png" ), tr( "decimate" ), this, Fn::Algo::MESH_DECIMATE );
    m_meshDecimateAction->setStatusTip( tr( "decimate" ) );
    connect( m_meshDecimateAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_fiberResampleAction = new FNAction( QIcon( ":/icons/tmpx.png" ), tr( "resample" ), this, Fn::Algo::FIBER_DOWNSAMPLE );
    m_fiberResampleAction->setStatusTip( tr( "resample" ) );
    connect( m_fiberResampleAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_consToGlyphsetAction = new FNAction( QIcon( ":/icons/tmpx.png" ), tr( "make glyphset" ), this, Fn::Algo::CONS_TO_GLYPHSET );
    m_consToGlyphsetAction->setStatusTip( tr( "resample" ) );
    connect( m_consToGlyphsetAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_fiberBundlingAction = new FNAction( QIcon( ":/icons/tmpx.png" ), tr( "fiber bundling" ), this, Fn::Algo::FIBER_BUNDLING );
    m_fiberBundlingAction->setStatusTip( tr( "fiber bundling" ) );
    connect( m_fiberBundlingAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_deleteLittleBrainsAction = new FNAction( QIcon( ":/icons/tmpx.png" ), tr( "little brain delete" ), this, Fn::Algo::DELETE_LITTLE_BRAINS );
    m_deleteLittleBrainsAction->setStatusTip( tr( "delete little brains" ) );
    connect( m_deleteLittleBrainsAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_colorLittleBrainsAction = new FNAction( QIcon( ":/icons/tmpx.png" ), tr( "load rgb on little brains" ), this, Fn::Algo::COLOR_LITTLE_BRAINS );
    m_colorLittleBrainsAction->setStatusTip( tr( "color little brains" ) );
    connect( m_colorLittleBrainsAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_applyROIAction = new FNAction( QIcon( ":/icons/tmpx.png" ), tr( "create glyphs" ), this, Fn::Algo::APPLY_ROI_BRAINS );
    m_applyROIAction->setStatusTip( tr( "apply color as ROI" ) );
    connect( m_applyROIAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_brainglMathAction = new FNAction( QIcon( ":/icons/calc.png" ), tr( "braingl math" ), this, Fn::Algo::BRAINGL_MATH );
    m_brainglMathAction->setStatusTip( tr( "math" ) );
    connect( m_brainglMathAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_flipXAction = new FNAction( QIcon( ":/icons/flipx.png" ), tr( "flip voxels on x axis" ), this, Fn::Algo::FLIP_X );
    m_flipXAction->setStatusTip( tr( "flip x" ) );
    connect( m_flipXAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_flipYAction = new FNAction( QIcon( ":/icons/flipy.png" ), tr( "flip voxels on y axis" ), this, Fn::Algo::FLIP_Y );
    m_flipYAction->setStatusTip( tr( "flip y" ) );
    connect( m_flipYAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );

    m_flipZAction = new FNAction( QIcon( ":/icons/flipz.png" ), tr( "flip voxels on z axis" ), this, Fn::Algo::FLIP_Z );
    m_flipZAction->setStatusTip( tr( "flip z" ) );
    connect( m_flipZAction, SIGNAL( sigTriggered( Fn::Algo ) ), this, SLOT( slot( Fn::Algo ) ) );
}

void ToolBar::slot( Fn::Algo algo )
{
    //m_toolBarView->activateAlgo( algo );

    QModelIndex index = m_toolBarView->model()->index( m_toolBarView->getSelected(), (int)Fn::Property::D_DATASET_POINTER );
    QList<Dataset*>l;
    Dataset* ds = VPtr<Dataset>::asPtr( m_toolBarView->model()->data( index, Qt::DisplayRole ) );
    QList<QVariant>dsList =  m_toolBarView->model()->data( m_toolBarView->model()->index( 0, (int)Fn::Property::D_DATASET_LIST ), Qt::DisplayRole ).toList();

    Qt::KeyboardModifiers mods = QApplication::queryKeyboardModifiers();
    switch ( algo )
    {
        case Fn::Algo::QBALL4:
            l = DWIAlgos::qBallSharp( ds, 4 );
            break;
        case Fn::Algo::QBALL6:
            l = DWIAlgos::qBallSharp( ds, 6 );
            break;
        case Fn::Algo::QBALL8:
            l = DWIAlgos::qBallSharp( ds, 8 );
            break;
        case Fn::Algo::TENSORFIT:
            l = DWIAlgos::tensorFit( ds );
            break;
        case Fn::Algo::SD:
        {
            m_sdw = new SDWidget( ds, dsList, this->parentWidget() );
            connect( m_sdw, SIGNAL( finished() ), this, SLOT( sdFinished() ) );
            m_sdw->show();
            break;
        }
        case Fn::Algo::FA:
            if ( ds->properties().get( Fn::Property::D_TYPE ) == (int)Fn::DatasetType::NIFTI_DWI )
            {
                l = DWIAlgos::calcFAFromDWI( ds );
            }
            else if ( ds->properties().get( Fn::Property::D_TYPE ) == (int)Fn::DatasetType::NIFTI_TENSOR )
            {
                l = DWIAlgos::calcFAFromTensor( ds );
            }
            break;
        case Fn::Algo::EV:
            if ( ds->properties().get( Fn::Property::D_TYPE ) == (int)Fn::DatasetType::NIFTI_DWI )
            {
                l = DWIAlgos::calcEVFromDWI( ds );
            }
            else if ( ds->properties().get( Fn::Property::D_TYPE ) == (int)Fn::DatasetType::NIFTI_TENSOR )
            {
                l = DWIAlgos::calcEVFromTensor( ds );
            }
            break;
        case Fn::Algo::BINGHAM:
            l = DWIAlgos::fitBingham( ds );
            break;
        case Fn::Algo::SH_2_MESH:
            l = DWIAlgos::sh2mesh( ds );
            break;
        case Fn::Algo::TENSOR_TRACK:
        {
            m_ttw = new TensorTrackWidget( ds, dsList, this->parentWidget() );
            connect( m_ttw, SIGNAL( finished() ), this, SLOT( tensorTrackFinished() ) );
            m_ttw->show();
            break;
        }
        case Fn::Algo::CROSSING_TRACK:
        {
            m_ctw = new CrossingTrackWidget( ds, dsList, this->parentWidget() );
            connect( m_ctw, SIGNAL( finished() ), this, SLOT( crossingTrackFinished() ) );
            m_ctw->show();
            break;
        }
        case Fn::Algo::ISOSURFACE:
            l = ScalarAlgos::isoSurface( ds );
            break;
        case Fn::Algo::ISOLINE:
            l = ScalarAlgos::isoLine( ds );
            break;
        case Fn::Algo::DISTANCE_MAP:
            l = ScalarAlgos::distanceMap( ds );
            break;
        case Fn::Algo::GAUSS:
            l = ScalarAlgos::gauss( ds );
            break;
        case Fn::Algo::MEDIAN:
            l = ScalarAlgos::median( ds );
            break;
        case Fn::Algo::CREATE_ROI:
            l = ScalarAlgos::createROI( ds );
            break;
        case Fn::Algo::BRAINGL_MATH:
            m_mw = new MathWidget( dynamic_cast<DatasetScalar*>( ds ), this->parentWidget() );
            m_mw->show();
            break;
        case Fn::Algo::CREATE_NEW_DATASET:
            l = ScalarAlgos::createNew( ds );
            break;
        case Fn::Algo::FIBER_THINNING:
            l = FiberAlgos::thinOut( ds );
            break;
        case Fn::Algo::FIBER_DOWNSAMPLE:
            l = FiberAlgos::downSample( ds );
            break;
        case Fn::Algo::FIBER_BUNDLING:
            m_fbw = new FiberBundleWidget( ds, this->parentWidget() );
            connect( m_fbw, SIGNAL( finished() ), this, SLOT( fiberBundlingFinished() ) );
            m_fbw->show();
            break;
        case Fn::Algo::TRACT_DENSITY:
            l = FiberAlgos::tractDensity( ds );
            break;
        case Fn::Algo::TRACT_COLOR:
            l = FiberAlgos::tractColor( ds );
            break;
        case Fn::Algo::BINGHAM_2_TENSOR:
            l = DWIAlgos::bingham2DWI( ds );
            break;
        case Fn::Algo::TEST:
            break;
        case Fn::Algo::NONE:
            break;
        case Fn::Algo::QBALL:
            break;
        case Fn::Algo::CUT_SELECTED_FIBERS:
            if ( ds->properties().get( Fn::Property::D_TYPE ) == (int)Fn::DatasetType::FIBERS )
            {
                l = FiberAlgos::cutSelecteded( ds );
            }
            break;
        case Fn::Algo::BUNDLE:
            qDebug() << "BUNDLING...";
            {
                Connections* cons = ( (DatasetCons*) ds )->getCons();
                m_bw = new BundlingWidget( cons, this->parentWidget() );
                connect( m_bw, SIGNAL( finished() ), this, SLOT( bundlingFinished() ) );
                m_bw->show();
            }
            break;
        case Fn::Algo::MAKECONS:
            qDebug() << "Making new connections...";
            l = ((DatasetGlyphset*)ds)->createConnections();
            break;
        case Fn::Algo::AVG_CON:
            ( (DatasetGlyphset*)ds )->avgCon();
            break;
        case Fn::Algo::AVG_CON_RZ:
            ( (DatasetGlyphset*)ds)->avgConRtoZ();
            break;
        case Fn::Algo::LITTLE_BRAINS:
            if ( mods & Qt::ShiftModifier )
            {
                ( (DatasetGlyphset*)ds )->deleteLittleBrains();
            }
            else if ( mods & Qt::ControlModifier )
            {
                ( (DatasetGlyphset*)ds )->deleteRowFromMatrix();
            }
            else
            {
                ( (DatasetGlyphset*)ds )->makeLittleBrains();
            };
            break;
        case Fn::Algo::LOOP_SUBDIVISION:
            l = MeshAlgos::loopSubdivision( ds );
            break;
        case Fn::Algo::MESH_BIGGEST_COMPONENT:
            l = MeshAlgos::biggestComponent( ds );
            break;
        case Fn::Algo::MESH_DECIMATE:
            l = MeshAlgos::decimate( ds );
            break;
        case Fn::Algo::MESH_TIME_SERIES:
        {
            std::vector< QPair<QString, QList<Fn::DatasetType> > >filter;
            QList<Fn::DatasetType>types;
            types.push_back(Fn::DatasetType::MESH_BINARY );
            types.push_back(Fn::DatasetType::MESH_ASCII );
            types.push_back(Fn::DatasetType::MESH_ISOSURFACE );
            QPair<QString, QList<Fn::DatasetType> > d1( "mesh", types );
            filter.push_back( d1 );

            DatasetSelectionWidget* dsw = new DatasetSelectionWidget( filter, dsList );
            connect( dsw, SIGNAL( signalSelected( QList<QVariant> ) ), this, SLOT( slotMeshSelected( QList<QVariant> ) ) );
            dsw->show();
            break;
        }
        case Fn::Algo::MESH_CORRELATION:
        {
            m_cw = new CorrelationWidget( ds, this->parentWidget() );
            connect( m_cw, SIGNAL( finished() ), this, SLOT( correlationFinished() ) );
            m_cw->show();
            break;
        }
        case Fn::Algo::CONS_TO_GLYPHSET:
        {
            DatasetCons* cons = (DatasetCons*)ds;
            cons->cons->hashEdges();
            DatasetConGlyphs* consglyphs = new DatasetConGlyphs(QDir("new_conglyphs"));
            consglyphs->setCons(cons->cons);
            l.push_back(consglyphs);
            break;
        }
        case Fn::Algo::DELETE_LITTLE_BRAINS:
        {
            ( (DatasetGlyphset*)ds )->deleteLittleBrains();
            break;
        }
        case Fn::Algo::COLOR_LITTLE_BRAINS:
        {
            ( (DatasetGlyphset*) ds )->colorLittleBrains();
            break;
        }
        case Fn::Algo::APPLY_ROI_BRAINS:
            if ( mods & Qt::ShiftModifier )
            {
                ( (DatasetGlyphset*) ds )->switchGlyphsOff();
            }
            else
            {
                ( (DatasetGlyphset*) ds )->applyROI();
                break;
            }
            break;
        case Fn::Algo::FLIP_X:
            ( (DatasetNifti*) ds )->flipX();
            break;
        case Fn::Algo::FLIP_Y:
            ( (DatasetNifti*) ds )->flipY();
            break;
        case Fn::Algo::FLIP_Z:
            ( (DatasetNifti*) ds )->flipZ();
            break;

    }
    for ( int i = 0; i < l.size(); ++i )
    {
        index = m_toolBarView->model()->index( m_toolBarView->model()->rowCount(), (int)Fn::Property::D_NEW_DATASET );
        m_toolBarView->model()->setData( index, VPtr<Dataset>::asQVariant( l[i] ), Qt::DisplayRole );
    }

}

void ToolBar::slotSelectionChanged( int type )
{
    this->clear();
    switch ( (Fn::DatasetType)type )
    {
        case Fn::DatasetType::NIFTI_SCALAR:
        {
            this->addAction( m_isosurfaceAct );
            this->addAction( m_isoLineAct );
            this->addAction( m_distanceMapAct );
            this->addAction( m_gaussAct );
            this->addAction( m_medianAct );
            this->addAction( m_createROIAction );
            this->addAction( m_brainglMathAction );
            this->addAction( m_flipXAction );
            this->addAction( m_flipYAction );
            this->addAction( m_flipZAction );
            break;
        }
        case Fn::DatasetType::NIFTI_VECTOR:
        {
            //this->addAction( m_vectorAction1 );
            this->addAction( m_flipXAction );
            this->addAction( m_flipYAction );
            this->addAction( m_flipZAction );
            break;
        }
        case Fn::DatasetType::NIFTI_TENSOR:
        {
            this->addAction( m_faAct );
            this->addAction( m_evAct );
            this->addAction( m_fiberTrackingAct );
            this->addAction( m_crossingTrackingAct );
//            this->addAction( m_flipXAction );
//            this->addAction( m_flipYAction );
//            this->addAction( m_flipZAction );
            break;
        }
        case Fn::DatasetType::NIFTI_FMRI:
        {
            this->addAction( m_meshTimeSeriesAction );
            this->addAction( m_flipXAction );
            this->addAction( m_flipYAction );
            this->addAction( m_flipZAction );
            break;
        }
        case Fn::DatasetType::NIFTI_SH:
        {
            this->addAction( m_binghamAction );
//            this->addAction( m_sh2meshAction );
//            this->addAction( m_flipXAction );
//            this->addAction( m_flipYAction );
//            this->addAction( m_flipZAction );
            break;
        }
        case Fn::DatasetType::NIFTI_DWI:
        {
            //this->addAction( m_sDAct );
            this->addAction( m_qball4Act );
            this->addAction( m_qball6Act );
            this->addAction( m_qball8Act );
            this->addAction( m_tensorAct );
            this->addAction( m_faAct );
            this->addAction( m_evAct );
            this->addAction( m_flipXAction );
            this->addAction( m_flipYAction );
            this->addAction( m_flipZAction );
            break;
        }
        case Fn::DatasetType::NIFTI_BINGHAM:
        {
            this->addAction( m_bingham2tensorAction );
//            this->addAction( m_flipXAction );
//            this->addAction( m_flipYAction );
//            this->addAction( m_flipZAction );
            break;
        }
        case Fn::DatasetType::FIBERS:
        {
            this->addAction( m_cutSelectedFibersAction );
            this->addAction( m_fiberThinningAct );
            this->addAction( m_fiberTractDensityAct );
            this->addAction( m_fiberTractColorAct );
            this->addAction( m_fiberResampleAction );
            this->addAction( m_fiberBundlingAction );
            break;
        }
        case Fn::DatasetType::GLYPHSET:
        {
            this->addAction( m_colorLittleBrainsAction );
            this->addAction( m_makeConsAction );
            //this->addAction( m_avgConAction );
            this->addAction( m_avgConRZAction );
            this->addAction( m_littleBrainsAction );
            //this->addAction( m_deleteLittleBrainsAction );
            this->addAction( m_applyROIAction );
            break;
        }
        case Fn::DatasetType::CONS:
        {
            this->addAction( m_bundleAction );
            this->addAction( m_consToGlyphsetAction );
            break;
        }
        case Fn::DatasetType::MESH_ASCII:
        case Fn::DatasetType::MESH_BINARY:
        case Fn::DatasetType::MESH_ISOSURFACE:
        {
            this->addAction( m_loopSubDAction );
            this->addAction( m_meshBiggestComponentAction );
            this->addAction( m_meshDecimateAction );
            break;
        }
        case Fn::DatasetType::MESH_TIME_SERIES:
            this->addAction( m_meshCorrelationAction );
            break;
        default:
        {
            break;
        }
    }
}

void ToolBar::tensorTrackFinished()
{
    qDebug() << "toolbar track finished";
    QList<Dataset*>l = m_ttw->getFibs();
    for ( int i = 0; i < l.size(); ++i )
    {
        QModelIndex index = m_toolBarView->model()->index( m_toolBarView->model()->rowCount(), (int)Fn::Property::D_NEW_DATASET );
        m_toolBarView->model()->setData( index, VPtr<Dataset>::asQVariant( l[i] ), Qt::DisplayRole );
    }
    m_ttw->hide();
    destroy( m_ttw );
}

void ToolBar::crossingTrackFinished()
{
    qDebug() << "toolbar crossing track finished";
    QList<Dataset*>l = m_ctw->getFibs();
    for ( int i = 0; i < l.size(); ++i )
    {
        QModelIndex index = m_toolBarView->model()->index( m_toolBarView->model()->rowCount(), (int)Fn::Property::D_NEW_DATASET );
        m_toolBarView->model()->setData( index, VPtr<Dataset>::asQVariant( l[i] ), Qt::DisplayRole );
    }
    m_ctw->hide();
    destroy( m_ctw );
}

void ToolBar::sdFinished()
{
    qDebug() << "toolbar sd finished";
//    QList<Dataset*>l = m_sdw->getResult();
//    for ( int i = 0; i < l.size(); ++i )
//    {
//        QModelIndex index = m_toolBarView->model()->index( m_toolBarView->model()->rowCount(), (int)Fn::Property::D_NEW_DATASET );
//        m_toolBarView->model()->setData( index, VPtr<Dataset>::asQVariant( l[i] ), Qt::DisplayRole );
//    }
    m_sdw->hide();
    destroy( m_sdw );
}

void ToolBar::correlationFinished()
{
    qDebug() << "toolbar correlation finished";
    QList<Dataset*>l = m_cw->getResult();
    for ( int i = 0; i < l.size(); ++i )
    {
        QModelIndex index = m_toolBarView->model()->index( m_toolBarView->model()->rowCount(), (int)Fn::Property::D_NEW_DATASET );
        m_toolBarView->model()->setData( index, VPtr<Dataset>::asQVariant( l[i] ), Qt::DisplayRole );
    }
    m_cw->hide();
    destroy( m_cw );
}


void ToolBar::slotMeshSelected( QList<QVariant> meshes )
{
    qDebug() << VPtr<DatasetMesh>::asPtr( meshes[0] )->properties().get( Fn::Property::D_NAME ).toString();

    QModelIndex index = m_toolBarView->model()->index( m_toolBarView->getSelected(), (int)Fn::Property::D_DATASET_POINTER );
    Dataset* ds = VPtr<Dataset>::asPtr( m_toolBarView->model()->data( index, Qt::DisplayRole ) );

    TriangleMesh2* mesh = VPtr<DatasetMesh>::asPtr( meshes[0] )->getMesh();

    QList<Dataset*>l = MeshAlgos::meshTimeSeries( ds, mesh );

    for ( int i = 0; i < l.size(); ++i )
    {
        QModelIndex index = m_toolBarView->model()->index( m_toolBarView->model()->rowCount(), (int)Fn::Property::D_NEW_DATASET );
        m_toolBarView->model()->setData( index, VPtr<Dataset>::asQVariant( l[i] ), Qt::DisplayRole );
    }
}

void ToolBar::bundlingFinished()
{
    qDebug() << "bundling finished";
    QList<Dataset*>l = m_bw->getBundles();
    for ( int i = 0; i < l.size(); ++i )
    {
        QModelIndex index = m_toolBarView->model()->index( m_toolBarView->model()->rowCount(), (int)Fn::Property::D_NEW_DATASET );
        m_toolBarView->model()->setData( index, VPtr<Dataset>::asQVariant( l[i] ), Qt::DisplayRole );
    }
    //TODO: Ask Ralph about this: Destroy does not delete and disconnect, the old widgets keep inserting the new dataset multiple times...
    //m_bw->hide();
    //destroy( m_bw );
    delete m_bw;
}

void ToolBar::fiberBundlingFinished()
{
    qDebug() << "toolbar fiber bundling finished";
    QList<Dataset*>l = m_fbw->getFibs();
    for ( int i = 0; i < l.size(); ++i )
    {
        QModelIndex index = m_toolBarView->model()->index( m_toolBarView->model()->rowCount(), (int)Fn::Property::D_NEW_DATASET );
        m_toolBarView->model()->setData( index, VPtr<Dataset>::asQVariant( l[i] ), Qt::DisplayRole );
    }
    m_fbw->hide();
    destroy( m_fbw );
}
