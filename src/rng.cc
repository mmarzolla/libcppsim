/*****************************************************************************
 *
 * rng.cc
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

extern "C" {
#include "mrand.h"
}
#include <cmath>
#include <cassert>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "rng.hh"

//////////////////////////////////////////////////////////////////////////////
//
// rngUniform01
//
//////////////////////////////////////////////////////////////////////////////

rngUniform01::seed_t rngUniform01::_globalSeed = { 0.0, 0.0, 1.0, 0.0, 0.0, 1.0 };
const double rngUniform01::norm  = 2.328306549295728e-10;  /* 1.0/(m1+1) */
const double rngUniform01::norm2 = 2.328318825240738e-10;  /* 1.0/(m2+1) */
const double rngUniform01::m1 = 4294967087.0;
const double rngUniform01::m2 = 4294944443.0;
unsigned int rngUniform01::maxSeqLen = 1000;

double rngUniform01::draw( seed_t s )
{
    long k;
    double p,
           s10 = s[0], s11 = s[1], s12 = s[2],
           s20 = s[3], s21 = s[4], s22 = s[5];

    p = 1403580.0 * s11 - 810728.0 * s10;
    k = (long)(p / m1);  p -= k*m1;  if (p < 0.0) p += m1;
    s10 = s11;   s11 = s12;  s12 = p;

    p = 527612.0 * s22 - 1370589.0 * s20;
    k = (long)(p / m2);  p -= k*m2;  if (p < 0.0) p += m2;
    s20 = s21;   s21 = s22;  s22 = p; 

    s[0] = s10;  s[1] = s11;  s[2] = s12;
    s[3] = s20;  s[4] = s21;  s[5] = s22;

    if (s12 <= s22) 
        return ((s12 - s22 + m1) * norm);
    else 
        return ((s12 - s22) * norm);
}

//
// Advance the global seed space
//
void rngUniform01::advance( unsigned int count ) 
{
    while( count-- > 0 )
        draw( _globalSeed );
}

rngUniform01::rngUniform01( const string& name ) :
    rng_impl( name ),
    _seqLen( maxSeqLen )
{
    memcpy( _mySeed, _globalSeed, sizeof( _globalSeed ) );
    advance( maxSeqLen );
}

double rngUniform01::value( void )
{
    double result = draw( _mySeed );
    if ( 0 == _seqLen-- ) {
        _seqLen = maxSeqLen;
        memcpy( _mySeed, _globalSeed, sizeof( _globalSeed ) );
        advance( maxSeqLen );
    }
    return result;
}

void rngUniform01::reset( void )
{
    _numResets++;
    memcpy( _mySeed, _globalSeed, sizeof( _globalSeed ) );
    _seqLen = maxSeqLen;
    advance( maxSeqLen );
}

void rngUniform01::report( void ) const
{
    printf("RngUniform01\n"
           "============\n"
           "\t%15s / %8s / %8s\n" 
           "\t%15s / %8d / %8d\n",
           "Name", "Drawn", "Resets",
           name().c_str( ), _numDraws, numResets() );
}

//////////////////////////////////////////////////////////////////////////////
//
// Uniform distribution over [a,b]
//
//////////////////////////////////////////////////////////////////////////////

rngUniform::rngUniform( const string& name, double a, double b ) :
    rng_impl    ( name ),
    _a          ( a ),
    _b          ( b ),
    U1          ( "U1_"+name )
{
    assert( _b > _a );
}

double rngUniform::value( void )
{
    _numDraws++;
    return U1.value( )*(_b - _a) + _a;
}

void rngUniform::reset( void )
{
    _numResets++;
    U1.reset( );
}

void rngUniform::report( void ) const
{
    printf( "RngUniform\n"
            "==========\n",
            "\t%15s / %8s / %8s / %8s / %8s\n"
            "\t%15s / %8d / %8d / %8.2f / %8.2f\n\n",
            "Name", "Draws", "Resets", "a", "b",
            name().c_str( ), _numDraws, numResets(), _a, _b );
}

//////////////////////////////////////////////////////////////////////////////
//
// Discrete Uniform distribution over {a, a+1, ..., b-1, b}
//
//////////////////////////////////////////////////////////////////////////////

rngDiscUniform::rngDiscUniform( const string& name, int a, int b ) :
    rng_impl    ( name ),
    _a          ( a ),
    _b          ( b ),
    U1          ( "U1_"+name )
{
    assert( _b >= _a );
}

int rngDiscUniform::value( void )
{
    _numDraws++;
    const double _aa = (double)_a - 0.499999999;
    const double _bb = (double)_b + 0.499999999;
    int result = (int)( rint( U1.value( )*(_bb - _aa) + _aa ) );
    assert( result >= _a );
    assert( result <= _b );
    return result;
}

