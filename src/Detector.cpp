#include <highgui.h>
#include <sstream>
#include "Detector.h"
#include "Grid.h"
#include "util.h"
using namespace cv;

Detector::Detector( void )
{
    mSvmParameters.svm_type = CvSVM::C_SVC;
    mSvmParameters.kernel_type = CvSVM::LINEAR;
    mSvmParameters.term_crit = cvTermCriteria( CV_TERMCRIT_ITER, 100, 1e-6 );
}

void Detector::addTrainingImage(
        const cv::Mat& image,
        bool isPositive,
        bool shouldShowHogImage )
{
    // create grid of HOG cells
    const int numBins = 9;
    Size cellDims( 6, 6 );
    Grid grid( image, cellDims, numBins );

    // create hogImage
    if ( shouldShowHogImage )
    {
        Mat hog = grid.createHogImage( 5 );
        show( hog, ( isPositive ? "+" : "-" ) );
        waitKey();
    }

// compute descriptors
    const vector< Mat >& descriptorVector = grid.getDescriptorVectors();
    for( size_t i = 0; i < descriptorVector.size(); i++ )
    {
        mDescriptorVectors.push_back( descriptorVector[ i ] );
        mDescriptorVectorLabels.push_back( isPositive );
    }

    printf( "num descriptor vectors: %lu\n", descriptorVector.size() );
}

float Detector::classify(
        const cv::Mat& image,
        const bool shouldShowHogImage )
{
    // create grid of HOG cells
    const int numBins = 9;
    Size cellDims( 6, 6 );
    Grid grid( image, cellDims, numBins );

    // compute descriptors
    const vector< Mat >& descriptorVector = grid.getDescriptorVectors();
    double predictSum = 0;
    for ( size_t i = 0; i < descriptorVector.size(); i++ )
    {
        const float predict = mSvm.predict( descriptorVector[ i ] );
        printf( "\npredict: %f\n", predict );
        predictSum += predict;
    }

    // create hogImage
    if ( shouldShowHogImage )
    {
        std::stringstream ss;
        ss << predictSum;
        printf ( "Image classification: %f\n", predictSum );
        Mat hog = grid.createHogImage( 5 );
        show( hog, ss.str().c_str() );
        waitKey();
    }

    return predictSum;
}

void Detector::train( void )
{
    assert( mDescriptorVectors.size() == mDescriptorVectors.size() );
    assert( !mDescriptorVectors.empty() );
    for( size_t i = 0; i < mDescriptorVectors.size(); i++ )
    {
        if( i > 0 )
        {
            assert( mDescriptorVectors[ i ].cols == mDescriptorVectors[ i - 1 ].cols );
        }
    }

    // convert from vector of descriptors to a single image with a descriptor per row
    // (descriptor vectors rows X descriptor length cols)
    Mat descriptors( static_cast< int >( mDescriptorVectors.size() ), mDescriptorVectors[ 0 ].cols, CV_32FC1 );
    for ( int y = 0; y < descriptors.rows; y++ )
    {
        for ( int x = 0; x < descriptors.cols; x++ )
        {
            descriptors.at< float >( y, x ) = mDescriptorVectors[ y ].at< float >( x );
        }
    }
    Mat labels( mDescriptorVectorLabels.size(), 1, CV_32FC1 );
    for ( int y = 0; y < labels.rows; y++ )
    {
        const float labelValue = ( mDescriptorVectorLabels[ y ] == true ) ? 1 : -1;
//        printf( "label[%d] = %f\n", y, labelValue );
        labels.at< float >( y, 0 ) = labelValue;
    }

    mSvm.train( descriptors, labels, Mat(), Mat(), mSvmParameters );
}
