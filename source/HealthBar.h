#pragma once
#include <Windows.h>
#include "Protocol.h"


#define MAX_MAIN_VIEWPORT 400

struct PMSG_NEW_HEALTH_BAR_RECV
{
	PSWMSG_HEAD header; // C2:F3:E2
	BYTE count;
};

struct PMSG_NEW_HEALTH_RECV
{
	WORD index;
	BYTE type;
	BYTE rate;
	DWORD ViewHP[2];
};


struct NEW_HEALTH_BAR2
{
	WORD index;
	BYTE type;
	BYTE rate;
	DWORD ViewHP[2];
};


class CHealthBar {
public:
	CHealthBar();
	~CHealthBar();

	void ClearNewHealthBar();
	NEW_HEALTH_BAR2* GetNewHealthBar(WORD index, BYTE type);
	void DrawNewHealthBar();
	void InsertNewHealthBar(WORD index, BYTE type, BYTE rate, DWORD* ViewHP);


private:
	NEW_HEALTH_BAR2 gNewHealthBar[MAX_MAIN_VIEWPORT];
};

extern CHealthBar gHealthBar;

void GCNewHealthBarRecv(PMSG_NEW_HEALTH_BAR_RECV* lpMsg);