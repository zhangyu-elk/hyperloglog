
/* debug宏 */
#ifdef DEBUG
	#define xprintf(fmt, ...)	printf("FILE:<%s>,FUN:<%s>,LINE:<%d>,"fmt"\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
	#define xprintf(fmt, ...)
#endif

/* assert宏, assert失败时打印表达式 */
#define xassert(X) 		if(!(X)){										\
								xprintf(" (%s) assert fail!", #X);		\
								assert(0);								\
							}
							

#include <lauxlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hyperloglog.h"

/* 针对lua5.1做的一些兼容处理, lua5.1取消luaL_register */
#if !defined LUA_VERSION_NUM || LUA_VERSION_NUM==501


static void luaL_setfuncs (lua_State *L, const luaL_Reg *l, int nup)
{
	luaL_checkstack(L, nup + 1, "too many upvalues");
	/* 调整栈的大小 */
	for (; l->name != NULL; l++)
	{  
		int i;
		lua_pushstring(L, l->name);
		for (i = 0; i < nup; i++)
		{			
			lua_pushvalue(L, -(nup + 1));
		}
		lua_pushcclosure(L, l->func, nup);
		lua_settable(L, -(nup + 3));
	}
	lua_pop(L, nup);  
}

#define lua_rawlen(L, i) lua_objlen(L, i)

#endif

#define MODNAME        "hll"
#define METANAME       MODNAME ".meta"

/*
* 销毁代码放在gc里面
*/
static int hll_new(lua_State *L)
{
	hllhdr **ptr = NULL;
	
	ptr = lua_newuserdata(L, sizeof(hllhdr*));
	luaL_getmetatable(L, METANAME);
	lua_setmetatable(L, -2);

	*ptr = createHLLObject();

	if(*ptr)
	{
		return 1;
	}
	return 0;
}

static int hll_gc(lua_State *L)
{
	hllhdr **ptr = NULL;
	if(!lua_isuserdata(L, 1))
	{
		return 0;
	}
	
	ptr = luaL_checkudata(L, 1, METANAME);
	deleteHLLObject(*ptr);
	
	return 0;
}

int hll_add(lua_State *L)
{
	int argc = lua_gettop(L);
	hllhdr **ptr = NULL;
	const unsigned char *key = NULL;
	size_t len = 0;
	int ret = 0;
	
	switch(argc)
	{
		case 3:
			{
				if(lua_isstring(L, 2) && (key = luaL_checkstring(L, 2)))
				{
					
				}
				else
				{
					ret = -1;
					break;
				}
			}
		case 2:
			{
				if(lua_isnumber(L, 3) && (len = luaL_checkinteger(L, 3)))
				{
					
				}
				else
				{
					ret = -1;
					break;
				}
			}
			break;
		default:
			ret = -1;
			break;
	}
	
	if(!ret)
	{
		if(!lua_isuserdata(L, 1) || (ptr = luaL_checkudata(L, 1, METANAME)) == NULL)
		{
			ret = -1;
		}
		else
		{
			ret = pfaddCommand(ptr, key, len);
		}
	}
	
	lua_pushinteger(L, ret);
	return 1;
}

int hll_count(lua_State *L)
{
	
	hllhdr **ptr = NULL;
	uint64_t  card = 0;
	int ret = 0;
	if(!lua_isuserdata(L, 1))
	{
		ret = -1;
	}
	else
	{
		ptr = luaL_checkudata(L, 1, METANAME);
		if(pfcountCommand(*ptr, &card))
		{
			ret = -1;
		}
	}
	
	if(ret)
	{
		lua_pushinteger(L, ret);
	}
	else
	{
		lua_pushinteger(L, card);
	}
	
	return 1;
}

int hll_merge(lua_State *L)
{
	int argc = lua_gettop(L);
	hllhdr **hdr = NULL;
	int ret = 0;
	
	if(!lua_isuserdata(L, 1) || (hdr = luaL_checkudata(L, 1, METANAME)) == NULL)
	{
		ret = -1;
	}
	
	if(!ret)
	{
		hllhdr **tmp = NULL;
		int i = 2;
		while(i < argc && lua_isuserdata(L, 2) && (tmp = luaL_checkudata(L, 2, METANAME)))
		{
			if(!hllMerge(hdr, *tmp))
			{
				ret++;
			}
			else
			{
				break;
			}
		}
	}
	
	lua_pushinteger(L, ret);
	return 1;
}

static const luaL_Reg hll[] = {
	{ "__gc", hll_gc},
	{ "new",  hll_new},
	//{ "delete", hll_delete },
	{ "add", hll_add },
	{ "count", hll_count },
	{ "merge", hll_merge },
	{ NULL, NULL },
};

int luaopen_hll(lua_State *L)
{
	luaL_newmetatable(L, METANAME);

	/* metatable.__index = metatable */
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");

	/* fill metatable */
	luaL_setfuncs(L, hll, 0);
	lua_pop(L, 1);

	lua_newtable(L);
	lua_pushvalue(L, -1);
	luaL_setfuncs(L, hll, 0);
	lua_setglobal(L, MODNAME);

	return 1;
}


