/****************************************************************************
 *
 * welch.cc
 *
 * This file is part of cppsim
 *
 * Copyright (C) 2003 Moreno Marzolla
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 ****************************************************************************/

#include <cassert>
#include <iostream>
#include <fstream>
#include <functional>
#include <algorithm>
#include "welch.hh"
#include "statfun.hh"
#include "repmean.hh"

/*
 * Implementation of Welch's graphical method for removal of initialization
 * bias. Implementation based on Banks, p. 239.
 */
welch::welch( const string& name, 
              double confl,     // Confidence level
              unsigned int w    // half window size
              ):
    statistic   ( name, confl ),
    _ob         ( 1 ),
    _wsize      ( w )
{

}

welch::~welch( )
{

}

void welch::update( double v )
{
    _ob.back( ).push_back( v );
    _numUpdates++;
}

void welch::reset( void )
{
    // Start a new replication
    _ob.push_back( vector<double>( ) );
    _numUpdates = 0;
    _numResets++;
}

void welch::report( void ) const
{
    //
    // Banks, p. 238
    //
    unsigned int _k = numResets();      // Number of replications
    assert( _k >= 5 );                  // Suggested by Law and Kelton, p. 520
    unsigned int i, j, l, minSize;
    double mean = 0.0;
    string fname( "rep."+name( ) );     // The output file name
    ofstream os( fname.c_str( ) );

    if ( ! os.is_open( ) ) {
	cerr << "Cannot open file '" << fname << "' for writing" << endl;
        abort( );
    }

    //
    // minSize is the minimum size of all the vectors of observations
    //
    minSize = _ob[0].size( );
    for ( j=1; j<_k; j++ ) 
	minSize = ( minSize < _ob[j].size( ) ? minSize : _ob[j].size( ) );

    os << "# wsize   = " << _wsize << endl
       << "# minSize = " << minSize << endl << flush;

    assert( _wsize <= minSize / 4 );     // Suggested by Law and Kelton, p. 521
    assert( minSize > _wsize );
    vector< double > _x( minSize, 0.0 ); // _x is the vector of the means
    vector< double > xbarbar( minSize, 0.0 );

    //
    // Step 1: _x[j] = mean of the j-th observations
    //
    for ( i=0; i<_k; i++ ) 
        for ( j=0; j<minSize; j++ ) 
            _x[j] += _ob[i][j] / (double)_k;

    os << "set title 'Mean for " << name( ) 
       << " with " << _k 
       << " replications and wSize=" << _wsize 
       << "'" << endl
       << "set grid" << endl
       << endl
       << "plot '-' using ($1):($2) title '"
       << name( ) << "' with lines" << endl;    

    for ( j=0; j<minSize-_wsize; j++ ) {
	double wmean = 0.0;
        const int w = ( j < _wsize ? j : _wsize );
        for ( int m = -w; m <= w; m++ ) {
            assert( j >= (unsigned int)abs(m) );
            assert( j+m < minSize );
            wmean += _x[j+m] / ( 2.0*w + 1.0 );
        }
	os << j << " " << wmean << endl;
    }

    os << "e" << endl << flush;
    os.close( );
}

confInt welch::value( void )
{
    cerr << "Unsupported call to welch::value() for '"
         << name( ) << "'" << endl;
    abort( );
}

