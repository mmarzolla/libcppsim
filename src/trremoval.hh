/****************************************************************************
 *
 * trremoval.hh
 *
 * Transient Removal classes
 *
 * This file is part of libcppsim
 *
 * Copyright (C) 2003, 2004 Moreno Marzolla
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
#ifndef TRREMOVAL_HH
#define TRREMOVAL_HH

#include <vector>
#include <cassert>
#include <functional>
#include "var.hh"
#include "assertions.hh"

typedef var< double, const vector<double>& > trremoval;

class trremoval_const : public trremoval {
public:
    trremoval_const( const string& name, const size_t l=0 ) :
        trremoval( name ),
        _l( l ) 
    { 

    };
    virtual ~trremoval_const( ) 
    { 

    };
    // Modifiers
    void update( double v ) 
    {
        _numUpdates++;
        if ( numUpdates() > _l )
            _ob.push_back( v );
    };
    void reset( void ) 
    {
        _numUpdates = 0;
        _numResets++;
        _ob.clear();
    };
    const vector<double>& value( void ) throw( runtime_error )
    {
        return _ob;
    }
    // Accessors
    void report( void ) const
    {
        // Nothing yet
    }
protected:
    size_t _l;                  // length of the warm-up period
    vector<double> _ob;         // list of (truncated) observations
};

class trremoval_frac : public trremoval {
public:
    trremoval_frac( const string& name, float f ) :
        trremoval( name ),
        _f( f ) 
    { 
        assert( f > 0.0 );
        assert( f < 1.0 );
    };
    virtual ~trremoval_frac( ) 
    { 

    };
    // Modifiers
    void update( double v ) 
    {
        _numUpdates++;
        _ob.push_back( v );
    };
    void reset( void ) 
    {
        _numUpdates = 0;
        _numResets++;
        _ob.clear();
        _result.clear();
    };
    const vector<double>& value( void ) throw( runtime_error )
    {
        t_assert( _ob.size() > 1, runtime_error( "Need at least one observation" ) );
        _result = vector<double>( _ob.begin()+(unsigned int)(_ob.size()*_f), _ob.end() );
        return _result;
    }
    // Accessors
    void report( void ) const
    {
        // Nothing 
    }
protected:
    float _f;                   // Fraction of the observation list to remove
                                // e.g., 0.2 = drop first 20% of the list
    vector<double> _ob;         // list of observations
    vector<double> _result;     // truncated list of observations
};

//
// trremoval factory
// Class used to instantiate a new trremoval object
//
typedef struct unary_function< void, trremoval* > trremoval_abs_factory;

// Creates a new trremoval_const object
class trremoval_factory_const : public trremoval_abs_factory {
public:
    trremoval_factory_const( unsigned int l ) : _l( l ) { };
    trremoval* operator()( void ) {
        return new trremoval_const( "trremoval_factory_const", _l );
    };
protected:
    unsigned int _l;
};

// Creates a new trremoval_frac object
class trremoval_factory_frac : public trremoval_abs_factory {
public:
    trremoval_factory_frac( float f ) : _f( f ) { };
    trremoval* operator()( void ) {
        return new trremoval_frac( "trremoval_factory_frac", _f );
    };
protected:
    float _f;
};

#endif
