#include "Detector.h"
#include "Grid.h"
using namespace cv;

Detector::Detector( void )
{
    mSvmParameters.svm_type = CvSVM::C_SVC;
    mSvmParameters.kernel_type = CvSVM::LINEAR;
    mSvmParameters.term_crit = cvTermCriteria( CV_TERMCRIT_ITER, 100, 1e-6 );
}

void Detector::train( const cv::Mat& trainingImage, bool isPositive )
{
    // create grid of HOG cells
    const int numBins = 9;
    Size cellDims( 6, 6 );
    Grid grid( trainingImage, cellDims, numBins );

    // compute descriptors
    const vector< Mat > descriptorVector = grid.getDescriptorVectors();

    const int labelValue = ( isPositive ) ? 1 : -1;
    const Mat label = ( Mat_< float >( 1, 1 ) << labelValue );
    for ( size_t i = 0; i < descriptorVector.size(); i++ )
    {
        mSvm.train( descriptorVector[ i ], label, Mat(), Mat(), mSvmParameters );
    }

}
