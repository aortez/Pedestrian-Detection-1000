#pragma once
#include <cv.h>
#include <vector>

class Cell
{
public:
    //! Constructor.
    Cell( int numBins, bool shouldIgnoreSign );

    //! Adds a pixel to the cell.
    void addPixel( double gradientAngle, double weight );

    //! Get the number of votes in the specified bin.
    double bin( int binIndex ) const;

    //! Gets the angle represented by the specified bin.
    double binAngle( int binIndex ) const;

    //! Gets the normalized [ 0, 1 ] number of votes in the specified bin.
    double binNormalized( int binIndex ) const;

    //! Returns the number of bins in the cell.
    int numBins( void ) const;

    //! Converts a 2d vector into an angle.
    static double vecToAngle( const cv::Vec2d& vec, bool shouldIgnoreSign );

protected:
    //! Converts an angle to a bin index.
    static int angleToBinIndex( double angle, int numBins, bool shouldIgnoreSign );

    static double binIndexToAngle( int binIndex, int numBins, bool shouldIgnoreSign );

private:
    std::vector< double > mBins;

    double mSampleSum;

    bool mShouldIgnoreSign;
};
