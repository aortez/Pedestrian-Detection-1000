#include <UnitTest++.h>
#include <cstdio>
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include "Cell.h"
#include "Grid.h"
using namespace cv;
using namespace std;

bool containsPerson( const Mat& img );

int main( int argc, const char* argv[] )
{
    Mat src;
    if( argc > 1 )
    {
        if ( argv[ 1 ] == string( "-test" ) )
        {
            return UnitTest::RunAllTests();
        }
        else
        {
            src = imread( argv[ 1 ] );
        }
    }
    else
    {
        src = imread( "test.png" );
    }

    cout << "Is a person: " << boolalpha << containsPerson( src ) << endl;
    return 0;
}

bool containsPerson( const Mat& src )
{
    // create grid of HOG cells
    const int numBins = 9;
    Size cellDims( 6, 6 );
    Grid grid( src, cellDims, numBins );

    // get descriptors
    const vector< Mat > descriptorVectors = grid.getDescriptorVectors();

    // create SVM
    CvSVMParams params;
    params.svm_type = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.term_crit = cvTermCriteria( CV_TERMCRIT_ITER, 100, 1e-6 );
    CvSVM svm;

    // train SVM
    for ( size_t i = 0; i < descriptorVectors.size(); i++ )
    {
        svm.train( trainingData, labels, Mat(), Mat(), params );
    }

    return false;
}
