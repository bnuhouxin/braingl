/*
 * dataset3d.h
 *
 * Created on: May 9, 2012
 * @author Ralph Schurade
 */

#ifndef DATASETDWI_H_
#define DATASETDWI_H_

#include "datasetnifti.h"

#include <QVector3D>
#include <QVector>

class DatasetDWI: public DatasetNifti
{
    Q_OBJECT

public:
    DatasetDWI( QDir fileName, std::vector<float>* data, std::vector<float> bvals, std::vector<QVector3D> bvecs, nifti_image* header );
    virtual ~DatasetDWI();

    std::vector<float>* getData();

    std::vector<float>* getB0Data();

    std::vector<float> getBvals();

    std::vector<QVector3D> getBvecs();

    void draw( QMatrix4x4 pMatrix, QMatrix4x4 mvMatrix, int width, int height, int renderMode, QString target );
    QString getValueAsString( int x, int y, int z );

    void flipX();
    void flipY();
    void flipZ();

    bool isOK() { return m_isOK; };

private:
    void examineDataset();
    void createTexture();

    std::vector<float> m_data;
    std::vector<float> m_b0Data;
    std::vector<float> m_bvals;
    std::vector<QVector3D> m_bvecs;

    bool m_isOK;

private slots:
    void selectTexture();

};

#endif /* DATASET3D_H_ */
