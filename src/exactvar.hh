/*****************************************************************************
 *
 * exactvar.hh
 *
 * Definition of an abstract class for exact (ie, not stochastic) variables.
 *
 * Copyright (C) 2002,2003 Moreno Marzolla
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

#ifndef _CPPSIM_EXACTVAR_HH
#define _CPPSIM_EXACTVAR_HH

#include <string>
#include "var.hh"

class exactvar : public var<double, double> {
public:

    virtual ~exactvar( ) { };
    virtual double value( void ) const = 0;

protected:

    exactvar( const string& name ) : 
	var<double, double>( name )
    { };

};

#endif
