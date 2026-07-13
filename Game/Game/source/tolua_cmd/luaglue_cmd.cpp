/*
** Lua binding: cmd
*/

#include "tolua.h"

#ifndef __cplusplus
#include <stdlib.h>
#endif
#ifdef __cplusplus
 extern "C" int tolua_bnd_takeownership (lua_State* L); // from tolua_map.c
#else
 int tolua_bnd_takeownership (lua_State* L); /* from tolua_map.c */
#endif
#include <string.h>

/* Exported function */
TOLUA_API int tolua_cmd_open (lua_State* tolua_S);
LUALIB_API int luaopen_cmd (lua_State* tolua_S);

#include "../lua_cmd.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
}

/* function: _LOG */
static int tolua_cmd__LOG00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isstring(tolua_S,1,0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const char* msg = ((const char*)  tolua_tostring(tolua_S,1,0));
 {
  _LOG(msg);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '_LOG'.",&tolua_err);
 return 0;
#endif
}

/* function: GetPad */
static int tolua_cmd_GetPad00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  int tolua_ret = (int)  GetPad();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPad'.",&tolua_err);
 return 0;
#endif
}

/* function: IsEnemyAlive */
static int tolua_cmd_IsEnemyAlive00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isnumber(tolua_S,1,0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  int enemyId = ((int)  tolua_tonumber(tolua_S,1,0));
 {
  bool tolua_ret = (bool)  IsEnemyAlive(enemyId);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsEnemyAlive'.",&tolua_err);
 return 0;
#endif
}

/* function: GetEnemyHP */
static int tolua_cmd_GetEnemyHP00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isnumber(tolua_S,1,0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  int index = ((int)  tolua_tonumber(tolua_S,1,0));
 {
  float tolua_ret = (float)  GetEnemyHP(index);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetEnemyHP'.",&tolua_err);
 return 0;
#endif
}

/* function: DrawEnemyStatus */
static int tolua_cmd_DrawEnemyStatus00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isnumber(tolua_S,1,0,&tolua_err) || 
 !tolua_isnumber(tolua_S,2,0,&tolua_err) || 
 !tolua_isnumber(tolua_S,3,0,&tolua_err) || 
 !tolua_isnumber(tolua_S,4,0,&tolua_err) || 
 !tolua_isnumber(tolua_S,5,0,&tolua_err) || 
 !tolua_isstring(tolua_S,6,0,&tolua_err) || 
 !tolua_isnumber(tolua_S,7,0,&tolua_err) || 
 !tolua_isnumber(tolua_S,8,0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  int x = ((int)  tolua_tonumber(tolua_S,1,0));
  int y = ((int)  tolua_tonumber(tolua_S,2,0));
  int r = ((int)  tolua_tonumber(tolua_S,3,0));
  int g = ((int)  tolua_tonumber(tolua_S,4,0));
  int b = ((int)  tolua_tonumber(tolua_S,5,0));
  const char* FormatString = ((const char*)  tolua_tostring(tolua_S,6,0));
  int id = ((int)  tolua_tonumber(tolua_S,7,0));
  double hp = ((double)  tolua_tonumber(tolua_S,8,0));
 {
  DrawEnemyStatus(x,y,r,g,b,FormatString,id,hp);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DrawEnemyStatus'.",&tolua_err);
 return 0;
#endif
}

/* function: DrawChara */
static int tolua_cmd_DrawChara00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isnumber(tolua_S,1,0,&tolua_err) || 
 !tolua_isnumber(tolua_S,2,0,&tolua_err) || 
 !tolua_isnumber(tolua_S,3,0,&tolua_err) || 
 !tolua_isnumber(tolua_S,4,0,&tolua_err) || 
 !tolua_isnumber(tolua_S,5,0,&tolua_err) || 
 !tolua_isstring(tolua_S,6,0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  int x = ((int)  tolua_tonumber(tolua_S,1,0));
  int y = ((int)  tolua_tonumber(tolua_S,2,0));
  int r = ((int)  tolua_tonumber(tolua_S,3,0));
  int g = ((int)  tolua_tonumber(tolua_S,4,0));
  int b = ((int)  tolua_tonumber(tolua_S,5,0));
  const char* FormatString = ((const char*)  tolua_tostring(tolua_S,6,0));
 {
  DrawChara(x,y,r,g,b,FormatString);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DrawChara'.",&tolua_err);
 return 0;
#endif
}

/* function: GetCharaClassName */
static int tolua_cmd_GetCharaClassName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isnumber(tolua_S,1,0,&tolua_err) || 
 !tolua_isnumber(tolua_S,2,0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  int charaType = ((int)  tolua_tonumber(tolua_S,1,0));
  int charaId = ((int)  tolua_tonumber(tolua_S,2,0));
 {
  const char* tolua_ret = (const char*)  GetCharaClassName(charaType,charaId);
 tolua_pushstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCharaClassName'.",&tolua_err);
 return 0;
#endif
}

/* function: SetCharaAttack */
static int tolua_cmd_SetCharaAttack00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isnumber(tolua_S,1,0,&tolua_err) || 
 !tolua_isnumber(tolua_S,2,0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  int charaType = ((int)  tolua_tonumber(tolua_S,1,0));
  int charaId = ((int)  tolua_tonumber(tolua_S,2,0));
 {
  SetCharaAttack(charaType,charaId);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetCharaAttack'.",&tolua_err);
 return 0;
#endif
}

/* function: GetCharaSpeed */
static int tolua_cmd_GetCharaSpeed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isnumber(tolua_S,1,0,&tolua_err) || 
 !tolua_isnumber(tolua_S,2,0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  int charaType = ((int)  tolua_tonumber(tolua_S,1,0));
  int charaId = ((int)  tolua_tonumber(tolua_S,2,0));
 {
  float tolua_ret = (float)  GetCharaSpeed(charaType,charaId);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCharaSpeed'.",&tolua_err);
 return 0;
#endif
}

/* Open lib function */
LUALIB_API int luaopen_cmd (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 tolua_constant(tolua_S,"ENEMYCOUNT",ENEMYCOUNT);
 tolua_function(tolua_S,"_LOG",tolua_cmd__LOG00);
 tolua_function(tolua_S,"GetPad",tolua_cmd_GetPad00);
 tolua_function(tolua_S,"IsEnemyAlive",tolua_cmd_IsEnemyAlive00);
 tolua_function(tolua_S,"GetEnemyHP",tolua_cmd_GetEnemyHP00);
 tolua_function(tolua_S,"DrawEnemyStatus",tolua_cmd_DrawEnemyStatus00);
 tolua_function(tolua_S,"DrawChara",tolua_cmd_DrawChara00);
 tolua_function(tolua_S,"GetCharaClassName",tolua_cmd_GetCharaClassName00);
 tolua_function(tolua_S,"SetCharaAttack",tolua_cmd_SetCharaAttack00);
 tolua_function(tolua_S,"GetCharaSpeed",tolua_cmd_GetCharaSpeed00);
 tolua_endmodule(tolua_S);
 return 1;
}
/* Open tolua function */
TOLUA_API int tolua_cmd_open (lua_State* tolua_S)
{
 lua_pushcfunction(tolua_S, luaopen_cmd);
 lua_pushstring(tolua_S, "cmd");
 lua_call(tolua_S, 1, 0);
 return 1;
}
