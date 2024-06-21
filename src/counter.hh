/*****************************************************************************
 *
 * counter.hh
 *
 * Definition of a counter class.
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

#ifndef COUNTER_HH
#define COUNTER_HH

#include "var.hh"

class counter : public var<double, double> {
public:

    // Typedefs
    typedef var<double, double> var_impl;

    counter( const string& name );
    virtual ~counter( );

    // Modifiers
    void update( double v );
    void reset( void );
    // Accessors
    void report( void ) const;
    double value( void )        { return _sum; };
    double min( void ) const    { return _min; };
    double max( void ) const    { return _max; };
    double sum( void ) const    { return _sum; };
protected:
    double _min;        // Minimum of the values
    double _max;        // Maximum of the values
    double _sum;        // Sum of the values
};

#endif
