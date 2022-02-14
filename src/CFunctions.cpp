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

#include "CFunctions.h"
#include "khalse_pm_include.h"

int CFunctions::Mail_Connect(lua_State* L)
{
	// Make sure entered params are correct
	if (lua_isstring(L, 1) && lua_isstring(L, 2) && lua_isstring(L, 3))
	{
		const char* host = lua_tostring(L, 1);
		const char* username = lua_tostring(L, 2);
		const char* password = lua_tostring(L, 3);
		try {
			auto const connection_id = connect_smtp(host, username, password);
			Connection* conn = new Connection(connection_id);

			lua_pushlightuserdata(L, conn);
		} catch(std::exception& exc) {
			lua_pushboolean(L, false);
		}
	} else
	{
		lua_pushboolean(L, false);
	}

	return 1;
}

int CFunctions::Mail_SendMail(lua_State* L)
{
	if (lua_isuserdata(L, 1) && lua_isstring(L, 2) && lua_isstring(L, 3) && lua_isstring(L, 4)
		&& lua_isstring(L, 5)
		)
	{
		auto* connection = static_cast<Connection*>(lua_touserdata(L, 1));
		if (connection)
		{
			auto const sender = luaL_checkstring(L, 2);
			auto const target = luaL_checkstring(L, 3);
			auto const subject = luaL_checkstring(L, 4);
			auto const body = luaL_checkstring(L, 5);

			send_mail(connection->connection_id, sender, target, subject, body);
		}
	}

	return 0;
}


// TODO: Add a global variable g_SmtpConnections to hold a list of Connection
// And use it here to hanlde closing ....
int CFunctions::Mail_Close(lua_State* L)
{

	return 0;
}