void rngDiscUniform::reset( void )
{
    _numResets++;
    U1.reset( );
}

void rngDiscUniform::report( void ) const
{
    printf( "RngDiscUniform\n"
            "==============\n"
            "\t%15s / %8s / %8s / %8s / %8s\n",
            "\t%15s / %8d / %8d / %8d / %8d\n",
            "Name", "Draws", "Resets", "a", "b",
            name().c_str( ), _numDraws, numResets(), _a, _b );
}

//////////////////////////////////////////////////////////////////////////////
//
// Discrete empirical distribution. Returns the integers {0, 1, ...n}
// with probability p[0], p[1], ... p[n]
//
//////////////////////////////////////////////////////////////////////////////
rngDiscEmpirical::rngDiscEmpirical( const string& name, const vector<double>& p ) :
    rng_impl    ( name ),
    _p          ( p ),
    U1          ( "U1_"+name )
{
    assert( _p.size( ) > 0 );
}

int rngDiscEmpirical::value( void )
{
    _numDraws++;
    const double tmp = U1.value( );
    double sum = 0.0;
    int i;
    for ( i=0; i<(int)_p.size( )-1; i++ ) {
        sum += _p[i];
        if ( tmp < sum ) return i;
    }
    return (int)i;
}

void rngDiscEmpirical::reset( void )
{
    _numResets++;
    U1.reset( );
}

void rngDiscEmpirical::report( void ) const
{
    printf( "RngDiscEmpirical\n"
            "================\n",
            "\t%15s / %8s / %8s / %8s / %8s\n"
            "\t%15s / %8d / %8d /",
            "Name", "Draws", "Resets", "P",
            name().c_str( ), _numDraws, numResets() );
    vector< double >::const_iterator it;
    for ( it = _p.begin( ); it != _p.end( ); it++ )
        cout << *it << " ";
    cout << endl;
}

//////////////////////////////////////////////////////////////////////////////
//
// Exponential Distribution
//
//////////////////////////////////////////////////////////////////////////////
rngExp::rngExp( const string& name, double mean ):
    rng_impl    ( name ),
    _mean       ( mean ),
    U1          ( "U1_"+name )
{
    assert( _mean > 0 );
}

double rngExp::value( void )
{
    _numDraws++;
    return (double)(-log( 1.0 - U1.value( ) ) * _mean );
}

void rngExp::reset( void )
{
    _numResets++;
    U1.reset( );
}

void rngExp::report( void ) const
{
    printf( "rngExp\n"
            "======\n",
            "\t%15s / %8s / %8s / %8s\n"
            "\t%15s / %8d / %8d / %8.2f\n",
            "Name", "Draws", "Resets", "Mean",
            name().c_str( ), _numDraws, numResets(), _mean );
}

//////////////////////////////////////////////////////////////////////////////
//
// Weibull distribution
//
//////////////////////////////////////////////////////////////////////////////
rngWeib::rngWeib( const string& name, double a, double b ) :
    rng_impl    ( name ),
    _a          ( a ),
    _b          ( b ),
    U1          ( "U1_"+name )
{
    assert( _a > 0 );
    assert( _b > 0 );
};

double rngWeib::value( void )
{
    return _a*pow( -log( 1.0 - U1.value() ), 1.0/_b );
};

void rngWeib::reset( void )
{
    _numResets++;
    U1.reset( );
}

void rngWeib::report( void ) const
{
    printf( "rngWeib\n",
            "=======\n"
            "\t%15s / %8s / %8s / %8s / %8s\n"
            "\t%15s / %8d / %8d / %8.2f / %8.2f\n",
            "Name", "Draws", "Resets", "a", "b",
            name().c_str( ), _numDraws, numResets(), _a, _b );
}

//////////////////////////////////////////////////////////////////////////////
//
// Gamma
//
//////////////////////////////////////////////////////////////////////////////
rngGam::rngGam( const string& name, double a, double b ) :
    rng_impl    ( name ),
    _a          ( a ),
    _b          ( b ),
    U1          ( "U1_"+name ),
    U2          ( "U1_"+name )
{
    assert( _a > 0 );
    assert( _b > 0 );
};

