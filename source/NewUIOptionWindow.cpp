// NewUIOptionWindow.cpp: implementation of the CNewUIOptionWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewUIOptionWindow.h"
#include "NewUISystem.h"
#include "ZzzTexture.h"
#include "DSPlaySound.h"
//#include "MoveWindow.h"


using namespace SEASON3B;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SEASON3B::CNewUIOptionWindow::CNewUIOptionWindow()
{
	m_pNewUIMng = NULL;
	m_Pos.x = 0;
	m_Pos.y = 0;

	m_bAutoAttack = true;
	m_bWhisperSound = false;
	m_bSlideHelp = true;
	m_iVolumeLevel = 0;
	m_iRenderLevel = 4;
	m_EffectSprite = false;
	m_EffectParticle = false;
	m_Terrain = false;
	m_Wings = false;
	m_Swords = false;
	m_Equipments = false;
	m_Object = false;
	m_Window = false;
}

SEASON3B::CNewUIOptionWindow::~CNewUIOptionWindow()
{
	Release();
}

bool SEASON3B::CNewUIOptionWindow::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_OPTION, this);
	//m_MoveWindow.Create(x, y, 312, 292);
	SetPos(x, y);
	LoadImages();
	SetButtonInfo();
	Show(false);
	return true;
}

void SEASON3B::CNewUIOptionWindow::SetButtonInfo()
{
	m_BtnClose.ChangeTextBackColor(RGBA(255, 255, 255, 0));
	m_BtnClose.ChangeButtonImgState(true, IMAGE_OPTION_BTN_CLOSE, false);
	m_BtnClose.ChangeButtonInfo(m_Pos.x + 84, m_Pos.y + 215, 54, 23);
	m_BtnClose.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	m_BtnClose.ChangeImgColor(BUTTON_STATE_DOWN, RGBA(255, 255, 255, 255));
}

void SEASON3B::CNewUIOptionWindow::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}


void SEASON3B::CNewUIOptionWindow::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

