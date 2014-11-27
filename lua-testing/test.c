#include <stdio.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

int main (void) {
  char buff[256];
  int error, status;
  lua_State *L = luaL_newstate();/* opens Lua */
  luaL_openlibs(L);/* opens the standard libraries */

  status = luaL_loadfile(L, "script.lua"); 
  printf("%d\n", status);
  lua_close(L);
  return 0;
}

