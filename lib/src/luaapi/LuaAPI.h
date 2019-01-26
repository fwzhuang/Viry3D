/*
* Viry3D
* Copyright 2014-2019 by Stack - stackos@qq.com
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#pragma once

#include "lua/lua.hpp"
#include "container/Map.h"
#include "string/String.h"

#define Top() lua_gettop(L)
#define Pop() lua_pop(L, 1)

#define IMPL_INDEX_FUNC() static int Index(lua_State* L) \
    { \
        const char* name = lua_tostring(L, 2); \
        const lua_CFunction* func = nullptr; \
        if (GetMethods().TryGet(name, &func)) \
        { \
            lua_pushcfunction(L, *func); \
            return 1; \
        } \
        return 0; \
    }

#define IMPL_NEW_FUNC(T) static int New(lua_State* L) \
    { \
        Ref<T>* ptr = new Ref<T>(new T()); \
        LuaAPI::PushPtr(L, { ptr, LuaClassType::T, LuaPtrType::Shared }); \
        return 1; \
    }

#define IMPL_GC_FUNC(T) static int GC(lua_State* L) \
    { \
        Ref<T>* ptr = (Ref<T>*) LuaAPI::GetPtr(L, 1, LuaClassType::T); \
        delete ptr; \
        return 0; \
    }

#define IMPL_GET_METHODS_FUNC() static Map<String, lua_CFunction>& GetMethods() \
    { \
        static Map<String, lua_CFunction> s_methods; \
        return s_methods; \
    }

namespace Viry3D
{
    enum class LuaClassType
    {
        Camera,
        CanvasRenderer,

        Count
    };

    enum class LuaPtrType
    {
        Raw,
        Shared,
    };

    struct LuaClassPtr
    {
        void* ptr;
        LuaClassType class_type;
        LuaPtrType ptr_type;
    };

    class LuaAPI
    {
    public:
        static void SetAll(lua_State* L);
        static void SetFunction(lua_State* L, const char* table, const char* name, lua_CFunction func);
        static void SetMetaTable(lua_State* L, const char* name, lua_CFunction index, lua_CFunction alloc, lua_CFunction gc);

        static const char* GetLuaClassName(LuaClassType type);
        static void PushPtr(lua_State* L, const LuaClassPtr& value);
        static void* GetPtr(lua_State* L, int index, LuaClassType class_type);
    };
}