bool SEASON3B::CNewUIOptionWindow::UpdateMouseEvent()
{

	/*if (m_MoveWindow.UpdatePosition(m_Pos.x, m_Pos.y))
	{
		SetPos(m_MoveWindow.m_PosTemp.x + 40, m_MoveWindow.m_PosTemp.y);

		SetButtonInfo();

		return  false;
	}*/

	if (m_BtnClose.UpdateMouseEvent() == true)
	{
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_OPTION);
		return false;
	}

	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x - 65 + 150, m_Pos.y + 43, 15, 15))
	{
		m_bAutoAttack = !m_bAutoAttack;
	}
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x - 65 + 150, m_Pos.y + 65, 15, 15))
	{
		m_bWhisperSound = !m_bWhisperSound;
	}
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x - 65 + 150, m_Pos.y + 127, 15, 15))
	{
		m_bSlideHelp = !m_bSlideHelp;
	}

	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x - 65 + 300, m_Pos.y + 43, 15, 15))
	{
		m_EffectSprite = !m_EffectSprite;
	}

	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x - 65 + 300, m_Pos.y + 65, 15, 15))
	{
		m_EffectParticle = !m_EffectParticle;
	}
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x - 65 + 300, m_Pos.y + 87, 15, 15))
	{
		m_Terrain = !m_Terrain;
	}
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x - 65 + 300, m_Pos.y + 108, 15, 15))
	{
		m_Wings = !m_Wings;
	}
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x - 65 + 300, m_Pos.y + 130, 15, 15))
	{
		m_Swords = !m_Swords;
	}
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x - 65 + 300, m_Pos.y + 152, 15, 15))
	{
		m_Equipments = !m_Equipments;
	}
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x - 65 + 300, m_Pos.y + 174, 15, 15))
	{
		m_Object = !m_Object;
	}

	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x - 65 + 300, m_Pos.y + 196, 15, 15))
	{
		m_Window = !m_Window;

		LONG windowStyle = GetWindowLong(g_hWnd, GWL_STYLE);

		RECT rcWindow;
		GetWindowRect(g_hWnd, &rcWindow);

		RECT rcClient;
		GetClientRect(g_hWnd, &rcClient);

		if (m_Window)
		{
			windowStyle &= ~WS_CAPTION;
			windowStyle &= ~WS_THICKFRAME;

			AdjustWindowRect(&rcClient, WS_POPUP | WS_CLIPCHILDREN, FALSE);

			SetWindowPos(g_hWnd, NULL, rcWindow.left, rcWindow.top,
				rcClient.right - rcClient.left,
				rcClient.bottom - rcClient.top,
				SWP_NOZORDER | SWP_FRAMECHANGED);
		}
		else
		{
			windowStyle |= WS_CAPTION;
			windowStyle &= ~WS_THICKFRAME;

			AdjustWindowRect(&rcClient, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER | WS_CLIPCHILDREN, FALSE);

			SetWindowPos(g_hWnd, NULL, rcWindow.left, rcWindow.top,
				rcClient.right - rcClient.left,
				rcClient.bottom - rcClient.top,
				SWP_NOZORDER | SWP_FRAMECHANGED);
		}

		SetWindowLong(g_hWnd, GWL_STYLE, windowStyle);

		SetWindowPos(g_hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
	}

	if (CheckMouseIn(m_Pos.x - 65 + 33 - 8, m_Pos.y + 104, 124 + 8, 16))
	{
		int iOldValue = m_iVolumeLevel;
		if (MouseWheel > 0)
		{
			MouseWheel = 0;
			m_iVolumeLevel++;
			if (m_iVolumeLevel > 10)
			{
				m_iVolumeLevel = 10;
			}
		}
		else if (MouseWheel < 0)
		{
			MouseWheel = 0;
			m_iVolumeLevel--;
			if (m_iVolumeLevel < 0)
			{
				m_iVolumeLevel = 0;
			}
		}
		if (SEASON3B::IsRepeat(VK_LBUTTON))
		{
			int x = MouseX - (m_Pos.x - 65 + 33);
			if (x < 0)
			{
				m_iVolumeLevel = 0;
			}
			else
			{
				float fValue = (10.f * x) / 124.f;
				m_iVolumeLevel = (int)fValue + 1;
			}
		}

		if (iOldValue != m_iVolumeLevel)
		{
			SetEffectVolumeLevel(m_iVolumeLevel);
		}
	}
	if (CheckMouseIn(m_Pos.x - 65 + 25, m_Pos.y + 168, 141, 29))
	{
		if (SEASON3B::IsRepeat(VK_LBUTTON))
		{
			int x = MouseX - (m_Pos.x - 65 + 25);
			float fValue = (5.f * x) / 141.f;
			m_iRenderLevel = (int)fValue;
		}
	}

	if (CheckMouseIn(m_Pos.x - 65, m_Pos.y, 340, 292) == true)
	{
		return false;
	}

	return true;
}

bool SEASON3B::CNewUIOptionWindow::UpdateKeyEvent()
{
	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_OPTION) == true)
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_OPTION);
			PlayBuffer(SOUND_CLICK01);
			return false;
		}
	}

	return true;
}

bool SEASON3B::CNewUIOptionWindow::Update()
{
	return true;
}

bool SEASON3B::CNewUIOptionWindow::Render()
{
	EnableAlphaTest();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	RenderFrame();
	RenderContents();
	RenderButtons();
	DisableAlphaBlend();
	return true;
}

float SEASON3B::CNewUIOptionWindow::GetLayerDepth()	//. 10.5f
{
	return 10.5f;
}

float SEASON3B::CNewUIOptionWindow::GetKeyEventOrder()	// 10.f;
{
	return 10.0f;
}

void SEASON3B::CNewUIOptionWindow::OpenningProcess()
{

}

void SEASON3B::CNewUIOptionWindow::ClosingProcess()
{

}

void SEASON3B::CNewUIOptionWindow::LoadImages()
{
	LoadBitmap("Interface\\newui_button_close.tga", IMAGE_OPTION_BTN_CLOSE, GL_LINEAR);
	LoadBitmap("Interface\\newui_msgbox_back.jpg", IMAGE_OPTION_FRAME_BACK, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back03.tga", IMAGE_OPTION_FRAME_DOWN, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_top.tga", IMAGE_OPTION_FRAME_UP, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_back06(L).tga", IMAGE_OPTION_FRAME_LEFT, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_back06(R).tga", IMAGE_OPTION_FRAME_RIGHT, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_line.jpg", IMAGE_OPTION_LINE, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_point.tga", IMAGE_OPTION_POINT, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_check.tga", IMAGE_OPTION_BTN_CHECK, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_volume01.tga", IMAGE_OPTION_VOLUME_BACK, GL_LINEAR);
	LoadBitmap("Interface\\newui_option_volume02.tga", IMAGE_OPTION_VOLUME_COLOR, GL_LINEAR);
	LoadBitmap("Custom\\NewInterface\\option_effect03.tga", IMAGE_OPTION_EFFECT_BACK, GL_LINEAR);
	LoadBitmap("Custom\\NewInterface\\option_effect04.tga", IMAGE_OPTION_EFFECT_COLOR, GL_LINEAR);
}

