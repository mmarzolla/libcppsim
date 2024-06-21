/****************************************************************************
 *
 * quantiletest.cc
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

#include <cmath>
#include <iostream>
#include <iomanip>
#include "cppsim.hh"

class myfun {
public:
    virtual double value( double x ) = 0;
protected:
    myfun( ) { };
};

class uniformPDF : public myfun {
public:
    uniformPDF( ) { };
    double value( double x ) { return x; };
};

class expPDF : public myfun {
public:
    expPDF( double a ) : _a( a ) { };
    double value( double x ) { return 1.0 - exp( -_a*x ); };
protected:
    double _a;
};

class weiPDF : public myfun {
public:
    weiPDF( double a, double b ) : _a( a ), _b( b ) { };
    double value( double x ) { return 1.0 - exp( -pow( x/_a, _b ) ); };
protected:
    double _a;
    double _b;
};

double compute_quantile( myfun* f, double q )
{
    double a = 0.0;
    double b = 10.0;
    while( (b-a) > 1e-4 ) {
        double fa = f->value( a );
        double fab = f->value( (b+a)/2.0 );
        double fb = f->value( b );
        assert( fb > q );
        assert( fa < q );
        if ( fab > q ) 
            b = (b+a)/2.0;
        else
            a = (b+a)/2.0;
    };
    return (b+a)/2.0;
};

int main( void )
{
    int i,j;
    vector< rng<double>* > rngs;
    vector< myfun* >       pdfs;
    vector< quantile* >    quas;

    rngs.push_back( new rngUniform01( "Ran01" ) );
    rngs.push_back( new rngExp( "Exp", 2.0 ) );
    rngs.push_back( new rngWeib( "Weib", 1.0, 0.5 ) );

    pdfs.push_back( new uniformPDF() );
    pdfs.push_back( new expPDF( 2.0 ) );
    pdfs.push_back( new weiPDF( 1.0, 0.5 ) );

    quas.push_back( new quantile( "Quantile_u01", 0.9, 0.90 ) );
    quas.push_back( new quantile( "Quantile_ex", 0.9, 0.90 ) );
    quas.push_back( new quantile( "Quantile_we", 0.9, 0.90 ) );

    for ( i=0; i<1e6; i++ ) {
        for ( j=0; j<3; j++ ) {
            quas[j]->update( rngs[j]->value() );
        }
        if ( (i == 100) || (i == 500) || (i == 1000) || (i == 10000) || (i == 999999) ) {
            cout << "After " << i << " insertions:" << endl
                 << endl;
            for ( j=0; j<3; j++ ) {
                confInt ci = quas[j]->value();
                cout << "\t" << rngs[j]->name()
                     << "\t"
                     << setw(6) << setprecision(4 ) << ci.lBound() << "\t" << ci.uBound()
                     << "\t"
                     << compute_quantile( pdfs[j], 0.9 ) << endl;
            }
            cout << endl;
        }
    }

    return 0;
}