double rngGam::value( void )
{
    if ( _b < 1.0 ) {
        const double beta = (M_E+_b)/M_E;
        while( 1 ) {
            double U = U1.value( );
            double W = beta*U;
            double V = U2.value( );
            if ( W < 1.0 ) {
                double Y = pow( W, 1.0/_b );
                if ( V <= exp( -Y ) )
                    return _a*Y;
            } else {
                double Y = -log( (beta-W)/_b );
                if ( V <= pow(Y, _b-1.0) )
                    return _a*Y;
            }
        }
    } else if ( _b < 5.0 ) {
        while( 1 ) {        
            double U_1 = U1.value( );
            double U_2 = U2.value( );
            double V_1 = -log( U_1 );
            double V_2 = -log( U_2 );
            if ( V_2 > ( _b-1.0 )*( V_1-log( V_1 )-1.0 ) )
                return _a*V_1;
        }
    } else {
        double alpha = 1.0/sqrt( 2.0*_b - 1.0 );
        double beta = _b - log(4.0);
        double gamma = _b + 1.0/alpha;
        double delta = 1.0 + log( 4.5 );
        while( 1 ) {
            double U_1 = U1.value( );
            double U_2 = U2.value( );
            double V = alpha*log( U_1/(1.0-U_1) );
            double Y = _b*exp( V );
            double Z = U_1 * U_1 * U_2;
            double W = beta + gamma*V - Y;
            if ( W+delta-4.5*Z >= 0 )
                return _a*Y;
            else
                if ( W >= log( Z ) )
                    return _a*Y;
        }
    }
};

void rngGam::reset( void )
{
    _numResets++;
    U1.reset( );
    U2.reset( );
};

void rngGam::report( void ) const
{
    printf( "rngGam\n"
            "======\n"
            "\t%15s / %8s / %8s / %8s / %8s\n"
            "\t%15s / %8d / %8d / %8.2f / %8.2f\n",
            "Name", "Draws", "Resets", "a", "b",
            name().c_str( ), _numDraws, numResets(), _a, _b );
};

//////////////////////////////////////////////////////////////////////////////
//
// Erlang
//
//////////////////////////////////////////////////////////////////////////////
rngErl::rngErl( const string& name, double m, int k ) :
    rng_impl    ( name ),
    _m          ( m ),
    _k          ( k ),
    _rng        ( 0 )
{
    assert( m > 0 );
    assert( k > 0 );
    if ( _k < 10 )
        _rng = new rngUniform01( "U1_"+name );
    else
        _rng = new rngGam( "GAM_"+name, m/(double)k, k );
}

rngErl::~rngErl( )
{
    delete _rng;
}

double rngErl::value( void )
{
    if ( _k < 10 ) {
        double prod = 1.0;
        for ( int i=0; i<_k; i++ )
            prod *= ( 1.0 - _rng->value() );
        return -( _m/(double)_k )*log( prod );
    } else {
        return _rng->value();
    }
}

void rngErl::reset( void )
{
    _numResets++;
    _rng->reset( );
}

void rngErl::report( void ) const
{
    printf( "rngErl\n"
            "======\n"
            "\t%15s / %8s / %8s / %8s / %8s\n"
            "\t%15s / %8d / %8d / %8.2f / %10d\n",
            "Name", "Draws", "Resets", "m", "k",
            name().c_str( ), _numDraws, numResets(), _m, _k );
}

//////////////////////////////////////////////////////////////////////////////
//
// Constant distribution of mean m
//
//////////////////////////////////////////////////////////////////////////////

void rngConst::report( void ) const
{
    printf( "rngConst\n"
            "========\n"
            "\t%15s / %8s / %8s / %8s\n"
            "\t%15s / %8d / %8d / %8.2f\n",
            "Name", "Draws", "Resets", "Mean",
            name().c_str( ), _numDraws, numResets(), _mean );
}

//////////////////////////////////////////////////////////////////////////////
//
// Bernoulli distribution (returns true with probability p, false with pro-
// bability 1-p),
//
//////////////////////////////////////////////////////////////////////////////
rngBernoulli::rngBernoulli( const string& name, double p ):
    rng_impl    ( name ),
    _p          ( p ),
    U1          ( "U01_"+name )
{
    assert( _p >= 0.0 );
    assert( _p <= 1.0 );
}

bool rngBernoulli::value( void )
{
    _numDraws++;
    return ( U1.value( ) <= _p );
}

void rngBernoulli::reset( void )
{
    _numResets++;
    U1.reset( );
}

void rngBernoulli::report( void ) const
{
    printf( "rngBernoulli\n"
            "============\n"
            "\t%15s / %8s / %8s / %8s\n"
            "\t%15s / %8d / %8d / %8.2f\n",
            "Name", "Draws", "Resets", "P",
            name().c_str( ), _numDraws, numResets(), _p );
}

//////////////////////////////////////////////////////////////////////////////
//
// Normal distribution N( mu, sigmasq )
//
//////////////////////////////////////////////////////////////////////////////
rngNormal::rngNormal( const string& name, double mu, double sigmasq ):
    rng_impl    ( name ),
    _mu         ( mu ),
    _sigmasq    ( sigmasq ),
    U1          ( "U1_"+name ),
    U2          ( "U2_"+name )
{
    assert( _sigmasq > 0.0 );
    val[0] = val[1] = 0.0;
}

