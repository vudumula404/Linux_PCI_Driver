// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <winioctl.h>
#include <conio.h>
#include <time.h>
#include "C:\FileSchedulerProject\FileSchedule\FileSchedule\Include\Libraries.h"
struct Msg
	{
		MSG_DEF	MsgDef;
		//MSG_RESULT MsgReturn;
		USHORT databuffer[32];
		char szMessageName[20];
		unsigned char ubyRxMsgCount;
		unsigned char ubyTxMsgCount;
	}m_stMsg[50], m_stMsgB2[50];
#define MINOR_CYCLE_ZERO		0
#define MINOR_CYCLE_ONE			1
#define MINOR_CYCLE_TWO			2
#define MINOR_CYCLE_THREE		3
#define MINOR_CYCLE_FOUR		4
#define MINOR_CYCLE_FIVE		5 
#define MINOR_CYCLE_SIX			6
#define MINOR_CYCLE_SEVEN		7