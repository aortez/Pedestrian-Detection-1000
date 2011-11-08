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

    // compute gradient images
    Mat gradient_horz( src.size(), CV_64FC1 );
    Mat gradient_vert( src.size(), CV_64FC1 );
    {
        Mat kernel_horz = Mat( 1, 3, CV_64FC1, Scalar( -1, 0, 1 ) ); // [-1, 0, 1]
        Mat kernel_vert = Mat( 3, 1, CV_64FC1, Scalar( -1, 0, 1 ) ); // [-1, 0, 1]^T
        filter2D( src, gradient_horz, -1, kernel_horz );
        filter2D( src, gradient_vert, -1, kernel_vert );
    }

    // compute orientation image
    Mat orientation( gradient_horz.size(), CV_64FC1 );
    {
        for ( int y = 0; y < orientation.rows; y++ )
        {
            for ( int x = 0; x < orientation.cols; x++ )
            {
                const double h = gradient_horz.at< double >( y, x );
                const double v = gradient_vert.at< double >( y, x );
                orientation.at< double >( y, x ) = atan2( v, h );
            }
        }
    }

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