void SEASON3B::CNewUIOptionWindow::UnloadImages()
{
	DeleteBitmap(IMAGE_OPTION_BTN_CLOSE);
	DeleteBitmap(IMAGE_OPTION_FRAME_BACK);
	DeleteBitmap(IMAGE_OPTION_FRAME_DOWN);
	DeleteBitmap(IMAGE_OPTION_FRAME_UP);
	DeleteBitmap(IMAGE_OPTION_FRAME_LEFT);
	DeleteBitmap(IMAGE_OPTION_FRAME_RIGHT);
	DeleteBitmap(IMAGE_OPTION_LINE);
	DeleteBitmap(IMAGE_OPTION_POINT);
	DeleteBitmap(IMAGE_OPTION_BTN_CHECK);
	DeleteBitmap(IMAGE_OPTION_EFFECT_BACK);
	DeleteBitmap(IMAGE_OPTION_EFFECT_COLOR);
	DeleteBitmap(IMAGE_OPTION_VOLUME_BACK);
	DeleteBitmap(IMAGE_OPTION_VOLUME_COLOR);
}

void SEASON3B::CNewUIOptionWindow::RenderFrame()
{
	float x, y;
	x = m_Pos.x;
	y = m_Pos.y;
	RenderImage(IMAGE_OPTION_FRAME_BACK, m_Pos.x - 65, m_Pos.y, 340, 249);
	RenderImage(IMAGE_OPTION_FRAME_UP, m_Pos.x - 65, m_Pos.y, 340, 292);
	y += 64.f;
	for (int i = 0; i < 14; ++i)
	{
		RenderImage(IMAGE_OPTION_FRAME_LEFT, m_Pos.x - 65, m_Pos.y, 340, 292);
		RenderImage(IMAGE_OPTION_FRAME_RIGHT, m_Pos.x - 65, m_Pos.y, 340, 292);
		y += 10.f;
	}
	RenderImage(IMAGE_OPTION_FRAME_DOWN, m_Pos.x - 65, m_Pos.y, 340, 250);

	y = m_Pos.y + 60.f;
	RenderImage(IMAGE_OPTION_LINE, x - 65 + 18, y, 154.f, 2.f);
	y += 22.f;
	RenderImage(IMAGE_OPTION_LINE, x - 65 + 18, y, 154.f, 2.f);
	y += 40.f;
	RenderImage(IMAGE_OPTION_LINE, x - 65 + 18, y, 154.f, 2.f);
	y += 22.f;
	RenderImage(IMAGE_OPTION_LINE, x - 65 + 18, y, 154.f, 2.f);

	y = m_Pos.y + 60.f;
	RenderImage(IMAGE_OPTION_LINE, x - 65 + 168, y, 154.f, 2.f);
	y += 22.f;
	RenderImage(IMAGE_OPTION_LINE, x - 65 + 168, y, 154.f, 2.f);
	y += 40.f;
	RenderImage(IMAGE_OPTION_LINE, x - 65 + 168, y, 154.f, 2.f);
	y += 22.f;
	RenderImage(IMAGE_OPTION_LINE, x - 65 + 168, y, 154.f, 2.f);
}

