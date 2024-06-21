/*****************************************************************************
 *
 * statfun.c
 *
 * Some statistics functions
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
#include <math.h>
}

/*
 * Compute the normal quantile. Code converted from the
 * Java version in http://www.stat.vt.edu/~sundar/java/code/qnorm_js.html
 * The following copyright appeared on the original code
 */

/* * @(#)qnorm.js * * Copyright (c) 2000 by Sundar Dorai-Raj
 * * @author Sundar Dorai-Raj
 * * Email: sdoraira@vt.edu
 * * This program is free software; you can redistribute it and/or
 * * modify it under the terms of the GNU General Public License 
 * * as published by the Free Software Foundation; either version 2 
 * * of the License, or (at your option) any later version, 
 * * provided that any use properly credits the author. 
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details at http://www.gnu.org * * */

double qnorm( double p ) {
    // ALGORITHM AS 111, APPL.STATIST., VOL.26, 118-121, 1977.
    // Computes z=invNorm(p)
    double split=0.42;
    double a0=  2.50662823884;
    double a1=-18.61500062529;
    double a2= 41.39119773534;
    double a3=-25.44106049637;
    double b1= -8.47351093090;
    double b2= 23.08336743743;
    double b3=-21.06224101826;
    double b4=  3.13082909833;
    double c0= -2.78718931138;
    double c1= -2.29796479134;
    double c2=  4.85014127135;
    double c3=  2.32121276858;
    double d1=  3.54388924762;
    double d2=  1.63706781897;
    double q = p-0.5;
    double r, ppnd;
    if( fabs(q)<=split ) {
        r = q*q;
        ppnd = q*(((a3*r+a2)*r+a1)*r+a0)/((((b4*r+b3)*r+b2)*r+b1)*r+1);
    } else {
        r = p;
        if ( q > 0 ) 
            r = 1-p;
        if ( r > 0 ) {
            r = sqrt(-log(r));
            ppnd = (((c3*r+c2)*r+c1)*r+c0)/((d2*r+d1)*r+1);
            if ( q < 0 ) 
                ppnd = -ppnd;
        } else {
            ppnd = 0;
        }
    }
    return(ppnd);
}

/*
 * Compute the quantile of a t-distribution. Code converted from the
 * Java version in http://www.stat.vt.edu/~sundar/java/code/qt_js.html
 * The following copyright appeared on the original code
 */

/* * @(#)qt.js * * Copyright (c) 2000 by Sundar Dorai-Raj
 * * @author Sundar Dorai-Raj
 * * Email: sdoraira@vt.edu
 * * This program is free software; you can redistribute it and/or
 * * modify it under the terms of the GNU General Public License 
 * * as published by the Free Software Foundation; either version 2 
 * * of the License, or (at your option) any later version, 
 * * provided that any use properly credits the author. 
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details at http://www.gnu.org * * */

double qt( double p, int ndf, bool lower_tail ) {
    // Algorithm 396: Student's t-quantiles by
    // G.W. Hill CACM 13(10), 619-620, October 1970
    const double eps = 1.0e-12;
#ifndef M_PI_2
    static const double M_PI_2 = 1.570796326794896619231321691640; // pi/2
#endif
    bool neg;
    double P, prob;
    double a, b, c, d, y, q;

    if ( p<=0 || p>=1 || ndf < 1 ) 
        return -1;
    if ( (lower_tail && p > 0.5) || 
         (!lower_tail && p < 0.5) ) {
        neg = false;
        P = 2 * (lower_tail ? (1 - p) : p);
    } else {
        neg = true;
        P = 2 * (lower_tail ? p : (1 - p));
    }
    
    if ( fabs(ndf - 2) < eps ) {   /* df ~= 2 */
        q = sqrt(2 / (P * (2 - P)) - 2);
    } else if ( ndf < 1 + eps ) {   /* df ~= 1 */
        prob = P * M_PI_2;
        q = cos(prob)/sin(prob);
    } else {      /*-- usual case;  including, e.g.,  df = 1.1 */
        a = 1 / (ndf - 0.5);
        b = 48 / (a * a);
        c = ((20700 * a / b - 98) * a - 16) * a + 96.36;
        d = ((94.5 / (b + c) - 3) / b + 1) * sqrt(a * M_PI_2) * ndf;
        y = pow(d * P, 2 / ndf);
        if ( y > 0.05 + a ) {
            /* Asymptotic inverse expansion about normal */
            double x = qnorm(0.5 * P);
            y = x * x;
            if (ndf < 5)
                c += 0.3 * (ndf - 4.5) * (x + 0.6);
            c = (((0.05 * d * x - 5) * x - 7) * x - 2) * x + b + c;
            y = (((((0.4 * y + 6.3) * y + 36) * y + 94.5) / c - y - 3) / b + 1) * x;
            y = a * y * y;
            if ( y > 0.002 ) {
                y = exp(y) - 1;
            } else { /* Taylor of    e^y -1 : */
                y = (0.5 * y + 1) * y;
            }
        } else {
            y = ((1 / (((ndf + 6) / (ndf * y) - 0.089 * d - 0.822)
                       * (ndf + 2) * 3) + 0.5 / (ndf + 4))
                 * y - 1) * (ndf + 1) / (ndf + 2) + 1 / y;
        }
        q = sqrt(ndf * y);
    }
    if ( neg ) 
        q = -q;
    return q;
}
