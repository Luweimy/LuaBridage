//
//  Lua.h
//  musegame
//
//  Created by Luwei on 15/5/19.
//  Copyright (c) 2015 MUSE-GAME. All rights reserved.
//

#ifndef __MUSEGAME__Lua__
#define __MUSEGAME__Lua__

#include <stdio.h>
#include <vector>
#include <string>
#include <assert.h>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}

namespace MUSE {

namespace Lua {
    using namespace std;
    
    /*
     * Stack
     * 对Lua堆栈的基本操作
     */
    class Stack {
    public:
        Stack(lua_State *pState) {
            assert(pState);
            m_pState = pState;
        }
        
        static const int TOP    = -1;
        static const int BOTTOM = 1;
        
        // Lua Basic Type
        enum Type {
            NONE       = LUA_TNONE,
            NIL        = LUA_TNIL,
            BOOLEAN    = LUA_TBOOLEAN,
            NUMBER     = LUA_TNUMBER,
            STRING     = LUA_TSTRING,
            TABLE      = LUA_TTABLE,
            FUNCTION   = LUA_TFUNCTION,
            USERDATA   = LUA_TUSERDATA,
            THREAD     = LUA_TTHREAD,
            LIGHTUSERDATA = LUA_TLIGHTUSERDATA
        };
        
        // Push Onto Stack
        
        void Push() {
            lua_pushnil(m_pState);
        }
        void Push(bool v) {
            lua_pushboolean(m_pState, v);
        }
        void Push(long v) {
            lua_pushinteger(m_pState, v);
        }
        void Push(int v) {
            lua_pushinteger(m_pState, v);
        }
        void Push(double v) {
            lua_pushnumber(m_pState, v);
        }
        void Push(float v) {
            lua_pushnumber(m_pState, v);
        }
        void Push(lua_CFunction v) {
            lua_pushcfunction(m_pState, v);
        }
        void Push(const std::string &s) {
            lua_pushstring(m_pState, s.c_str());
        }
        void Push(const char *s, size_t l) {
            lua_pushlstring(m_pState, s, l);
        }
        void Push(const char *fmt, ...) {
            va_list ap;
            va_start(ap, fmt);
            this->Push(fmt, ap);
            va_end(ap);
        }
        void Push(const char *fmt, va_list argp) {
            lua_pushvfstring(m_pState, fmt, argp);
        }
        
        // Peek Stack Index
        
        bool PeekBoolean(int i = Stack::TOP) {
            return lua_toboolean(m_pState, i) != 0;
        }
        long PeekInteger(int i = Stack::TOP) {
            return lua_tointeger(m_pState, i);
        }
        double PeekNumber(int i = Stack::TOP) {
            return lua_tonumber(m_pState, i);
        }
        std::string PeekString(int i = Stack::TOP) {
            return lua_tostring(m_pState, i);
        }
        const char* PeekLString(size_t *pLen/* Out */, int i = Stack::TOP) {
            return lua_tolstring(m_pState, i, pLen);
        }
        lua_CFunction PeekCFunction(int i = Stack::TOP) {
            return lua_tocfunction(m_pState, i);
        }
        // Converts the value at the given acceptable index to a generic C pointer (void*). The value may be a userdata,
        // a table, a thread, or a function; otherwise, peekPointer returns null. Different objects will give different
        // pointers. There is no way to convert the pointer back to its original value.
        // Typically this function is used only for debug information.
        const void* PeekPointer(int i = Stack::TOP) {
            return lua_topointer(m_pState, i);
        }
        
        // Pop Stack Top
        
        void Pop(int n = 1) {
            lua_pop(m_pState, n);
        }
        bool PopBoolean() {
            bool e = this->PeekBoolean(); this->Pop();
            return e;
        }
        long PopInteger() {
            long e = this->PeekInteger(); this->Pop();
            return e;
        }
        double popNumber() {
            double e = this->PeekNumber(); this->Pop();
            return e;
        }
        std::string PopString() {
            std::string s = this->PeekString(); this->Pop();
            return s;
        }
        lua_CFunction PopCFunction() {
            lua_CFunction p = this->PeekCFunction(); this->Pop();
            return p;
        }
        const void* PopPointer() {
            const void *p = this->PeekPointer(); this->Pop();
            return p;
        }
        
        
        // Element Type
        
        // Returns the type of the value in the given acceptable index, or LUA_TNONE for a non-valid index (that is, an index
        // to an "empty" stack position). The types returned by lua_type are coded by the following constants defined in lua.h:
        // LUA_TNIL, LUA_TNUMBER, LUA_TBOOLEAN, LUA_TSTRING, LUA_TTABLE, LUA_TFUNCTION, LUA_TUSERDATA, LUA_TTHREAD, and LUA_TLIGHTUSERDATA.
        int GetType(int i = Stack::TOP) {
            return lua_type(m_pState, i);
        }
        std::string GetTypeName(int i = Stack::TOP) {
            return this->TypeToName((this->GetType(i)));
        }
        // Returns the name of the type encoded by the value tp, which must be one the values returned by lua_type.
        std::string TypeToName(int type) {
            return lua_typename(m_pState, type);
        }
        
        
        // Global & Filed
        
