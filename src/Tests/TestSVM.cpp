#include <cv.h>
#include <highgui.h>
#include <opencv2/ml/ml.hpp>
#include "UnitTest++.h"
#include "util.h"

using namespace cv;

SUITE( SVMTest )
{
TEST( SimpleTest )
{
    // Data for visual representation
    int width = 512, height = 512;
    Mat image = Mat::zeros( height, width, CV_8UC3 );

    // Set up training data
    Mat labels = ( Mat_< float >( 3, 1 ) << 1, 1, -1, -1 );
    Mat trainingData = ( Mat_< float >( 3, 2 ) << 501, 10, 255, 10, 501, 255, 10, 501 );
    Mat trainingData2 = ( Mat_< float >( 3, 2 ) << 401, 110, 255, 110, 401, 255, 110, 401 );

    // Set up SVM's parameters
    CvSVMParams params;
    params.svm_type = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.term_crit = cvTermCriteria( CV_TERMCRIT_ITER, 100, 1e-6 );

    // Train the SVM
    CvSVM SVM;
    SVM.train( trainingData, labels, Mat(), Mat(), params );
    SVM.train( trainingData2, labels, Mat(), Mat(), params );

    // Show the decision regions given by the SVM
    Vec3b green( 0, 255, 0 ), blue( 255, 0, 0 );
    for( int i = 0; i < image.rows; ++i )
        for( int j = 0; j < image.cols; ++j )
        {
            Mat sampleMat = ( Mat_< float >( 1, 2 ) << i, j );
            float response = SVM.predict( sampleMat );

            if( response == 1 )
                image.at< Vec3b >( j, i ) = green;
            else if( response == -1 ) image.at< Vec3b >( j, i ) = blue;
        }

    // Show the training data
    int thickness = -1;
    int lineType = 8;
    circle( image, Point( 501, 10 ), 5, Scalar( 0, 0, 0 ), thickness, lineType );
    circle( image, Point( 255, 10 ), 5, Scalar( 0, 255, 255 ), thickness, lineType );
    circle( image, Point( 501, 255 ), 5, Scalar( 255, 255, 255 ), thickness, lineType );
    circle( image, Point( 10, 501 ), 5, Scalar( 255, 255, 255 ), thickness, lineType );
    circle( image, Point( 401, 110 ), 5, Scalar( 0, 0, 0 ), thickness, lineType );
    circle( image, Point( 255, 110 ), 5, Scalar( 0, 255, 255 ), thickness, lineType );
    circle( image, Point( 401, 255 ), 5, Scalar( 255, 255, 255 ), thickness, lineType );
    circle( image, Point( 110, 401 ), 5, Scalar( 255, 255, 255 ), thickness, lineType );

    // Show support vectors
    thickness = 2;
    lineType = 8;
    int c = SVM.get_support_vector_count();

    for( int i = 0; i < c; ++i )
    {
        const float* v = SVM.get_support_vector( i );
        circle( image, Point( (int) v[ 0 ], (int) v[ 1 ] ), 6, Scalar( 128, 128, 128 ), thickness, lineType );
    }

    show( image, "SVM Simple Example" );
    waitKey( 0 );
}
}
