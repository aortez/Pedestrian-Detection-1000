#include <fstream>
#include <highgui.h>
#include <sstream>
#include "Detector.h"
#include "Grid.h"
#include "util.h"
using namespace cv;
using namespace std;

const string SVM_FILENAME = "svm.txt";

Detector::Detector( void )
        : mCellDims( 6, 6 ),
          mDetectionWindow( 64, 128 ),
          mNumBins( 9 ),
          mShouldIgnoreSign( true )
{
    mSvmParameters.svm_type = CvSVM::C_SVC;
    mSvmParameters.C = 0.1;
    mSvmParameters.kernel_type = CvSVM::LINEAR;
    mSvmParameters.term_crit = cvTermCriteria( CV_TERMCRIT_ITER, 10000, 1e-6 );
}

void Detector::addTrainingFile( const string& fileName, const bool shouldShouldHogImages )
{
    vector< string > posFiles;
    vector< string > negFiles;
    ifstream file;
    file.open( fileName.c_str(), ios::in );
    assertTrue( file.is_open(), "Error: failed to open file: " + fileName );
    printf( "Opened training file: %s\n", fileName.c_str() );
    while( file.good() )
    {
        string line;
        getline( file, line );
        assertTrue( line.size() > 0, "Error: line too short to contain symbol" );
        const char symbol = line[ 0 ];
        if( symbol == '#' ) continue;
        assertTrue( line.size() > 2, "Error: line too short to contain filename" );
        const string filename = line.substr( 2, line.size() - 2 );
        switch( symbol )
        {
        case '-':
            negFiles.push_back( filename );
            break;
        case '+':
            posFiles.push_back( filename );
            break;
        default:
            assertTrue( false, "Error: invalid symbol: " + symbol );
            break;
        }
    }

    for( size_t i = 0; i < posFiles.size(); i++ )
    {
        Mat pos = imread( posFiles[ i ].c_str() );
        printf( "Adding positive training image: %s\n", posFiles[ i ].c_str() );
        addTrainingImage( pos, true, shouldShouldHogImages );
    }
    for( size_t i = 0; i < negFiles.size(); i++ )
    {
        Mat neg = imread( negFiles[ i ].c_str() );
        printf( "Adding negative training image: %s\n", negFiles[ i ].c_str() );
        addTrainingImage( neg, false, shouldShouldHogImages );
    }
}

void Detector::addTrainingImage( const cv::Mat& image, bool isPositive, bool shouldShowHogImage )
{
    // create grid of HOG cells
    Grid grid( cropToDetectionWindow( image ), mCellDims, mNumBins, mShouldIgnoreSign );

    // create hogImage
    if( shouldShowHogImage )
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
}

float Detector::classify( const cv::Mat& image, const bool shouldShowHogImage )
{
    printf( "Loading SVM state...\n" );
    {
        ifstream file;
        file.open( SVM_FILENAME.c_str(), ios::in );
        assertTrue( file.is_open(), "Error: failed to open SVM file: " + SVM_FILENAME + "\nPlease train detector first." );
        file.close();
    }
    mSvm.load( "svm.txt" );

    printf( "SVM loaded\n" );
    printf( "Classifying...\n" );

    // create grid of HOG cells
    Grid grid( cropToDetectionWindow( image ), mCellDims, mNumBins, mShouldIgnoreSign );

    // compute descriptors
    const vector< Mat >& descriptorVector = grid.getDescriptorVectors();
    double predictSum = 0;
    for( size_t i = 0; i < descriptorVector.size(); i++ )
    {
        const float predict = mSvm.predict( descriptorVector[ i ] );
        predictSum += predict;
    }
    const float predict = predictSum / descriptorVector.size();

    // create hogImage
    if( shouldShowHogImage )
    {
        std::stringstream ss;
        ss << predict;
        printf( "Image classification: %f\n", predict );
        Mat hog = grid.createHogImage( 5 );
        show( hog, ss.str().c_str() );
        waitKey();
    }

    return predict;
}

Mat Detector::cropToDetectionWindow( const cv::Mat& image )
{
    assertTrue( !image.empty(), "Error: invalid image" );

    const Vec2i center( image.cols / 2, image.rows / 2 );

    Rect roi( center[ 0 ] - mDetectionWindow[ 0 ] / 2,
              center[ 1 ] - mDetectionWindow[ 1 ] / 2,
              mDetectionWindow[ 0 ],
              mDetectionWindow[ 1 ] );
    Mat croppedImage = image( roi );

    return croppedImage;
}

void Detector::train( void )
{
    assert( mDescriptorVectors.size() == mDescriptorVectors.size() );
    assert( !mDescriptorVectors.empty() );
    for( size_t i = 0; i < mDescriptorVectors.size(); i++ )
    {
        if( i > 0 )
        {
            assertTrue( mDescriptorVectors[ i ].cols == mDescriptorVectors[ i - 1 ].cols,
                        "Error: descriptor vectors must be all of the same size." );
        }
    }

    // convert from vector of descriptors to a single image with a descriptor per row
    // (descriptor vectors rows X descriptor length cols)
    printf( "Preparing descriptor vectors...\n" );
    Mat descriptors( static_cast< int >( mDescriptorVectors.size() ), mDescriptorVectors[ 0 ].cols, CV_32FC1 );
    for( int y = 0; y < descriptors.rows; y++ )
    {
        for( int x = 0; x < descriptors.cols; x++ )
        {
            descriptors.at< float >( y, x ) = mDescriptorVectors[ y ].at< float >( x );
        }
    }
    Mat labels( mDescriptorVectorLabels.size(), 1, CV_32FC1 );
    for( int y = 0; y < labels.rows; y++ )
    {
        const float labelValue = ( mDescriptorVectorLabels[ y ] == true ) ? 1 : -1;
        labels.at< float >( y, 0 ) = labelValue;
    }

    printf( "Training SVM...\n" );
    mSvm.train( descriptors, labels, Mat(), Mat(), mSvmParameters );
    mSvm.save( SVM_FILENAME.c_str() );
}
