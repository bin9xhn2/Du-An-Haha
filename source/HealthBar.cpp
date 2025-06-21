#include "stdafx.h"
#include "HealthBar.h"
#include "Protocol.h"


CHealthBar gHealthBar;

CHealthBar::CHealthBar()
{
	ClearNewHealthBar();
}

CHealthBar::~CHealthBar()
{

}

void GCNewHealthBarRecv(PMSG_NEW_HEALTH_BAR_RECV* lpMsg) // OK
{
	gHealthBar.ClearNewHealthBar();

	for (int n = 0; n < lpMsg->count; n++)
	{
		PMSG_NEW_HEALTH_RECV* lpInfo = (PMSG_NEW_HEALTH_RECV*)(((BYTE*)lpMsg) + sizeof(PMSG_NEW_HEALTH_BAR_RECV) + (sizeof(PMSG_NEW_HEALTH_RECV) * n));

		gHealthBar.InsertNewHealthBar(lpInfo->index, lpInfo->type, lpInfo->rate, lpInfo->ViewHP);
	}

}

void CHealthBar::ClearNewHealthBar() // OK
{
	for (int n = 0; n < MAX_MAIN_VIEWPORT; n++)
	{
		gNewHealthBar[n].index = 0xFFFF;
		gNewHealthBar[n].type = 0;
		gNewHealthBar[n].rate = 0;
		gNewHealthBar[n].ViewHP[0] = 0;
		gNewHealthBar[n].ViewHP[1] = 0;
	}
}

void  CHealthBar::InsertNewHealthBar(WORD index, BYTE type, BYTE rate, DWORD* ViewHP) // OK
{
	for (int n = 0; n < MAX_MAIN_VIEWPORT; n++)
	{
		if (gNewHealthBar[n].index == 0xFFFF)
		{
			gNewHealthBar[n].index = index;
			gNewHealthBar[n].type = type;
			gNewHealthBar[n].rate = rate;
			gNewHealthBar[n].ViewHP[0] = ViewHP[0];
			gNewHealthBar[n].ViewHP[1] = ViewHP[1];

			return;
		}
	}
}

NEW_HEALTH_BAR2* CHealthBar::GetNewHealthBar(WORD index, BYTE type) // OK
{
	for (int n = 0; n < MAX_MAIN_VIEWPORT; n++)
	{
		if (gNewHealthBar[n].index != 0xFFFF)
		{
			if (gNewHealthBar[n].index == index && gNewHealthBar[n].type == type)
			{
				return &gNewHealthBar[n];
			}
		}
	}

	return 0;
}

