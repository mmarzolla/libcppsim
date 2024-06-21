/****************************************************************************
 *
 * jain.cc
 *
 * This file is part of libcppsim
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

#include <fstream>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include "jain.hh"

jain::jain( const string& name, double confl ) :
    statistic   ( name, confl ),
    _ob         ( 1 )
{

}

jain::~jain( )
{

}

void jain::update( double v )
{
    _ob.back( ).push_back( v );
    _numUpdates++;
}

void jain::reset( void )
{
    _ob.push_back( vector<double>( ) );
    _numUpdates = 0;
    _numResets++;
}

void jain::report( void ) const
{
    //
    // Banks, p. 238
    //
    unsigned int _k = numResets();      // Number of replications
    assert( _k >= 5 );                  // Suggested by Law and Kelton, p. 520
    unsigned int i, j, l, minSize;
    string fname( "jain."+name( ) );     // The output file name
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

    //
    // Step 1: Xbar[j] = mean of the j-th observations
    //
    vector< double > Xbar( minSize, 0.0 );
    for ( i=0; i<_k; i++ ) 
        for ( j=0; j<minSize; j++ ) 
            Xbar[j] += _ob[i][j] / (double)_k;

    //
    // Step 2: compute the overall mean, Xbarbar
    //
    double Xbarbar = 0.0;
    for ( j=0; j<minSize; j++ )
        Xbarbar += Xbar[j] / (double)minSize;

    //
    // Step 3: compute the means after truncating the first l elements
    //
    vector<double> Xbarbar_l( minSize, 0.0 );
    for ( l=0; l<minSize-1; l++ ) {
        for ( j=l; j<minSize; j++ ) 
            Xbarbar_l[l] += Xbar[j] / (double)(minSize-l);
    }

    //
    // Step 4: Plot the truncated means
    //

    os << "set title 'Mean of the last n-l obsertations for " << name( ) 
       << " with " << _k 
       << " replications'" << endl
       << "set grid" << endl
       << "set xlabel 'l'" << endl
       << "set ylabel 'Xbarbar_l'" << endl
       << endl
       << "plot '-' using ($1):($2) title '"
       << name( ) << "' with lines" << endl;

    for ( j=0; j<minSize; j++ ) 
	os << j << " " << Xbarbar_l[j] << endl;
    os << "e" << endl << flush;

    os << "pause -1 'Press any key'" << endl;    

    //
    // Step 5: Plot the truncated means
    //
    os << "set title 'Relative change for " << name( ) 
       << " with " << _k 
       << " replications'" << endl
       << "set grid" << endl
       << "set xlabel 'l'" << endl
       << "set ylabel '(Xbarbar_l - Xbarbar)/Xbarbar'" << endl
       << endl
       << "plot '-' using ($1):($2) title '"
       << name( ) << "' with lines" << endl;    

    for ( j=0; j<minSize; j++ ) 
	os << j << " " << (Xbarbar_l[j]-Xbarbar)/Xbarbar << endl;
    os << "e" << endl << flush;

    os.close( );
}


confInt jain::value( void )
{
    cerr << "Unsupported call to jain::value() for '"
         << name( ) << "'" << endl;
    abort( );
}
