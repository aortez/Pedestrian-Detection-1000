#include <highgui.h>
#include "util.h"
using namespace std;
using namespace cv;

void show(
        const Mat& img,
        const string& windowName,
        const int scale,
        const int interpolation_type )
{
    Mat out;
    resize( img, out, img.size() * scale, interpolation_type );
    namedWindow( windowName.c_str() );
    imshow( windowName.c_str(), out );
}

string toString( const Mat& img )
{
    stringstream ss;
    ss << "cols: " << img.cols << endl;
    ss << "rows: " << img.rows << endl;
    ss << "type: " << img.type() << endl;
    ss << "depth: " << img.depth() << endl;
    return ss.str();
}
