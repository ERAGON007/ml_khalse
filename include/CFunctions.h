/*********************************************************
*
*  Multi Theft Auto: San Andreas - Deathmatch
*
*  ml_base, External lua add-on module
*
*  Copyright © 2003-2008 MTA.  All Rights Reserved.
*
*  Grand Theft Auto is © 2002-2003 Rockstar North
*
*  THE FOLLOWING SOURCES ARE PART OF THE MULTI THEFT
*  AUTO SOFTWARE DEVELOPMENT KIT AND ARE RELEASED AS
*  OPEN SOURCE FILES. THESE FILES MAY BE USED AS LONG
*  AS THE DEVELOPER AGREES TO THE LICENSE THAT IS
*  PROVIDED WITH THIS PACKAGE.
*
*********************************************************/
#pragma once

class CFunctions;

#include "include/ILuaModuleManager.h"
#include "lua.h"
extern ILuaModuleManager10 *pModuleManager;

class CFunctions
{
public:
    static int Mail_Connect(lua_State* vm);
    static int Mail_SendMail(lua_State* vm);
    static int Mail_Close(lua_State* L);
};

class Connection
{
public:
    int connection_id;
    Connection(int conn_id) : connection_id(conn_id) {}
};
