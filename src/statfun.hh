/*****************************************************************************
 *
 * statfun.hh
 *
 * Some statistics functions
 *
 * Copyright (C) 2002, 2003 Moreno Marzolla
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 *****************************************************************************/

#ifndef _STATFUN_HH
#define _STATFUN_HH

double qnorm( double p );
double qt( double p, int ndf, bool lower_tail );
inline double squared( double x ) { return x*x; };

#endif
