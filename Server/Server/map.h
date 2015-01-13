#ifndef MY_MAP_H
#define MY_MAP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define THEHOST "127.0.0.1"
#define THEPORT 2015
#define MAX_BUFF 8192

enum
{
	COMMAND_SIGN = 0x01,
	COMMAND_SHELL,
};


#endif