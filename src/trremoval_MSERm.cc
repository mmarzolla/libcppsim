/****************************************************************************
 *
 * trremoval_MSERm.cc
 *
 * This file is part of libcppsim
 *
 * Copyright (C) 2003, 2024 Moreno Marzolla
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ****************************************************************************/

#include "trremoval_MSERm.hh"

void trremoval_MSERm::update( const double& v )
{
    _ob.push_back( v );
    _numUpdates++;
}

void trremoval_MSERm::reset( void )
{
    _numUpdates = 0;
    _numResets++;
    _ob.clear();
}

const vector<double>& trremoval_MSERm::value( void )
{
    static vector<double> result;
    if ( _ob.size() < 1000 ) {
        result.clear(); // Too few observations
    }
    return result;
}

void trremoval_MSERm::report( void ) const
{

}
