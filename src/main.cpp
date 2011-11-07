#include <stdio.h>
#include <cv.h>

using namespace cv;

/**
 * Entry point to app. Loads image from file system and feeds it through the algorithm.
 */
int main(int argc, const char* argv[]) {
	
	if (argc != 2) {
		cerr << "Please provide a path to an image" << endl;
		return -1;
	}
	
	Mat src = imread(argv[1], 1).data
	if (!src) {
		cerr << "Unable to load image file " << argv[1] << endl;
		return -1;
	}
	
	// Gradient computation
	Mat kernel = Mat(1, 3, CV_8UC1, Scalar(-1, 0, 1)); // [-1, 0, 1] and [-1,0,1]^T
	filter2D(&src, &src, -1, kernel, Point(-1, -1), 0, BORDER_DEFAULT); // TODO: Not sure if in place copy works here...
	
	// Debug: Show progress.
	namedWindow("Progress", 1);
	imshow("Progress", src);
	waitKey();
	
	// Histograms -- Orientation Binning
	
	// Descriptor Blocks
	
	// Block Normalization
	
	// SVM classification
	
	return 1;
}
