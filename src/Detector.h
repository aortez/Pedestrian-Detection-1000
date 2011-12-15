#pragma once
#include <cv.h>
#include <opencv2/ml/ml.hpp>
#include <vector>

//! A two-class image classifer.  The Dectector first needs to be trained
//! using the 'addTrainingImage' or the 'addTrainingFile' methods.  After
//! the Detector has been trained it can classify images using the
//! 'classify' method.
class Detector
{
public:
    //! Constructor.
    Detector( void );

    //! Adds the images listed in the file to the training set.
    //! Each line should be composed of either a '+' or a '-' and
    //! a file name.  The '+' and '-' signs indicate positive and
    //! negative training images.
    //! example line:
    //! - Images/not_a_person.png
    void addTrainingFile(
            const std::string& fileName,
            bool shouldShowHogImages );

    //! Adds an image to the training set.
    void addTrainingImage(
            const cv::Mat& image,
            bool isPositive,
            bool shouldShowHogImage = false );

    //! Classifies the input image.  The sign of the output corresponds
    //! to the positive or negative classification.
    double classify(
            const cv::Mat& image,
            bool shouldShowHogImage = false );

    //! Trains the classifier.
    void train( void );

private:
    cv::Mat cropToDetectionWindow( const cv::Mat& image );

    cv::Size mCellDims;
    std::vector< cv::Mat > mDescriptorVectors;
    std::vector< bool > mDescriptorVectorLabels;
    const cv::Vec2i mDetectionWindow;
    const int mNumBins;
    const bool mShouldIgnoreSign;
    CvSVM mSvm;
    CvSVMParams mSvmParameters;
};
