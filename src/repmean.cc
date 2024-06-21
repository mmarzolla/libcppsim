/*****************************************************************************
 *
 * repmean.cc
 *
 * Estimation of the mean using independent replications, for terminating 
 * simulations.
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

#include <cassert>
#include <cmath>
#include <vector>
#include <functional>
#include <algorithm>
#include "repmean.hh"
#include "statfun.hh"
#include "trremoval.hh"

// 
// Use this class if you want to compute the mean of a terminating 
// (finite-horizon) simulation. Suppose you can collect a limited number of
// observations X_1, X_2, \ldots X_N. If they are correlated, clearly the usual
// algorithm for computing the mean and the confidence interval does not apply.
// Instead, you are required to run a number R of replications of the
// simulation. During run j you collect the observations X_j1, X_j2, \ldots
// X_jN. Considering the mean of run j, Xbar_j. Then Xbar_1, Xbar_2, \ldots
// Xbar_R are less biased than the X_i, and the mean and confidence interval
// for Xbar_i can be computed as usual.
//
// In this class we use two objects for computing the mean. _m_current contains
// the mean for the current run, _m_all contains the mean of the Xbar_j's
//
// Source: Banks, p. 234
//
repmean::repmean( const string& name, double confl, trremoval* t ) :
    statistic   ( name, confl ),
    _m_all      ( "repmean(" + name + ")::_m_all", confl ),
    _t          ( t ? 
                  t : 
                  new trremoval_const( "repmean(" + name + ")::_t", 0 ) )
{
    assert( 0 != _t );
}

repmean::~repmean( )
{
    delete _t;
}

void repmean::update( double v )
{
    _t->update( v );
    _numUpdates++;
}

void repmean::reset( )
{
    vector< double > _ob = _t->value( ); 
    _t->reset( );
    assert( ! _ob.empty() );
    vector< double >::const_iterator it;
    mean m_current( "repmean(" + name() + ")::reset()", confl() );

    /*
    for ( it = _ob.begin(); it != _ob.end(); it++ )
        m_current.update( *it );
    */
    for_each( _ob.begin(), _ob.end(), 
              bind1st( 
                      // mem_fun1( & mean::update) does not work anymore
                      // with gcc3 without kludges/warning
                      mem_fun1_t< void, mean, double >( & mean::update ), 
                      &m_current 
                      ) 
              );

    _m_all.update( m_current.m() );
    _numUpdates = 0;
    _numResets++;
}

void repmean::report( void ) const 
{
#ifdef NOT_USE
    confInt c = _m_all.value( );
    cout << " ConfInt (" << c.lBound( ) << "-" << c.uBound( ) << ")" << endl;
#endif
}

confInt repmean::value( void )
{
    double mean = _m_all.m();
    double hw = qt( 1.0-alpha()/2.0, _m_all.numUpdates()-1, true )*sqrt( _m_all.variance() / _m_all.numUpdates() );
    return confInt( mean - hw, mean + hw, confl() );
}
