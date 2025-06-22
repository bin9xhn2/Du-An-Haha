// NewUINameWindow.cpp: implementation of the CNewUINameWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewUINameWindow.h"
#include "ZzzBmd.h"
#include "ZzzObject.h"
#include "ZzzCharacter.h"
#include "ZzzInterface.h"
#include "ZzzInventory.h"
#include "UIControls.h"
#include "CSChaosCastle.h"
#include "PersonalShopTitleImp.h"
#include "MatchEvent.h"
#include "MapManager.h"
#include "Interfaces.h"
#include "HealthBar.h"

using namespace SEASON3B;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SEASON3B::CNewUINameWindow::CNewUINameWindow()
{
	m_pNewUIMng = NULL;
	m_Pos.x = m_Pos.y = 0;

	m_bShowItemName = false;
}

SEASON3B::CNewUINameWindow::~CNewUINameWindow()
{
	Release();
}

bool SEASON3B::CNewUINameWindow::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_NAME_WINDOW, this);

	SetPos(x, y);

	Show(true);

	return true;
}

void SEASON3B::CNewUINameWindow::Release()
{
	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj( this );
		m_pNewUIMng = NULL;
	}
}
		
void SEASON3B::CNewUINameWindow::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

bool SEASON3B::CNewUINameWindow::UpdateMouseEvent()
{
	return true;
}

bool SEASON3B::CNewUINameWindow::UpdateKeyEvent()
{
	if(SEASON3B::IsPress(VK_MENU) == true)
	{
		m_bShowItemName = !m_bShowItemName;
	}

	return true;
}

bool SEASON3B::CNewUINameWindow::Update()
{
	return true;
}

bool SEASON3B::CNewUINameWindow::Render()
{
	EnableAlphaTest();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	RenderName();
    RenderTimes();
    matchEvent::RenderMatchTimes ();
    RenderBooleans();
	DrawPersonalShopTitleImp();
	DisableAlphaBlend();

	if (SceneFlag == MAIN_SCENE)
	{
		gInterface.m_Lua.Generic_Call("BeforeMainProc", ">");
#if (HealthBar == 1)
		gHealthBar.DrawNewHealthBar(); //hpbar
#endif //HealthBar
	}
	return true;
}