        // Pops a value from the stack and sets it as the new value of global name in lua.
        void SetGlobal(const char *varname) {
            lua_setglobal(m_pState, varname);
        }
        // Pushes onto the stack the value of the global name in lua.
        void GetGlobal(const char *varname) {
            lua_getglobal(m_pState, varname);
        }
        
        // Does the equivalent to t[k] = v, where t is the value at the given valid index index and v is the value at the top of the stack,
        // This function pops the value from the stack. As in Lua, this function may trigger a metamethod for the "newindex" event
        void SetField(int i, const char *keyname) {
            lua_setfield(m_pState, i, keyname);
        }
        // Pushes onto the stack the value t[k], where t is the value at the given valid index index. As in Lua, this function may trigger
        // a metamethod for the "index" event
        void GetField(int i, const char *keyname) {
            lua_getfield(m_pState, i, keyname);
        }
        // Does the equivalent of t[n] = v, where t is the value at the given valid index index.
        // This function pops the value from the stack. The assignment is raw; that is, it does not invoke metamethods.
        void SetField(int i, const int key) {
            lua_rawseti(m_pState, i, key);
        }
        // Pushes onto the stack the value t[n], where t is the value at the given valid index index.
        // The access is raw; that is, it does not invoke metamethods.
        void GetField(int i, int key) {
            lua_rawgeti(m_pState, i, key);
        }
        // Does the equivalent to t[k] = v, where t is the value at the given valid index index, v is the value at the top of the stack,
        // and k is the value just below the top. This function pops both the key and the value from the stack.
        void SetField(int i, bool raw=false) {
            if (raw) lua_rawset(m_pState, i);
            else lua_settable(m_pState, i);
        }
        // Pushes onto the stack the value t[k], where t is the value at the given valid index index and k is the value at the top of the stack.
        // This function pops the key from the stack (putting the resulting value in its place).
        void GetField(int i, bool raw=false) {
            if (raw) lua_rawget(m_pState, i);
            else lua_gettable(m_pState, i);
        }
        
        // Stack Operation
        
        // Accepts any acceptable index, or 0, and sets the stack top to this index. If the new top is larger than the old one,
        // then the new elements are filled with nil. If index is 0, then all stack elements are removed.
        void SetTop(int i) {
            lua_settop(m_pState, i);
        }
        int GetTop() {
            return lua_gettop(m_pState);
        }
        // Returns the "length" of the value at the given acceptable index:
        // for strings, this is the string length;
        // for tables, this is the result of the length operator ('#');
        // for userdata, this is the size of the block of memory allocated for the userdata;
        // for other values, it is 0.
        size_t GetObjSize(int i = Stack::TOP) {
            return lua_objlen(m_pState, i);
        }
        // Clear the stack
        void Clear() {
            lua_settop(m_pState, 0);
        }
        // Pushes a copy of the element at the given valid index onto the stack
        void Copy(int i) {
            lua_pushvalue(m_pState, i);
        }
        // Removes the element at the given valid index, shifting down the elements above this index to fill the gap.
        // Cannot be called with a pseudo-index, because a pseudo-index is not an actual stack position.
        void Remove(int i) {
            lua_remove(m_pState, i);
        }
        // Moves the top element into the given valid index, shifting up the elements above this index to open space.
        // Cannot be called with a pseudo-index, because a pseudo-index is not an actual stack position.
        void Insert(int i) {
            lua_insert(m_pState, i);
        }
        // Moves the top element into the given position (and pops it), without shifting any element
        // (therefore replacing the value at the given position).
        void Replace(int i) {
            lua_replace(m_pState, i);
        }
        // Concatenates the n values at the top of the stack, pops them, and leaves the result at the top.
        // If n is 1, the result is the single string on the stack (that is, the function does nothing);
        // if n is 0, the result is the empty string. Concatenation is done following the usual semantics of Lua
        void Contact(int i) {
            lua_concat(m_pState, i);
        }
        
        
        // Load & Execute
        
