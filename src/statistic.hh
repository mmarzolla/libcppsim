/*****************************************************************************
 *
 * statistic.hh
 *
 * Definition of an abstract class for statistics
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

#ifndef STATISTIC_HH
#define STATISTIC_HH

#include <string>
#include <cassert>
#include "var.hh"

//
// Confidence Interval
//
class confInt {
public:
    confInt( double lBound, double uBound, double confl ) :
	_lBound( lBound ),
	_uBound( uBound ),
	_confl( confl )
    {
	assert( lBound <= uBound );
	assert( 0 <= confl );
	assert( 1 >= confl );
    };

    // Accessors
    double width( void )  const { return _uBound - _lBound; };
    double lBound( void ) const { return _lBound;           };
    double uBound( void ) const { return _uBound;           };
    double confl( void )  const { return _confl;            };
    double val( void )    const { return ( _uBound + _lBound ) / 2.0; };

    // Other methods
    bool contains( double x ) const { 
        return ( ( _lBound <= x ) && ( x <= _uBound ) );
    };
    virtual ~confInt( ) { };
protected:
    double _lBound;
    double _uBound;
    double _confl;
};

class statistic : public var< double, confInt > {
public:
    // Typedefs
    typedef var< double, confInt > var_impl;

    virtual ~statistic( ) { };

    // Accessors (const)
    double confl( void ) const { return _confl; };
    double alpha( void ) const { return _alpha; };

protected:
    statistic( const string& name, double confl ):
	var_impl        ( name ),
	_confl          ( confl ),
        _alpha          ( 1.0 - confl )
    { 
	assert( confl > 0.0 );
	assert( confl < 1.0 );
    };
private:
    const double _confl;
    const double _alpha;
};

#endif