void CHealthBar::DrawNewHealthBar()
{
	float Width = 85.f;
	char Text[100], Text2[100], Text3[100];
	vec3_t Angle;
	int ScreenX, ScreenY;

	static float LastHP[MAX_MAIN_VIEWPORT] = { 0 };
	static float DamageTimer[MAX_MAIN_VIEWPORT] = { 0 };

	auto FormatNumber = [](int num) -> std::string {
		char buffer[16];
		if (num >= 1000000000)      // 1 tỷ
			sprintf(buffer, "%.1fTỉ", num / 1000000000.0);
		else if (num >= 1000000)    // 1 triệu
			sprintf(buffer, "%.1ftr", num / 1000000.0);
		else if (num >= 1000)       // 1 ngàn
			sprintf(buffer, "%.1fK", num / 1000.0);
		else
			sprintf(buffer, "%d", num);
		return std::string(buffer);
		};

	for (int j = 0; j < MAX_MAIN_VIEWPORT; ++j)
	{
		auto ViewportAddress = &CharactersClient[j];
		if (!ViewportAddress || ViewportAddress->Object.Kind == 0)
			continue;

		NEW_HEALTH_BAR2* lpNewHealthBar = GetNewHealthBar(ViewportAddress->Key, ViewportAddress->Object.Kind);
		if (!lpNewHealthBar)
			continue;

		Vector(
			ViewportAddress->Object.Position[0],
			ViewportAddress->Object.Position[1],
			ViewportAddress->Object.Position[2] + ViewportAddress->Object.BoundingBoxMax[2] + 100.f,
			Angle);

		BeginOpengl(0, 0, GetWindowsX, GetWindowsY);
		Projection(Angle, &ScreenX, &ScreenY);
		EndOpengl();

		ScreenX -= (int)(Width / 2);

		bool drawBar = false;

		if (ViewportAddress->Object.Kind == 2) // Monster - luôn hiện
		{
			drawBar = true;
		}
		else if (ViewportAddress->Object.Kind == 1) // Player - cần Ctrl + hover chuột
		{
			if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) &&
				CheckMouseIn(ScreenX - 20, ScreenY, 40, 120))
			{
				drawBar = true;
			}
		}

		if (!drawBar)
			continue;

		// --- Vẽ khung text ---
		std::string hpCurrent = FormatNumber(lpNewHealthBar->ViewHP[0]);
		std::string hpMax = FormatNumber(lpNewHealthBar->ViewHP[1]);
		sprintf(Text, "%s / %s (%d%%)", hpCurrent.c_str(), hpMax.c_str(), lpNewHealthBar->rate);

		g_pRenderText->SetFont(g_hFont);
		g_pRenderText->SetTextColor(255, 255, 255, 255);
		g_pRenderText->SetBgColor(0, 0, 0, 128);
		g_pRenderText->RenderText(ScreenX + (Width / 2), ScreenY - 8, Text, Width, 0, 8);

		sprintf(Text2, "%s", ViewportAddress->ID);
		g_pRenderText->RenderText(ScreenX + (Width / 2), ScreenY - 18, Text2, Width, 0, 8);

		// --- Vẽ khung thanh máu ---
		EnableAlphaTest();
		glColor4f(0.2f, 0.2f, 0.2f, 0.8f);
		RenderColor((float)(ScreenX), (float)(ScreenY + 2), Width, 6.f);

		// Phần trăm máu
		float HP_Percentage = (float)lpNewHealthBar->ViewHP[0] / (float)lpNewHealthBar->ViewHP[1];
		float HP_Width = Width * HP_Percentage;

		// Thanh máu tụ
		if (lpNewHealthBar->ViewHP[0] < LastHP[j])
			DamageTimer[j] = 2.0f;

		if (LastHP[j] > lpNewHealthBar->ViewHP[0])
			LastHP[j] -= ((LastHP[j] - lpNewHealthBar->ViewHP[0]) * 0.2f);
		else
			LastHP[j] = lpNewHealthBar->ViewHP[0];

		float LastHP_Percentage = (float)LastHP[j] / (float)lpNewHealthBar->ViewHP[1];
		float LastHP_Width = min(Width, Width * LastHP_Percentage);

		if (DamageTimer[j] > 0) {
			glColor4f(0.8f, 0.0f, 0.0f, DamageTimer[j] * 0.66f);
			RenderColor((float)(ScreenX + HP_Width), (float)(ScreenY + 3), LastHP_Width - HP_Width, 4.f);
			DamageTimer[j] -= 0.008f;
		}

		// Thanh máu chính
		float Red = 1.0f - HP_Percentage;
		float Green = HP_Percentage;
		glColor4f(Red, Green, 0.f, 1.f);
		RenderColor((float)(ScreenX + 1), (float)(ScreenY + 3), HP_Width - 2, 4.f);

		DisableAlphaBlend();
	}

	DisableAlphaBlend();
	glColor3f(1.f, 1.f, 1.f);
}

std::string FormatNumber(int num)
{
	char buffer[16];
	if (num >= 1000000000)
		sprintf(buffer, "%.1fTỉ", num / 1000000000.0);
	else if (num >= 1000000)
		sprintf(buffer, "%.1ftr", num / 1000000.0);
	else if (num >= 1000)
		sprintf(buffer, "%.1fK", num / 1000.0);
	else
		sprintf(buffer, "%d", num);
	return std::string(buffer);
}

