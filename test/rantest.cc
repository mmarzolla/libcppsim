/****************************************************************************
 *
 * rantest.cc
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

#include <iostream>
#include <vector>
#include "rng.hh"
#include "histogram.hh"

int main( int argc, char* argv[] )
{
    rng<double> *a, *b, *c, *d, *g;
    rng<int> *e, *f;
    histogram *ha, *hb, *hc, *hd, *he, *hf, *hg;
    vector<double> p( 4 );

    p[0] = 0.1;
    p[1] = 0.2;
    p[2] = 0.3;
    p[3] = 0.4;

    a = new rngExp( "NegExp", 10.0 );
    b = new rngUniform( "Uniform", 0.0, 1.0 );
    c = new rngNormal( "Normal", 0.0, 1.0 );
    g = new rngLTNormal( "LTnormal", 0.0, 1.0, 0.0 );
    d = new rngConst( "Constant", 5.0 );
    e = new rngDiscUniform( "Discrete_Uniform", 0, 10 );
    f = new rngDiscEmpirical( "Empirical", p );

    ha = new histogram( "NegExp", 0.0, 0.5, 20 );
    hb = new histogram( "Uniform", 0.0, 1.0, 20 );
    hc = new histogram( "Normal", -3.0, 3.0, 20 );
    hg = new histogram( "LTNormal", -3.0, 3.0, 20 );
    hd = new histogram( "Constant", 0.0, 10.0, 20 );
    he = new histogram( "Discrete_Uniform", 0.0, 10.0, 10 );
    hf = new histogram( "Empirical", 0.0, 10.0, 10 );

    for ( int i=0; i<50000; i++ ) {
        ha->update( a->value( ) );
        hb->update( b->value( ) );
        hc->update( c->value( ) );
        hd->update( d->value( ) );
        he->update( e->value( ) );
        hf->update( f->value( ) );
        hg->update( g->value( ) );
    }

    ha->report( );
    hb->report( );
    hc->report( );
    hd->report( );
    he->report( );
    hf->report( );
    hg->report( );

    return 0;
}
