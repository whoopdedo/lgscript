#include "lua.h"
#include "lauxlib.h"

#include <stdlib.h>

char progdir[MAX_PATH+1];

void setprogdir (lua_State *L) {
  luaL_gsub(L, lua_tostring(L, -1), LUA_EXECDIR, progdir);
  lua_remove(L, -2);  /* remove original string */
}