        // Loads a string as a Lua chunk. This function uses lua_load to load the chunk in the zero-terminated string s.
        // If there are no errors, lua_load pushes the compiled chunk as a Lua function on top of the stack.
        // This function only loads a chunk; it does not run it.
        bool LoadString(const char *code) {
            // return 0 if no error
            return luaL_loadstring(m_pState, code) == 0;
        }
        // Loads and runs the given string.
        bool DoString(const char *code) {
            // return 0 if no error
            return luaL_dostring(m_pState, code) == 0;
        }
        // Calls a function in protected mode.
        // To call a function you must use the following protocol: first, the function to be called is
        //  pushed onto the stack; then, the arguments to the function are pushed in direct order; that is,
        //  the first argument is pushed first. Finally you call lua_call; nargs is the number of arguments
        //  that you pushed onto the stack. All arguments and the function value are popped from the stack
        //  when the function is called. The function results are pushed onto the stack when the function
        //  returns. The number of results is adjusted to nresults, unless nresults is LUA_MULTRET.
        //  In this case, all results from the function are pushed. Lua takes care that the returned values
        //  fit into the stack space.
        //  The function results are pushed onto the stack in direct order (the first result is pushed first),
        //  so that after the call the last result is on the top of the stack.
        // If there are no errors during the call, lua_pcall behaves exactly like lua_call. However, if there
        //  is any error, lua_pcall catches it, pushes a single value on the stack (the error message), and
        //  returns an error code. Like lua_call, lua_pcall always removes the function and its arguments from the stack.
        // If errfunc is 0, then the error message returned on the stack is exactly the original error message.
        //  Otherwise, errfunc is the stack index of an error handler function. (In the current implementation,
        //  this index cannot be a pseudo-index.) In case of runtime errors, this function will be called with
        //  the error message and its return value will be the message returned on the stack by lua_pcall.
        // Typically, the error handler function is used to add more debug information to the error message,
        //  such as a stack traceback. Such information cannot be gathered after the return of lua_pcall,
        //  since by then the stack has unwound.
        bool PCall(const int nargs = 0, const int nresults = 0, const int errfunc = 0) {
            return lua_pcall(m_pState, nargs, nresults, errfunc) == 0;
        }
        // Calls a function(not suggestion).
        void Call(const int nargs = 0, const int nresults = 0) {
            lua_call(m_pState, nargs, nresults);
        }
        
        // Error
        
        // Generates a Lua error. The error message (which can actually be a Lua value of any type) must be on the stack top
        void Error() {
            lua_error(m_pState);
        }
        void Error(const char *fmt, ...) {
            va_list ap;
            va_start(ap, fmt);
            luaL_error(m_pState, fmt, ap);
            va_end(ap);
        }
        
        // Lua State
        
        lua_State* GetState() {
            return m_pState;
        }
        void SetState(lua_State *pState) {
            assert(pState);
            m_pState = pState;
        }
        
    private:
        lua_State *m_pState;
    };
    
    /*
     * Table
     * 简便的构造Lua Table
     */
    class Table {
    public:
        Table(lua_State *pState):m_stack(pState) {
        }
        
        // Build New Table
        
        // Creates a new empty table and pushes it onto the stack. The new table has space pre-allocated for narr array elements and nrec
        // non-array elements. This pre-allocation is useful when you know exactly how many elements the table will have.
        void Build(int narr=0, int nrec=0) {
            lua_createtable(m_stack.GetState(), narr, nrec);
        }
        // After build the table will onto stack top
        template<typename T> void Build(const std::vector<T> &elems) {
            int len = (int)elems.size();
            lua_createtable(m_stack.GetState(), len, 0);
            for (int i = 0; i < len; i++) {
                m_stack.Push(elems[i]);
                m_stack.SetField(Stack::TOP - 1, i+1);
            }
        }
        
        // Set Key Value
        
        template<typename T1, typename T2> void Set(T1 k, T2 v) {
            assert(m_stack.GetType()==Stack::Type::TABLE);
            
            m_stack.Push(k);    /* ..., t, k */
            m_stack.Push(v);    /* ..., t, k, v */
            m_stack.SetField(Stack::TOP-2); /* ..., t */
        }
        
        size_t GetCount() {
            assert(m_stack.GetType()==Stack::Type::TABLE);
            return m_stack.GetObjSize();
        }
        Stack& GetStack() {
            return m_stack;
        }
    private:
        Stack m_stack;
    };
    
    /*
     * Buffer
     * A string buffer allows C code to build Lua strings piecemeal.
     * Usage:
     *  Lua::Buffer(L).Begin().Add("Hello").Add(',').Add("World").End();
     *  //Output: Hello,World
     */
    class Buffer {
    public:
        Buffer(lua_State *pState):m_stack(pState) {
        }
        
        // Initializes a buffer B. This function does not allocate any space
        Buffer& Begin() {
            luaL_buffinit(m_stack.GetState(), &m_buffer);
            return *this;
        }
        // Finishes the use of buffer B leaving the final string on the top of the stack.
        Buffer& End() {
            luaL_pushresult(&m_buffer);
            return *this;
        }
        
        // Adds the value at the top of the stack to the buffer B. Pops the value.
        Buffer& Add() {
            luaL_addvalue(&m_buffer);
            return *this;
        }
        Buffer& Add(char c) {
            luaL_putchar(&m_buffer, c);
            return *this;
        }
        Buffer& Add(const std::string &s) {
            luaL_addstring(&m_buffer, s.c_str());
            return *this;
        }
        Buffer& Add(const char *s, const size_t l) {
            luaL_addlstring(&m_buffer, s, l);
            return *this;
        }
        
    private:
        Stack       m_stack;
        luaL_Buffer m_buffer;
    };
    
} // Namespace-Lua
    
} // Namespace-Muse

#endif
