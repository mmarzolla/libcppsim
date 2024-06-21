/****************************************************************************
 *
 * resource.hh
 *
 * This file is part of cppsim
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

#ifndef _RESOURCE_HH_
#define _RESOURCE_HH_

#include "process.hh"
#include <list>

class resource {

    friend class process;

public:

    resource( unsigned int quantity = 1 ) : 
	_quantity( quantity ) { };

    virtual ~resource( ) { };

protected:

    unsigned int _quantity;
    list< handle<process> > _pending;

};

#endif