void SEASON3B::CNewUINameWindow::RenderName()
{
	if(g_bGMObservation == true)
	{
		for(int i=0; i<MAX_CHARACTERS_CLIENT; i++)
		{
			CHARACTER *c = &CharactersClient[i];
			OBJECT *o = &c->Object;
			if(o->Live && o->Kind == KIND_PLAYER)
			{
				if(IsShopTitleVisible(c) == false)
				{
					CreateChat(c->ID,"",c);
				}
			}
		}
	}
	
#ifndef GUILD_WAR_EVENT
    if(gMapManager.InChaosCastle() == true)
    {
        if(FindText(Hero->ID, "webzen") == false)
        {
            if(SelectedNpc != -1 || SelectedCharacter != -1 )
			{
				return;
			}
        }
    }
#endif//GUILD_WAR_EVENT

	if(SelectedItem != -1 || SelectedNpc != -1 || SelectedCharacter != -1)
	{
		if(SelectedNpc != -1)
		{
			CHARACTER *c = &CharactersClient[SelectedNpc];
			OBJECT *o = &c->Object;
			CreateChat(c->ID,"",c);
		}
		else if(SelectedCharacter != -1)
		{
			CHARACTER *c = &CharactersClient[SelectedCharacter];

				OBJECT *o = &c->Object;
				if(o->Kind == KIND_MONSTER)
				{
					g_pRenderText->SetTextColor(255, 230, 200, 255);
					g_pRenderText->SetBgColor(100, 0, 0, 255);
					g_pRenderText->RenderText(setPosCenterX(0), 2, c->ID, 0 ,0, RT3_WRITE_CENTER);
				}
				else
#ifdef ASG_ADD_GENS_SYSTEM
#ifndef PBG_MOD_STRIFE_GENSMARKRENDER
				if (!::IsStrifeMap(World) || Hero->m_byGensInfluence == c->m_byGensInfluence)
#endif //PBG_MOD_STRIFE_GENSMARKRENDER
#endif	// ASG_ADD_GENS_SYSTEM
				{
					if(IsShopTitleVisible(c) == false)
					{
						CreateChat(c->ID,"",c);
					}
				}
		}
		else if(SelectedItem != -1)
		{
			RenderItemName(SelectedItem,&Items[SelectedItem].Object,Items[SelectedItem].Item.Level,Items[SelectedItem].Item.Option1,Items[SelectedItem].Item.ExtOption,false);
		}
	}

	if(m_bShowItemName || SEASON3B::IsRepeat(VK_MENU))
	{
		for(int i=0;i<MAX_ITEMS;i++)
		{
			OBJECT *o = &Items[i].Object;
			if(o->Live)
			{
				if(o->Visible && i!=SelectedItem)
				{
					RenderItemName(i,o,Items[i].Item.Level,Items[i].Item.Option1,Items[i].Item.ExtOption,true);
				}
			}
		}
	}	
#if ShowEXP
#if Toado		//Toado
	float TDX1 = GetPrivateProfileIntA("VIEW", "TDX1", 0, MENU_GAME_FILE);
	float TDY1 = GetPrivateProfileIntA("VIEW", "TDY1", 0, MENU_GAME_FILE);
	float TDX2 = GetPrivateProfileIntA("VIEW", "TDX2", 0, MENU_GAME_FILE);
	float TDY2 = GetPrivateProfileIntA("VIEW", "TDY2", 0, MENU_GAME_FILE);
	float TDX3 = GetPrivateProfileIntA("VIEW", "TDX3", 0, MENU_GAME_FILE);											  //check Vị trí qua mess.ini
	float TDY3 = GetPrivateProfileIntA("VIEW", "TDY3", 0, MENU_GAME_FILE);
	float TDX4 = GetPrivateProfileIntA("VIEW", "TDX4", 0, MENU_GAME_FILE);
	float TDY4 = GetPrivateProfileIntA("VIEW", "TDY4", 0, MENU_GAME_FILE);
	float TDX5 = GetPrivateProfileIntA("VIEW", "TDX5", 0, MENU_GAME_FILE);
	float TDY5 = GetPrivateProfileIntA("VIEW", "TDY5", 0, MENU_GAME_FILE);
#endif	//Toado
	if (CharacterMachine->PowerLevelUp)
	{
		auto current_time = std::chrono::steady_clock::now();
		double difTime = std::chrono::duration<double>(current_time - CharacterMachine->last_time).count();

		double time_checked = 1.5;

		if (difTime >= time_checked)
		{
			CharacterMachine->PowerLevelUp = false;
			CharacterMachine->last_time = current_time;
		}

		float Alpha = (float)(1.f - (difTime / time_checked));
		float MoveY = (float)(60.f * (1.f - (difTime / time_checked)));

		glColor4f(1.f, 1.f, 1.f, 1);

		vec3_t Position;
		int ScreenX = 0, ScreenY = 0;

		OBJECT* pObj = &Hero->Object;
		Vector(pObj->Position[0], pObj->Position[1], pObj->Position[2] + pObj->BoundingBoxMax[2] + 100.f, Position);

		Projection(Position, &ScreenX, &ScreenY);

		ScreenY = ScreenY + (Alpha * 50);

		RenderNumberHQ(ScreenX + 6.f, ScreenY - 8.f, CharacterMachine->ATTKPowerValue, 7.f, 10.f);
		RenderBitmap(BITMAP_TEXT_POWER, ScreenX - 5+TDX1-TDY1, ScreenY+TDX2-TDY2, 60.f, 28.f, 0.0, 0.0, 1.f, 118.f / 128.f, true, true, 0.0);
	}

}
#endif //ShowEXP
float SEASON3B::CNewUINameWindow::GetLayerDepth()
{
	return 1.0f;
}


