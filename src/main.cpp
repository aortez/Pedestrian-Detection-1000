#include <cv.h>
#include <highgui.h>
#include <iostream>
using namespace cv;
using namespace std;

/**
 * Entry point to app. Loads image from file system and feeds it through the algorithm.
 */
int main( int argc, const char* argv[] )
{
    if( argc != 2 )
    {
        cerr << "Please provide a path to an image" << endl;
        return -1;
    }

    // read input file
    Mat src = imread( argv[ 1 ], 1 );

    // Gradient computation
    Mat kernel_horz = Mat( 1, 3, CV_8UC1, Scalar( -1, 0, 1 ) ); // [-1, 0, 1]
    Mat kernel_vert = Mat( 3, 1, CV_8UC1, Scalar( -1, 0, 1 ) ); // [-1, 0, 1]^T
    Mat gradient_horz;
    Mat gradient_vert;
    filter2D( src, gradient_horz, -1, kernel_horz, Point( -1, -1 ) );
    filter2D( src, gradient_vert, -1, kernel_vert, Point( -1, -1 ) );

    // Debug: Show progress.
    namedWindow( "Progress", 1 );
    imshow( "Progress", src );
    waitKey();

    // Histograms -- Orientation Binning

    // Descriptor Blocks

    // Block Normalization

    // SVM classification

    return 0;
}
