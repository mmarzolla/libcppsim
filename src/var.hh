/*****************************************************************************
 *
 * var.hh
 *
 * Definition of an abstract class for variables.
 *
 * Copyright (C) 2003 Moreno Marzolla
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

#ifndef VAR_HH
#define VAR_HH

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>

using namespace std;

//////////////////////////////////////////////////////////////////////////////
//
// Class var. 
//
// A Variable is an object which can be updated with objects of type T_in,
// and which can return objects of type T_out. Usually, T_in and T_out
// will both be of type double, or something similar. For statistics,
// T_in could be of type double, and T_out of type confInt (Confidence
// Interval)
//
//////////////////////////////////////////////////////////////////////////////

template< class T_in, class T_out >
class var {
public:
    //
    // Typedefs
    //
    typedef T_in var_input_t;
    typedef T_out var_output_t;
    //
    // Destructor
    //
    virtual ~var( ) { };
    //
    // Modifiers
    //
    virtual void update( T_in v ) = 0;
    virtual void reset( void ) = 0;
    virtual T_out value( void ) = 0;
    //
    // Accessors
    //
    virtual void report( void ) const = 0;
    const string& name( void ) const { return _name; };
    unsigned int numUpdates( void ) const { return _numUpdates; };
    unsigned int numResets( void ) const { return _numResets; };
protected:
    //
    // Constructor
    //
    var( const string& name ) : 
	_name           ( name ),
	_numUpdates     ( 0 ),
	_numResets      ( 0 )
    { };

    const string _name;
    unsigned int _numUpdates;
    unsigned int _numResets;
};

#endif
