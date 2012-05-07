/*
 * datasetscalar.h
 *
 *  Created on: May 4, 2012
 *      Author: schurade
 */

#ifndef DATASETSCALAR_H_
#define DATASETSCALAR_H_

#include <vector>

#include "nifti/nifti1_io.h"

#include "dataset.h"

class DatasetScalar : public Dataset
{
public:
    DatasetScalar( std::vector<float> data );
    virtual ~DatasetScalar();

    void parseNiftiHeader( nifti_image* header );
private:
    std::vector<float> m_data;

    int ndim;       /*!< last dimension greater than 1 (1..7) */
    int nx;         /*!< dimensions of grid array             */
    int ny;         /*!< dimensions of grid array             */
    int nz;         /*!< dimensions of grid array             */
    int nt;         /*!< dimensions of grid array             */
    int nu;         /*!< dimensions of grid array             */
    int nv;         /*!< dimensions of grid array             */
    int nw;         /*!< dimensions of grid array             */
    int dim[8];     /*!< dim[0]=ndim, dim[1]=nx, etc.         */
    size_t nvox;    /*!< number of voxels = nx*ny*nz*...*nw   */
    int nbyper;     /*!< bytes per voxel, matches datatype    */
    int datatype;   /*!< type of data in voxels: DT_* code    */

    float dx;           /*!< grid spacings      */
    float dy;           /*!< grid spacings      */
    float dz;           /*!< grid spacings      */
    float dt;           /*!< grid spacings      */
    float du;           /*!< grid spacings      */
    float dv;           /*!< grid spacings      */
    float dw;           /*!< grid spacings      */
    float pixdim[8];    /*!< pixdim[1]=dx, etc. */

    float scl_slope;    /*!< scaling parameter - slope        */
    float scl_inter;    /*!< scaling parameter - intercept    */

    float cal_min;      /*!< calibration parameter, minimum   */
    float cal_max;      /*!< calibration parameter, maximum   */

    int qform_code;     /*!< codes for (x,y,z) space meaning  */
    int sform_code;     /*!< codes for (x,y,z) space meaning  */

    int freq_dim;       /*!< indexes (1,2,3, or 0) for MRI    */
    int phase_dim;      /*!< directions in dim[]/pixdim[]     */
    int slice_dim;      /*!< directions in dim[]/pixdim[]     */

    int slice_code;         /*!< code for slice timing pattern    */
    int slice_start;        /*!< index for start of slices        */
    int slice_end;          /*!< index for end of slices          */
    float slice_duration;   /*!< time between individual slices   */

    /*! quaternion transform parameters
     [when writing a dataset, these are used for qform, NOT qto_xyz]   */
    float quatern_b, quatern_c, quatern_d, qoffset_x, qoffset_y, qoffset_z, qfac;

    mat44 qto_xyz;      /*!< qform: transform (i,j,k) to (x,y,z) */
    mat44 qto_ijk;      /*!< qform: transform (x,y,z) to (i,j,k) */

    mat44 sto_xyz;      /*!< sform: transform (i,j,k) to (x,y,z) */
    mat44 sto_ijk;      /*!< sform: transform (x,y,z) to (i,j,k) */

    float toffset;      /*!< time coordinate offset */

    int xyz_units;      /*!< dx,dy,dz units: NIFTI_UNITS_* code  */
    int time_units;     /*!< dt       units: NIFTI_UNITS_* code  */

    int nifti_type;     /*!< 0==ANALYZE,
                             1==NIFTI-1 (1 file),
                             2==NIFTI-1 (2 files),
                             3==NIFTI-ASCII (1 file) */
    int intent_code;        /*!< statistic type (or something)       */
    float intent_p1;        /*!< intent parameters                   */
    float intent_p2;        /*!< intent parameters                   */
    float intent_p3;        /*!< intent parameters                   */
    char intent_name[16];   /*!< optional description of intent data */

    char descrip[80];       /*!< optional text to describe dataset   */
    char aux_file[24];      /*!< auxiliary filename                  */

    char *fname;            /*!< header filename (.hdr or .nii)         */
    char *iname;            /*!< image filename  (.img or .nii)         */
    int iname_offset;       /*!< offset into iname where data starts    */
    int swapsize;           /*!< swap unit in image data (might be 0)   */
    int byteorder;          /*!< byte order on disk (MSB_ or LSB_FIRST) */
};

#endif /* DATASETSCALAR_H_ */
