/*
 * dwialgos.h
 *
 *  Created on: Jun 18, 2012
 *      Author: schurade
 */

#ifndef DWIALGOS_H_
#define DWIALGOS_H_

#include <QtCore/QList>

class Dataset3D;
class DatasetBingham;
class DatasetDWI;
class DatasetNifti;
class DatasetTensor;
class DatasetSH;
class TriangleMesh;

class DWIAlgos
{
public:
    static DatasetSH* qBall( DatasetDWI* ds );
    static DatasetSH* qBallSharp( DatasetDWI* ds, int order );
    static DatasetTensor* tensorFit( DatasetDWI* ds );
    static DatasetScalar* calcFAFromDWI( DatasetDWI* ds );
    static QList<Dataset*> calcEVFromDWI( DatasetDWI* ds );
    static DatasetScalar* calcFAFromTensor( DatasetTensor* ds );
    static QList<Dataset*> calcEVFromTensor( DatasetTensor* ds );
    static QList<Dataset*> fitBingham( DatasetSH* ds );
    static QList<Dataset*> bingham2Tensor( DatasetBingham* ds );

    static QList<Dataset*> tensorTrack( DatasetTensor* ds );

private:
    DWIAlgos();
    virtual ~DWIAlgos();

    static double pow2( double v )
    {
        return v * v;
    };
    static double pow3( double v )
    {
        return v * v * v;
    };
};

#endif /* DWIALGOS_H_ */
