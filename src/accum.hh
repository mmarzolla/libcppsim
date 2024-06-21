/*****************************************************************************
 *
 * accum.hh
 *
 * Definition of an accumulator class. Accumulators collect the integral
 * of some time-dependent function.
 *
 * Copyright (C) 2002, 2003 Moreno Marzolla
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

#ifndef _CPPSIM_ACCUM_HH
#define _CPPSIM_ACCUM_HH

#include <utility>
#include <cassert>

#include "var.hh"


class accum : public var< const pair<double, double>&, double > {
public:
    typedef var< const pair<double, double> &, double > var_impl;

    accum( const string& name );
    virtual ~accum( );
    // Modifiers
    void update( const pair< double, double >& val_time );
    void reset( void );
    // Accessors
    void report( void ) const;
    double value( void ) { return _sum; };
    double lastUpdate( void ) const { return _lastUpdate; };
protected:
    double _sum;           // Sum of the values
    double _lastUpdate;    // Time of last update
    double _lastVal;       // Last value inserted
};

#endif