void SEASON3B::CNewUIOptionWindow::RenderContents()
{
	float x, y;
	x = m_Pos.x + 20.f;
	y = m_Pos.y + 46.f;
	RenderImage(IMAGE_OPTION_POINT, x - 65, y, 10.f, 10.f);
	y += 22.f;
	RenderImage(IMAGE_OPTION_POINT, x - 65, y, 10.f, 10.f);
	y += 22.f;
	RenderImage(IMAGE_OPTION_POINT, x - 65, y, 10.f, 10.f);
	y += 40.f;
	RenderImage(IMAGE_OPTION_POINT, x - 65, y, 10.f, 10.f);
	y += 22.f;
	RenderImage(IMAGE_OPTION_POINT, x - 65, y, 10.f, 10.f);

	x = m_Pos.x + 178;
	y = m_Pos.y + 46.f;
	RenderImage(IMAGE_OPTION_POINT, x - 65, y, 10.f, 10.f);
	y += 22.f;
	RenderImage(IMAGE_OPTION_POINT, x - 65, y, 10.f, 10.f);
	y += 22.f;
	RenderImage(IMAGE_OPTION_POINT, x - 65, y, 10.f, 10.f);

	RenderImage(IMAGE_OPTION_POINT, x - 65, y + 20, 10.f, 10.f);
	y += 40.f;
	RenderImage(IMAGE_OPTION_POINT, x - 65, y, 10.f, 10.f);
	y += 22.f;
	RenderImage(IMAGE_OPTION_POINT, x - 65, y, 10.f, 10.f);
	RenderImage(IMAGE_OPTION_POINT, x - 65, y + 24, 10.f, 10.f);

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetBgColor(0);
	g_pRenderText->RenderText(m_Pos.x - 65 + 40, m_Pos.y + 48, GlobalText[386]);
	g_pRenderText->RenderText(m_Pos.x - 65 + 40, m_Pos.y + 70, GlobalText[387]);
	g_pRenderText->RenderText(m_Pos.x - 65 + 40, m_Pos.y + 92, GlobalText[389]);
	g_pRenderText->RenderText(m_Pos.x - 65 + 40, m_Pos.y + 132, GlobalText[919]);
	g_pRenderText->RenderText(m_Pos.x - 65 + 40, m_Pos.y + 154, GlobalText[1840]);

}

