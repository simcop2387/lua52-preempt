

/* foo.c */
#include <lua5.2/lua.h>
#include <lua5.2/lualib.h>
#include <lua5.2/lauxlib.h>

void l_yield(lua_State *L, lua_Debug *ar) {
  printf("Yielder called\n");
  lua_yield(L, 0);
}

static int l_sethook(lua_State *L) {
  printf("In hooksetter\n");
  lua_sethook(L, &l_yield, LUA_MASKCOUNT, 2);
  printf("Out of hooksetter\n");
  return 0;
}

lua_State *create_thread(lua_State *L, const char *scr)
{
  lua_State *ret;

  ret = lua_newthread(L);

  return ret;
}

int run_thread(lua_State *L, lua_State *t) {
  return lua_resume(t, L, 0);
}

int full_run(lua_State *L, lua_State *t) {
  int rc = run_thread(L, t);

  printf("rc = %d\n", rc);
  if (rc == 0) {
    printf("Finished?\n");
  } else if (rc == LUA_ERRRUN) {
    printf("Errored! %s", lua_tostring(L, -1));
  } else if (rc != LUA_YIELD) {
    printf("Errored!\n");
  } else {
    printf("Yielded!\n");
  }

  return rc;
}

int main(void)
{
  double z;

  lua_State *L = luaL_newstate();
  luaL_openlibs(L);

  luaL_dofile(L, "foo.lua");

  lua_State *t1 = lua_newthread(L);
  l_sethook(t1);

  lua_getglobal(t1,"main");	/* function we'll be calling */

  full_run(L, t1);
  full_run(L, t1);
  full_run(L, t1);
  full_run(L, t1);
	
  lua_close(L);
  return 0;
}
