/*
 * writer.h
 *
 * Created on: Jun 1, 2012
 * @author Ralph Schurade
 */

#ifndef WRITER_H_
#define WRITER_H_

#include "../thirdparty/nifti/nifti1_io.h"

#include <QFileInfo>
#include <QString>

class Dataset;
class TriangleMesh2;

class Writer
{
public:
    Writer( Dataset* dataset, QFileInfo fileName, QString file = "" );
    virtual ~Writer();

    bool save();

    nifti_image* createHeader( int dim );
    void addPropsToHeader( nifti_image* header );

private:
    Dataset* m_dataset;
    QFileInfo m_fileName;
    QString m_filter;

    void setDescrip( nifti_image* hdr, QString descrip );

    void saveRGB();
    void save1D();
    void saveROI();
    void saveConnexels();
    void saveOBJ();
    void saveVRML();
    void saveMeshJson();
    void saveMeshAsc();
    void saveBinaryConnectivity();
    void saveFibJson();
    void saveFibTrk();
};

#endif /* WRITER_H_ */
