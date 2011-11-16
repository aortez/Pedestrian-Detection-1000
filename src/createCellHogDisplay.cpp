#include <CImg.h>
#include "createCellHogDisplay.h"
using namespace cimg_library;

CImg< double > createCellHogDisplay( const CImg< double >& cells )
{
    const int displayWidth = 20;
    const double lineRadius = displayWidth * 0.40;
    const int numBins = cells.depth();

    // compute the angle represented by each bin
    double binAngle[ numBins ];
    for ( int i = 0; i < numBins; i++ )
    {
        binAngle[ i ] = ( 2 * cimg::PI ) / ( numBins - 1 ) * i;
//        printf( "binAngle[%d] = %f\n", i, binAngle[ i ] );
    }

    CImg< double > hogImage( ( cells.width() + 1 ) * displayWidth, ( cells.height() + 1 ) * displayWidth, 1, 1, 0 );
    cimg_forXYZ( cells, x, y, z )
    {
        double start[ 2 ];
        start[ 0 ] = x * displayWidth + displayWidth / 2.0;
        start[ 1 ] = y * displayWidth + displayWidth / 2.0;
        hogImage( start[ 0 ], start[ 1 ] ) = 1.0;

        double end[ 2 ];
        const double angle = binAngle[ z ];
        const double magnitude = cells( x, y, z );
        end[ 0 ] = start[ 0 ] + cos( angle ) * magnitude * lineRadius;
        end[ 1 ] = start[ 1 ] + sin( angle ) * magnitude * lineRadius;
        double color = 1;

        hogImage.draw_line( start[ 0 ], start[ 1 ], end[ 0 ], end[ 1 ], &color );

    }
    hogImage.display();
    return hogImage;
}
