#pragma once
#include <vector>
namespace cv { class Mat; }

class Cell
{
public:
    //! Constructor.
    Cell( int numBins, bool shouldIgnoreSign );

    //! Bins up an image
    void addImage( const cv::Mat& src );

    //! Adds a pixel to the cell.
    void addPixel( double gradientAngle, float weight );

    //! Get the number of votes in the specified bin.
    double bin( int binIndex ) const;

    //! Gets the angle represented by the specified bin.
    double binAngle( int binIndex ) const;

    //! Gets the normalized [ 0, 1 ] number of votes in the specified bin.
    float binNormalized( int binIndex ) const;

    //! Creates a visual representation of the cell's HOG.
    cv::Mat drawHOG( int cellScale = 10 ) const;

    //! Gets the Cell's HOG.
    const std::vector< float >& getHog( void ) const;

    //! Gets the sum of the cell's votes.
    float getSum( void ) const;

    //! Returns the number of bins in the cell.
    int numBins( void ) const;

protected:
    //! Converts an angle to a bin index.
    static int angleToBinIndex( double angle, int numBins, bool shouldIgnoreSign );

    //! Converts a bin index to an angle.
    static double binIndexToAngle( int binIndex, int numBins, bool shouldIgnoreSign );

private:
    std::vector< float > mBins;

    float mSampleSum;

    bool mShouldIgnoreSign;

    cv::Mat mSource;
};
