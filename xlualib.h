#pragma once

#include <xlib.h>

#include <lua.hpp>

#ifndef XLUALIB_INSIDE
#pragma comment(lib, "lua")
#endif

typedef void(*xlualib_register_routine)(lua_State* ls);

bool Add_XLUALIB_REGISTER_ROUTINE(const char* const name, xlualib_register_routine func);


#define ADD_XLUALIB_REGISTER(name) \
                      static void register_##name(lua_State* ls);\
                      static const bool breg = Add_XLUALIB_REGISTER_ROUTINE(#name, register_##name);\
                      static void register_##name(lua_State* ls)
