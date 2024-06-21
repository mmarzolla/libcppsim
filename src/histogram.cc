/*****************************************************************************
 *
 * histogram.cc
 *
 * Definition of histograms.
 *
 * Copyright (C) 2002,2003 Moreno Marzolla
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *****************************************************************************/

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cassert>
#include <cmath>
#include <algorithm>
#include "histogram.hh"

histogram::histogram( const string& name, double a, double b, int nbins ) :
    var_impl    ( name ),
    _a          ( a ),
    _b          ( b ),
    _nbins      ( nbins+2 ),
    _width      ( (_b-_a)/nbins ),
    _min        ( -1 ),
    _max        ( -1 ),
    _values     ( _nbins, 0 ),
    _sum        ( 0.0 ),
    _sumsq      ( 0.0 )
{
    
}
    
histogram::~histogram( )
{
    
}

void histogram::update( double v )
{
    int pos; // bin

    if ( v < _a )
	pos = 0;
    else if ( v > _b )
	pos = _nbins-1;
    else 
	pos = (int)(1 + ( (v-_a) / _width ));

    if ( _min == -1 || v < _min )
	_min = v;
    if ( _max == -1 || v > _max )
	_max = v;

    assert( pos < _nbins );
    assert( pos >= 0 );

    _values[ pos ]++;
    _sum += v;
    _sumsq += v*v;

    _numUpdates++;
}

void histogram::reset( void )
{
    _min = _max = -1;
    _sum = _sumsq = 0.0;
    // FIXME: I found a version of the STL library which does not implements
    // the assign() member of the vector class. Hence the assignment below
    // _values.assign( _nbins, 0 );
    fill( _values.begin(), _values.end(), 0 );
    _numUpdates = 0;
    _numResets++;
}

void histogram::report( void ) const
{
    const int hwidth = 35;
    double mean = _sum / numUpdates();
    double variance = 
        fabs( ( _sumsq - _sum * _sum / numUpdates() ) / 
              ( (double)( (double)numUpdates() - 1 ) ) );
    double sperc = 0.0;
    double pmax = 0.0;
    unsigned long ncount = 0;

    for ( int i=0; i<_nbins; i++ ) {
	if ( (double)_values[i] / numUpdates() > pmax )
	    pmax = (double)_values[i]/numUpdates();
    }

    printf( "%15s / %6s / %3s / %8s / %8s / %8s / %8s\n"
            "%15s / %6d / %3d / %8.2f / %8.2f / %8.2f / %8.2f\n\n",
            "Name", "Obs", "N", "min", "max", "mean", "std.dev",
            name().c_str( ), numUpdates(), _nbins, _min, _max, mean, sqrt(variance) );
    printf( "%3s/%10s/%6s/%6s/%-35s/%s\n",
            "bin", "low.bound", "bin% ", "cum% ", "   Histogram", "count" );

    for ( int i=0; i<_nbins; i++ ) {

	double perc = (double)_values[i] / numUpdates();
	int barlen = (int)(perc/pmax*hwidth);
	sperc += perc;

	if ( ncount < numUpdates() ) {
	    
	    cout << setw(3) << i << "/"
		 << setiosflags( ios::fixed );
	    
	    if ( i > 0 )
		cout << setw(10) << setprecision(2) << (double)(_a+_width*(i-1));
	    else
		cout << "    -INFTY";
	    
	    cout << "/"  
		 << setw(6) << setprecision(2) << perc*100
		 << "/"
		 << setw(6) << setprecision(2) << sperc*100
		 << "/";
	    
	    if ( 0 == barlen && _values[i] > 0 ) {
		cout << "." << string( hwidth-1, ' ' );
	    } else {
		cout << string( barlen, '*' ) 
		     << string( hwidth - barlen, ' ');
	    }
	    cout << "/" << _values[i] << endl;
	} else {
	    cout << "All remaining bins are 0" << endl;
	    break;
	}
	ncount += _values[i];
    }
    cout << endl;
}