// Correction with respect to Banks. See Jain, p. 494
double rngNormal::value( void )
{
    if ( 0 == ( _numDraws % 2 ) ) {
	double R = sqrt( -2.0*log( U1.value( ) ) );
	double T = 2.0*M_PI*U2.value( );
	val[0] = _mu + sqrt( _sigmasq )*R*cos( T );
	val[1] = _mu + sqrt( _sigmasq )*R*sin( T );
    }
    _numDraws++;
    return val[ _numDraws % 2 ];
}

void rngNormal::reset( void )
{
    _numResets++;
    U1.reset( );
    U2.reset( );
}

void rngNormal::report( void ) const
{
    printf( "rngNormal\n"
            "=========\n"
            "\t%15s / %8s / %8s / %8s / %8s\n"
            "\t%15s / %8d / %8d / %8.2f / 8.2f\n",
            "Name", "Draws", "Resets", "Mu", "SigmaSQ",
            name().c_str( ), _numDraws, numResets(), _mu, _sigmasq );
}



//////////////////////////////////////////////////////////////////////////////
//
// Left truncated Normal distribution N( mu, sigmasq, l )
//
//////////////////////////////////////////////////////////////////////////////
rngLTNormal::rngLTNormal( const string& name, double mu, double sigmasq, double l ):
    rng_impl    ( name ),
    _mu         ( mu ),
    _sigmasq    ( sigmasq ),
    _l          ( l ),
    U1          ( "U1_"+name, mu, sigmasq )
{
    assert( _sigmasq > 0.0 );
}

double rngLTNormal::value( void )
{
    double result;
    do {
        result = U1.value( );
    } while ( result < _l );
    return result;
}

void rngLTNormal::reset( void )
{
    _numResets++;
    U1.reset( );
}

void rngLTNormal::report( void ) const
{
    printf( "rngLTNormal\n"
            "=========\n"
            "\t%15s / %8s / %8s / %8s / %8s / %8s\n"
            "\t%15s / %8d / %8d / %8.2f / 8.2f / %8.2f\n",
            "Name", "Draws", "Resets", "Mu", "SigmaSQ", "LTrunc",
            name().c_str( ), _numDraws, numResets(), _mu, _sigmasq, _l );
}



//////////////////////////////////////////////////////////////////////////////
//
// Right truncated Normal distribution N( mu, sigmasq, r )
//
//////////////////////////////////////////////////////////////////////////////
rngRTNormal::rngRTNormal( const string& name, double mu, double sigmasq, double r ):
    rng_impl    ( name ),
    _mu         ( mu ),
    _sigmasq    ( sigmasq ),
    _r          ( r ),
    U1          ( "U1_"+name, mu, sigmasq )
{
    assert( _sigmasq > 0.0 );
}

double rngRTNormal::value( void )
{
    double result;
    do {
        result = U1.value( );
    } while ( result > _r );
    return result;
}

void rngRTNormal::reset( void )
{
    _numResets++;
    U1.reset( );
}

void rngRTNormal::report( void ) const
{
    printf( "rngRTNormal\n"
            "=========\n"
            "\t%15s / %8s / %8s / %8s / %8s / %8s\n"
            "\t%15s / %8d / %8d / %8.2f / 8.2f / %8.2f\n",
            "Name", "Draws", "Resets", "Mu", "SigmaSQ", "RTrunc",
            name().c_str( ), _numDraws, numResets(), _mu, _sigmasq, _r );
}


//////////////////////////////////////////////////////////////////////////////
//
// Left and Right truncated Normal distribution N( mu, sigmasq, l, r )
//
//////////////////////////////////////////////////////////////////////////////
rngLRTNormal::rngLRTNormal( const string& name, double mu, double sigmasq, double l, double r ):
    rng_impl    ( name ),
    _mu         ( mu ),
    _sigmasq    ( sigmasq ),
    _l          ( l ),
    _r          ( r ),
    U1          ( "U1_"+name, mu, sigmasq )
{
    assert( _sigmasq > 0.0 );
}

double rngLRTNormal::value( void )
{
    double result;
    do {
        result = U1.value( );
    } while ( ( result < _l ) || ( result > _r ) );
    return result;
}

void rngLRTNormal::reset( void )
{
    _numResets++;
    U1.reset( );
}

void rngLRTNormal::report( void ) const
{
    printf( "rngRTNormal\n"
            "=========\n"
            "\t%15s / %8s / %8s / %8s / %8s / %8s / %8s\n"
            "\t%15s / %8d / %8d / %8.2f / 8.2f / %8.2f / %8.2f\n",
            "Name", "Draws", "Resets", "Mu", "SigmaSQ", "LTrunc", "RTrunc",
            name().c_str( ), _numDraws, numResets(), _mu, _sigmasq, _l, _r );
}

