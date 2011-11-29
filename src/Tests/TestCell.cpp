#include <cv.h>
#include "Cell.h"
#include "UnitTest++.h"

SUITE( Cell_unit_tests )
{

//! Test class that allows tests access to protected methods.
struct CellFixture: public Cell
{
    //! Setup.
    CellFixture( void ) :
            Cell( 1, false )
    {
    }
};

TEST_FIXTURE( CellFixture, angleToBinIndex_ignoreSignFalse )
{
    const double angle[ 3 ] =
    { 0, CV_PI * 0.5, -CV_PI };

    const int expectedBin[ 3 ] =
    { 2, 3, 0 };

    const int numBins = 5;

    int computedBin[ 3 ];
    computedBin[ 0 ] = CellFixture::angleToBinIndex( angle[ 0 ], numBins, false );
    computedBin[ 1 ] = CellFixture::angleToBinIndex( angle[ 1 ], numBins, false );
    computedBin[ 2 ] = CellFixture::angleToBinIndex( angle[ 2 ], numBins, false );

    CHECK_EQUAL( expectedBin[ 0 ], computedBin[ 0 ] );
    CHECK_EQUAL( expectedBin[ 1 ], computedBin[ 1 ] );
    CHECK_EQUAL( expectedBin[ 2 ], computedBin[ 2 ] );
}

TEST_FIXTURE( CellFixture, angleToBinIndex_ignoreSignTrue )
{
    const double angle[ 3 ] =
    { 0, -CV_PI * .75, CV_PI * 0.25 };

    const int expectedBin[ 3 ] =
    { 0, 1, 1 };

    const int numBins = 5;

    int computedBin[ 3 ];
    computedBin[ 0 ] = CellFixture::angleToBinIndex( angle[ 0 ], numBins, true );
    computedBin[ 1 ] = CellFixture::angleToBinIndex( angle[ 1 ], numBins, true );
    computedBin[ 2 ] = CellFixture::angleToBinIndex( angle[ 2 ], numBins, true );

    CHECK_EQUAL( expectedBin[ 0 ], computedBin[ 0 ] );
    CHECK_EQUAL( expectedBin[ 1 ], computedBin[ 1 ] );
    CHECK_EQUAL( expectedBin[ 2 ], computedBin[ 2 ] );
}

TEST_FIXTURE( CellFixture, vecToAngle )
{

}


}
