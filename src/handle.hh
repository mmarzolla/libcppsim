/*****************************************************************************
 *
 * handle.hh
 *
 * Definition of a handle class. A handle behaves almost like a pointer,
 * but keeps track of the number of handles referencing the same object.
 *
 * Copyright (C) 2002, 2012 Moreno Marzolla, Paolo Palmerini
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

#ifndef HANDLE_HH
#define HANDLE_HH

#include <cassert>
#include <map>
#include "logger.hh"

using namespace std;

template< class T > class handle;

//////////////////////////////////////////////////////////////////////////////
//
// class "shared". Each class T whose objects should be placed inside a
// handle<T> must inherit from this class.
//
// Inside a shared object there is an unsigned integer _sharedCount, which
// is the number of handles referencing that object. When _sharedCount
// equals zero, it is safe to destroy the object, as no handles are currently
// using it.
//
//////////////////////////////////////////////////////////////////////////////
class shared {
    template< class T > friend class handle;
public:
    virtual ~shared( ) 
    { 
        assert( 0 == _sharedCount );
        assert( _active_handle_count > 0 );
	logger* l = getLogger();
        l->message( 0, "%ld\n", --_active_handle_count );
    };

    static logger* getLogger( void ) { return logger::instance( "libcppsim::shared" ); };

protected:
    // Instances of this class cannot be created directly
    shared( ) : 
        _sharedCount( 0 ) 
    { 
	logger* l = getLogger();
        l->message( 0, "%ld\n", ++_active_handle_count );
    };

    // get_ref() increments the _sharedCount counter. Returns the updated
    // value. get_ref should be called whenever a handle binds to a shared
    // object
    unsigned int get_ref( void ) 
    { 
	return ++_sharedCount; 
    };

    // unget_ref() releases the shared object, decrementing _sharedCount.
    // The new value of _sharedCount is returned.
    unsigned int unget_ref( void ) 
    { 
	assert( _sharedCount > 0 ); 
	return --_sharedCount; 
    };

private:
    unsigned int                _sharedCount;
    static long unsigned int    _active_handle_count;
};

//////////////////////////////////////////////////////////////////////////////
//
// handle class.
//
//////////////////////////////////////////////////////////////////////////////
template< class T >
class handle {
public:
    handle( T* obj = 0 ) : _obj( obj ) 
    {
	if ( ! isNull( ) )
            _obj->get_ref( );
    };

    handle( const handle<T>& other ) : _obj ( other._obj )
    {
	if ( ! isNull( ) ) 
            _obj->get_ref( );
    };

    virtual ~handle( ) 
    {
        unBind( );
    };

    handle& operator=( const handle<T>& other ) 
    {
	unBind( );
	_obj = other._obj;
	if ( !isNull( ) ) 
	    other._obj->get_ref( );
	return *this;
    };

    T* operator->( ) const 
    {
	assert( ! isNull( ) );
	return _obj;
    };

    operator T* ( ) const { return rep( ); };

    T* rep( void ) const { return _obj; };

    friend bool operator==( const handle& lhs, const handle& rhs )
    {
	return lhs._obj == rhs._obj;
    };
    
    friend bool operator!=( const handle& lhs, const handle& rhs )
    {
	return lhs._obj != rhs._obj;
    };

    bool isNull( void ) const { return ( 0 == _obj ); };

    void setNull( void ) { unBind( ); };
    
    // Type conversion
    template< class S > operator handle<S>() 
    {
        return handle<S>( dynamic_cast<S*>( _obj ) );
    };

private:
    void unBind( void )
    {
        if ( !isNull( ) && ( 0 == _obj->unget_ref( ) ) )
            delete _obj;
	_obj = 0;
    };

#ifdef DO_NOT_USE
    unsigned int get_ref( void* obj )
    {
        return ++__refs[ obj ];
    };

    unsigned int unget_ref( void* obj )
    {
        assert( __refs[ obj ] >= 0 );
        unsigned int result = --__refs[ obj ];
        if ( 0 == result )
            __refs.erase( obj );
        return result;
    };

    static map< void*, unsigned int > __refs;
#endif
    T* _obj;
};

//////////////////////////////////////////////////////////////////////////////
//
// weak_handle class.
//
//////////////////////////////////////////////////////////////////////////////
template< class T >
class weak_handle {
public:
    weak_handle( T* obj = 0 ) : _obj( obj ) 
    {

    };

    weak_handle( const handle<T>& other ) : _obj ( other._obj )
    {

    };

    virtual ~weak_handle( ) 
    {

    };

    weak_handle& operator=( const weak_handle<T>& other ) 
    {
	_obj = other._obj;
	return *this;
    };

    T* operator->( ) const 
    {
	assert( ! isNull( ) );
	return _obj;
    };

    operator T* ( ) const { return rep( ); };

    T* rep( void ) const { return _obj; };

    friend bool operator==( const weak_handle& lhs, const weak_handle& rhs )
    {
	return lhs._obj == rhs._obj;
    };
    
    friend bool operator!=( const weak_handle& lhs, const weak_handle& rhs )
    {
	return lhs._obj != rhs._obj;
    };

    bool isNull( void ) const { return ( 0 == _obj ); };

    void setNull( void ) { _obj = 0; };
    
    // Type conversion
    template< class S > operator weak_handle<S>() 
    {
        return weak_handle<S>( dynamic_cast<S*>( _obj ) );
    };

private:

    T* _obj;
};

#endif