//void CHealthBar::DrawNewHealthBar()
//{
//	float Width = 85.f;
//	char Text[100], Text2[100], Text3[100];
//	vec3_t Angle;
//	int ScreenX, ScreenY;
//
//	static float LastHP[MAX_MAIN_VIEWPORT] = { 0 };
//	static float DamageTimer[MAX_MAIN_VIEWPORT] = { 0 };
//
//	for (int j = 0; j < MAX_MAIN_VIEWPORT; ++j)
//	{
//		auto ViewportAddress = &CharactersClient[j];
//
//		if (!ViewportAddress) continue;
//
//		NEW_HEALTH_BAR2* lpNewHealthBar = GetNewHealthBar(ViewportAddress->Key, ViewportAddress->Object.Kind);
//		if (lpNewHealthBar == 0) continue;
//
//		Vector(
//			ViewportAddress->Object.Position[0],
//			ViewportAddress->Object.Position[1],
//			ViewportAddress->Object.Position[2] + ViewportAddress->Object.BoundingBoxMax[2] + 100.f,
//
//			Angle);
//
//		if (ViewportAddress->Object.Kind == 2)
//		{
//			BeginOpengl(0, 0, GetWindowsX, GetWindowsY);
//			Projection(Angle, &ScreenX, &ScreenY);
//			EndOpengl();
//
//			ScreenX -= (int)(Width / 2);
//
//			// Texto do HP aprimorado
//			auto FormatNumber = [](int num) -> std::string {
//				char buffer[16];
//				if (num >= 1000000000)  // 1 bilhão
//					sprintf(buffer, "%.1fTỉ", num / 1000000000.0);
//				else if (num >= 1000000) // 1 milhão
//					sprintf(buffer, "%.1ftr", num / 1000000.0);
//				else if (num >= 1000)    // 1 mil
//					sprintf(buffer, "%.1fK", num / 1000.0);
//				else
//					sprintf(buffer, "%d", num);
//				return std::string(buffer);
//				};
//
//			std::string hpCurrent = FormatNumber(lpNewHealthBar->ViewHP[0]);
//			std::string hpMax = FormatNumber(lpNewHealthBar->ViewHP[1]);
//
//			sprintf(Text, "%s / %s (%d%%)", hpCurrent.c_str(), hpMax.c_str(), lpNewHealthBar->rate);
//
//			g_pRenderText->SetFont(g_hFont);
//			g_pRenderText->SetTextColor(255, 255, 255, 255);
//			g_pRenderText->SetBgColor(0, 0, 0, 128);
//			g_pRenderText->RenderText(ScreenX + (Width / 2), ScreenY - 8, Text, Width, 0, 8);
//
//			sprintf(Text2, "%s", ViewportAddress->ID);
//			g_pRenderText->RenderText(ScreenX + (Width / 2), ScreenY - 18, Text2, Width, 0, 8);
//
//			// Barra de fundo
//			EnableAlphaTest();
//			glColor4f(0.2f, 0.2f, 0.2f, 0.8f);
//			RenderColor((float)(ScreenX), (float)(ScreenY + 2), Width, 6.f);
//
//			// Cálculo da porcentagem do HP
//			float HP_Percentage = (float)lpNewHealthBar->ViewHP[0] / (float)lpNewHealthBar->ViewHP[1];
//			float HP_Width = Width * HP_Percentage;
//
//			// Se a vida diminuiu, inicia o timer e mantém a barra de marcação do dano
//			if (lpNewHealthBar->ViewHP[0] < LastHP[j]) {
//				DamageTimer[j] = 2.0f; // Agora dura 1,5 segundos antes de desaparecer
//			}
//
//			// Suaviza a descida da barra de marcação do dano
//			if (LastHP[j] > lpNewHealthBar->ViewHP[0]) {
//				LastHP[j] -= ((LastHP[j] - lpNewHealthBar->ViewHP[0]) * 0.2f); // Suavização lenta
//			}
//			else {
//				LastHP[j] = lpNewHealthBar->ViewHP[0];
//			}
//
//			float LastHP_Percentage = (float)LastHP[j] / (float)lpNewHealthBar->ViewHP[1];
//			float LastHP_Width = min(Width, Width * LastHP_Percentage);
//
//			// Reduz gradualmente a transparência da marcação do dano
//			if (DamageTimer[j] > 0) {
//				glColor4f(0.8f, 0.0f, 0.0f, DamageTimer[j] * 0.66f); // Mais opaco no início, sumindo devagar
//				RenderColor((float)(ScreenX + HP_Width), (float)(ScreenY + 3), LastHP_Width - HP_Width, 4.f);
//				DamageTimer[j] -= 0.008f; // Agora a transparência desaparece mais devagar
//			}
//
//			// Barra de HP principal com gradiente
//			float Red = 1.0f - HP_Percentage;
//			float Green = HP_Percentage;
//			glColor4f(Red, Green, 0.f, 1.f);
//			RenderColor((float)(ScreenX + 1), (float)(ScreenY + 3), HP_Width - 2, 4.f);
//
//			DisableAlphaBlend();
//		}
//		else if (ViewportAddress->Object.Kind == 1)
//		{
//
//			BeginOpengl(0, 0, GetWindowsX, GetWindowsY);
//			Projection(Angle, &ScreenX, &ScreenY);
//			EndOpengl();
//			if (CheckMouseIn(ScreenX - 20, ScreenY, 40, 120))
//			{
//				if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
//
//					EnableAlphaTest();
//
//					glColor4f(0, 0.f, 0.f, 1.f);
//					RenderColor((float)((GetWindowsX / 2) - 51), (float)(GetWindowsY - 80 - 1), 102, 8.f);
//
//					glColor4f(1.f, 0.f, 0.f, 1.f);
//					RenderColor((float)((GetWindowsX / 2) - 50), (float)(GetWindowsY - 80), lpNewHealthBar->rate, 6.f);
//
//					DisableAlphaBlend();
//					glColor3f(1.f, 1.f, 1.f);
//
//					EnableAlphaBlend();
//
//					g_pRenderText->SetFont(g_hFont);
//					g_pRenderText->SetTextColor(255, 255, 255, 255);
//					g_pRenderText->SetBgColor(0, 0, 0, 0);
//
//					sprintf(Text3, "%d%%", lpNewHealthBar->rate);
//
//					g_pRenderText->RenderText((float)((GetWindowsX / 2)), (float)(GetWindowsY - 81), Text3, 100.f, 0, 8);
//				}
//			}
//		}
//
//	}
//	DisableAlphaBlend();
//	glColor3f(1.f, 1.f, 1.f);
//}
