/*
 * datasetglyphset.h
 *
 *  Created on: Apr 24, 2013
 *      Author: boettgerj
 */

#include "datasetcorrelation.h"

#include "../../gui/gl/pointglyphrenderer.h"
#include "../../gui/gl/diffpointglyphrenderer.h"
#include "../../gui/gl/vectorglyphrenderer.h"
#include "../../gui/gl/pieglyphrenderer.h"
#include "../../gui/gl/littlebrainrenderer.h"
#include "../../gui/gl/colormaprenderer.h"

#include "../../gui/gl/meshrenderer.h"

#include <qstring.h>

#ifndef DATASETGLYPHSET_H_
#define DATASETGLYPHSET_H_

class DatasetGlyphset: public DatasetCorrelation
{
    Q_OBJECT
public:
    DatasetGlyphset( QDir filename, float minThreshold, float maxThreshold = 1.0 );
    virtual ~DatasetGlyphset();

    void addSecondSurfaceSelector();

    void addProperties();

    void readConnectivity( QString filename );
    void addCorrelation( float** corr );

    void draw( QMatrix4x4 pMatrix, QMatrix4x4 mvMatrix, int width, int height, int renderMode, QString target );

    void setProperties();

    void makeCons();
    int consNumber;

    void makeVecs();
    int vecsNumber;

    void makePies();
    int maxNodeCount;

    void makeDiffPoints();
    int diffsNumber;

    QList<Dataset*> createConnections();

    void exportColors();
    void loadROI( QString filename, bool* roin );
    void initROI();
    void avgCon();
    void avgConRtoZ();
    void makeLittleBrains();
    void colorLittleBrains();
    void deleteLittleBrains();
    void switchGlyphsOff();
    void applyROI();
    void deleteRowFromMatrix();

    int m_tris_middle;
    int m_points_middle;
    bool m_is_split;

    bool* roi;
    bool* roi2;

private:
    int m_n;  //number of vertices, has to match size of matrix

    float* consArray;
    float* diffsArray;
    float* vecsArray;

    std::vector<float*>* pieArrays;
    std::vector<int>* numbers;

    PointGlyphRenderer* m_prenderer;
    DiffPointGlyphRenderer* m_dprenderer;
    VectorGlyphRenderer* m_vrenderer;
    PieGlyphRenderer* m_pierenderer;

    ColormapRenderer* m_colormapRenderer;

    int prevGeo, prevGlyph, prevCol, prevGlyphstyle, prevLR, prevColorMode, prevThreshSign;
    float prevThresh, prevMinlength;
    bool glyphsChanged;

    QString m_colors_name;

    std::vector<LittleBrainRenderer*> littleBrains;
    std::vector<TriangleMesh2*> littleMeshes;
    std::vector<QVector3D> shifts1;
    std::vector<QVector3D> shifts2;

    bool filter( int i, int j, int lr, float threshold, int sign );

private slots:
    void colorModeChanged( QVariant qv );
    void glyphStyleChanged( QVariant qv );
    void rotationChanged( QVariant qv );
    void littleBrainVisibilityChanged( QVariant qv );
    void slotCopyColors();
    void thresholdChanged( QVariant v );
    void thresholdPercChanged( QVariant v );
};

#endif /* DATASETGLYPHSET_H_ */