void SEASON3B::CNewUIOptionWindow::RenderButtons()
{
	m_BtnClose.Render();

	if (m_bAutoAttack)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x - 65 + 150, m_Pos.y + 43, 15, 15, 0, 0);
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x - 65 + 150, m_Pos.y + 43, 15, 15, 0, 15.f);
	}

	if (m_bWhisperSound)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x - 65 + 150, m_Pos.y + 65, 15, 15, 0, 0);
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x - 65 + 150, m_Pos.y + 65, 15, 15, 0, 15.f);
	}

	if (m_bSlideHelp)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x - 65 + 150, m_Pos.y + 127, 15, 15, 0, 0);
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x - 65 + 150, m_Pos.y + 127, 15, 15, 0, 15.f);
	}


	if (m_EffectSprite)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x - 65 + 300, m_Pos.y + 43, 15, 15, 0, 0);
		g_pRenderText->RenderText(m_Pos.x - 65 + 200, m_Pos.y + 48, "Enabled Static Effect");
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x - 65 + 300, m_Pos.y + 43, 15, 15, 0, 15.f);
		g_pRenderText->RenderText(m_Pos.x - 65 + 200, m_Pos.y + 48, "Disabled Static Effect");
	}

	if (m_EffectParticle)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x - 65 + 300, m_Pos.y + 65, 15, 15, 0, 0);
		g_pRenderText->RenderText(m_Pos.x - 65 + 200, m_Pos.y + 70, "Enabled Dynamic Effect");
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x - 65 + 300, m_Pos.y + 65, 15, 15, 0, 15.f);
		g_pRenderText->RenderText(m_Pos.x - 65 + 200, m_Pos.y + 70, "Disabled Dynamic Effect");
	}
	if (m_Terrain)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x - 65 + 300, m_Pos.y + 87, 15, 15, 0, 0);
		g_pRenderText->RenderText(m_Pos.x - 65 + 200, m_Pos.y + 92, "Enabled Terrain");
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x - 65 + 300, m_Pos.y + 87, 15, 15, 0, 15.f);
		g_pRenderText->RenderText(m_Pos.x - 65 + 200, m_Pos.y + 92, "Disabled Terrain");
	}
	if (m_Wings)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x - 65 + 300, m_Pos.y + 108, 15, 15, 0, 0);
		g_pRenderText->RenderText(m_Pos.x - 65 + 200, m_Pos.y + 113, "Enabled Wings / Capes");
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x - 65 + 300, m_Pos.y + 108, 15, 15, 0, 15.f);
		g_pRenderText->RenderText(m_Pos.x - 65 + 200, m_Pos.y + 113, "Disabled Wings / Capes");
	}
	if (m_Swords)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x - 65 + 300, m_Pos.y + 130, 15, 15, 0, 0);
		g_pRenderText->RenderText(m_Pos.x - 65 + 200, m_Pos.y + 135, "Enabled Sword / Shields");
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x - 65 + 300, m_Pos.y + 130, 15, 15, 0, 15.f);
		g_pRenderText->RenderText(m_Pos.x - 65 + 200, m_Pos.y + 135, "Disabled Sword / Shields");
	}
	if (m_Equipments)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x - 65 + 300, m_Pos.y + 152, 15, 15, 0, 0);
		g_pRenderText->RenderText(m_Pos.x - 65 + 200, m_Pos.y + 156, "Enabled Equipments");
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x - 65 + 300, m_Pos.y + 152, 15, 15, 0, 15.f);
		g_pRenderText->RenderText(m_Pos.x - 65 + 200, m_Pos.y + 156, "Disabled Equipments");
	}

	if (m_Object)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x - 65 + 300, m_Pos.y + 174, 15, 15, 0, 0);
		g_pRenderText->RenderText(m_Pos.x - 65 + 200, m_Pos.y + 178, "Enabled Objects");
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x - 65 + 300, m_Pos.y + 174, 15, 15, 0, 15.f);
		g_pRenderText->RenderText(m_Pos.x - 65 + 200, m_Pos.y + 178, "Disabled Objects");
	}

	if (m_Window)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x - 65 + 300, m_Pos.y + 196, 15, 15, 0, 0);
		g_pRenderText->RenderText(m_Pos.x - 65 + 200, m_Pos.y + 200, "Enabled Window");
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x - 65 + 300, m_Pos.y + 196, 15, 15, 0, 15.f);
		g_pRenderText->RenderText(m_Pos.x - 65 + 200, m_Pos.y + 200, "Disabled Window");
	}


	RenderImage(IMAGE_OPTION_VOLUME_BACK, m_Pos.x - 65 + 33, m_Pos.y + 104, 124.f, 16.f);
	if (m_iVolumeLevel > 0)
	{
		RenderImage(IMAGE_OPTION_VOLUME_COLOR, m_Pos.x - 65 + 33, m_Pos.y + 104, 124.f * 0.1f * (m_iVolumeLevel), 16.f);
	}

	RenderImage(IMAGE_OPTION_EFFECT_BACK, m_Pos.x - 65 + 25, m_Pos.y + 168, 141.f, 29.f);
	if (m_iRenderLevel >= 0)
	{
		RenderImage(IMAGE_OPTION_EFFECT_COLOR, m_Pos.x - 65 + 25, m_Pos.y + 168, 141.f * 0.2f * (m_iRenderLevel + 1), 29.f);
	}

}

void SEASON3B::CNewUIOptionWindow::SetAutoAttack(bool bAuto)
{
	m_bAutoAttack = bAuto;
}

bool SEASON3B::CNewUIOptionWindow::IsAutoAttack()
{
	return m_bAutoAttack;
}

void SEASON3B::CNewUIOptionWindow::SetWhisperSound(bool bSound)
{
	m_bWhisperSound = bSound;
}

bool SEASON3B::CNewUIOptionWindow::IsWhisperSound()
{
	return m_bWhisperSound;
}

void SEASON3B::CNewUIOptionWindow::SetSlideHelp(bool bHelp)
{
	m_bSlideHelp = bHelp;
}

bool SEASON3B::CNewUIOptionWindow::IsSlideHelp()
{
	return m_bSlideHelp;
}

void SEASON3B::CNewUIOptionWindow::SetVolumeLevel(int iVolume)
{
	m_iVolumeLevel = iVolume;
}

int SEASON3B::CNewUIOptionWindow::GetVolumeLevel()
{
	return m_iVolumeLevel;
}

void SEASON3B::CNewUIOptionWindow::SetRenderLevel(int iRender)
{
	m_iRenderLevel = iRender;
}

int SEASON3B::CNewUIOptionWindow::GetRenderLevel()
{
	return m_iRenderLevel;
}
