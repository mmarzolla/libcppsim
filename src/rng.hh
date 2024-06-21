/*****************************************************************************
 *
 * rng.hh
 *
 * Random Number Generators classes. 
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

#ifndef RNG_HH
#define RNG_HH

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include "var.hh"

template< class T >
class rng : public var< double, T > {
public:
    typedef var< double, T > var_impl;
    virtual ~rng( ) { };
protected:
    rng( const string& name ) :
        var_impl        ( name ),
        _numDraws       ( 0 ) 
    { }; 

    unsigned int _numDraws;
private:
    void update( double v ) // This method is disabled for rngs
    { 
        cerr << "FATAL: rng::update( ) called for " << var<double,T>::name() << endl;
        abort( );
    };
};

//////////////////////////////////////////////////////////////////////////////
//
// Uniform distribution in [0,1]
//
//////////////////////////////////////////////////////////////////////////////
class rngUniform01 : public rng<double> {
public:
    typedef rng<double> rng_impl;

    rngUniform01( const string& name );
    virtual ~rngUniform01( ) { };
    double value( void );
    void reset( void );
    void report( void ) const;

    static unsigned int maxSeqLen;

protected:

    typedef double seed_t[6];

    double draw( seed_t );
    void advance( unsigned int count );

    seed_t              _mySeed;
    unsigned int        _seqLen;

    // Static members
    static seed_t       _globalSeed;
    static const double norm;
    static const double norm2;
    static const double m1;
    static const double m2;
};

//////////////////////////////////////////////////////////////////////////////
//
// Uniform distribution in [a,b]
//
// Density function: f(x) = 1/(b-a) if a < x < b
//                        = 0       otherwise
//
//////////////////////////////////////////////////////////////////////////////
class rngUniform : public rng<double> {
public:
    typedef rng<double> rng_impl;

    rngUniform( const string& name, double a, double b );
    virtual ~rngUniform( ) { };
    double value( void );
    void reset( void );
    void report( void ) const;
protected:
    double      _a, _b;
    rngUniform01 U1;
};

//////////////////////////////////////////////////////////////////////////////
// 
// Weibull districution
//
// Density Function: f(x) = ba^(-b )x^(b-1)exp( -(x/a)^b ) if x > 0
//                          0                              otherwise
//
//////////////////////////////////////////////////////////////////////////////
class rngWeib : public rng<double> {
public:
    typedef rng<double> rng_impl;

    rngWeib( const string& name, double a, double b );
    virtual ~rngWeib( ) { };
    double value( void );
    void reset( void );
    void report( void ) const;
protected:
    double      _a, _b;
    rngUniform01 U1;
};

//////////////////////////////////////////////////////////////////////////////
//
// Constant distribution (always returns mean)
//
//////////////////////////////////////////////////////////////////////////////
class rngConst : public rng<double> {
public:
    typedef rng<double> rng_impl;

    rngConst( const string& name, double mean ) : 
	rng_impl( name ), 
        _mean( mean ) 
    { };
    virtual ~rngConst( ) { };
    double value( void ) { 
        _numDraws++; 
        return _mean; 
    };
    void reset( void ) { 
        _numUpdates = 0;
        _numResets++;
    };
    void report( void ) const;
protected:
    double      _mean;
};


//////////////////////////////////////////////////////////////////////////////
//
// Bernoulli
//
//////////////////////////////////////////////////////////////////////////////
class rngBernoulli : public rng<bool> {
public:
    typedef rng<bool> rng_impl;

    rngBernoulli( const string& name, double p );
    virtual ~rngBernoulli( ) { };
    bool value( void );
    void reset( void );
    void report( void ) const;
protected:
    double      _p;
    rngUniform01 U1;
};

//////////////////////////////////////////////////////////////////////////////
//
// Gamma
//
//////////////////////////////////////////////////////////////////////////////
class rngGam : public rng<double> {
public:
    typedef rng<double> rng_impl;

    rngGam( const string& name, double a, double b );
    virtual ~rngGam( ) { };
    double value( void );
    void reset( void );
    void report( void ) const;
protected:
    double      _a, _b;
    rngUniform01 U1;
    rngUniform01 U2;
};

//////////////////////////////////////////////////////////////////////////////
//
// k-Erlang
//
//////////////////////////////////////////////////////////////////////////////
class rngErl : public rng<double> {
public:
    typedef rng<double> rng_impl;

    rngErl( const string& name, double m, int k );
    virtual ~rngErl( );
    double value( void );
    void reset( void );
    void report( void ) const;
protected:
    double      _m;
    int         _k;
    rng<double> *_rng;
};

//////////////////////////////////////////////////////////////////////////////
//
// Normal
//
//////////////////////////////////////////////////////////////////////////////
class rngNormal : public rng<double> {
public:
    typedef rng<double> rng_impl;

    rngNormal( const string& name, double mu, double sigmasq );
    virtual ~rngNormal( ) { };
    double value( void );
    void reset( void );
    void report( void ) const;
protected:
    double      _mu;
    double      _sigmasq;
    rngUniform01 U1;
    rngUniform01 U2;
    double      val[2];
};


//////////////////////////////////////////////////////////////////////////////
//
// Left-truncated normal distribution
//
//////////////////////////////////////////////////////////////////////////////
class rngLTNormal : public rng<double> {
public:
    typedef rng<double> rng_impl;

    rngLTNormal( const string& name, double mu, double sigmasq, double l );
    virtual ~rngLTNormal( ) { };
    double value( void );
    void reset( void );
    void report( void ) const;
protected:
    double      _mu;
    double      _sigmasq;
    double      _l;
    rngNormal   U1;
};

//////////////////////////////////////////////////////////////////////////////
//
// Right-truncated normal distribution
//
//////////////////////////////////////////////////////////////////////////////
class rngRTNormal : public rng<double> {
public:
    typedef rng<double> rng_impl;

    rngRTNormal( const string& name, double mu, double sigmasq, double r );
    virtual ~rngRTNormal( ) { };
    double value( void );
    void reset( void );
    void report( void ) const;
protected:
    double      _mu;
    double      _sigmasq;
    double      _r;
    rngNormal   U1;
};


//////////////////////////////////////////////////////////////////////////////
//
// Left- and Right-truncated normal distribution
//
//////////////////////////////////////////////////////////////////////////////
class rngLRTNormal : public rng<double> {
public:
    typedef rng<double> rng_impl;

    rngLRTNormal( const string& name, double mu, double sigmasq, double l, double r );
    virtual ~rngLRTNormal( ) { };
    double value( void );
    void reset( void );
    void report( void ) const;
protected:
    double      _mu;
    double      _sigmasq;
    double      _l;
    double      _r;
    rngNormal   U1;
};

//////////////////////////////////////////////////////////////////////////////
//
// Discrete Uniform distribution in {a, a+1, ..., b-1, b}. Equivalent
// to the constant distribution if a==b.
//
//////////////////////////////////////////////////////////////////////////////
class rngDiscUniform : public rng<int> {
public:
    typedef rng<int> rng_impl;

    rngDiscUniform( const string& name, int a, int b );
    virtual ~rngDiscUniform( ) { };
    int value( void );
    void reset( void );
    void report( void ) const;
protected:
    int _a, _b;
    rngUniform01 U1;
};

//////////////////////////////////////////////////////////////////////////////
//
// Discrete Empirical Distribution
//
//////////////////////////////////////////////////////////////////////////////
class rngDiscEmpirical : public rng<int> {
public:
    typedef rng<int> rng_impl;

    rngDiscEmpirical( const string& name, const vector<double>& p );
    virtual ~rngDiscEmpirical( ) { };
    int value( void );
    void reset( void );
    void report( void ) const;
protected:
    vector<double> _p;
    rngUniform01 U1;
};

//////////////////////////////////////////////////////////////////////////////
//
// Exponential distribution
//
// Density function: f(x) = a exp( -ax ) if x > 0
//                        = 0            otherwise
//
//////////////////////////////////////////////////////////////////////////////
class rngExp : public rng<double> {
public:
    typedef rng<double> rng_impl;

    rngExp( const string& name, double mean );
    virtual ~rngExp( ) { };
    double value( void );
    void reset( void );
    void report( void ) const;
protected:
    double      _mean;
    rngUniform01 U1;
};

#endif
