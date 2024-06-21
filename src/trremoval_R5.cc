/****************************************************************************
 *
 * trremoval_R5.cc
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

#include "trremoval_R5.hh"
#include <cassert>

void trremoval_R5::update( const double& v )
{
    _ob.push_back( v );
    _mean.update( v );
    _numUpdates++;
}

void trremoval_R5::reset( void )
{
    _numUpdates = 0;
    _numResets++;
    _mean.reset( );
}

const vector<double>& trremoval_R5::value( void )
{
    static vector<double> result;
    result = _ob;
    double m = _mean.m( );
    unsigned int cnt = 0;
    vector<double>::iterator  it;
    assert( !result.empty() );
    for ( it = result.begin()+1; (it != result.end()) && (cnt < _k); it++ ) {
        if ( ( *it > m && *(it-1) < m ) || ( *it < m && *(it-1) > m ) )
            cnt++;
    }
    assert( it != result.end( ) );
    result.erase( result.begin(), it );
    return result;
}

void trremoval_R5::report( void ) const
{

}
