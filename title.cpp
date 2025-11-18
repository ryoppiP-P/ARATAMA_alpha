//==============================================================================
//
//   タイトル画面表示 [title.cpp]
// Author : Yumito Yokozuka
// Date   : 2025/09/12
//------------------------------------------------------------------------------
//
//==============================================================================
#include "title.h"
#include "key_logger.h"
#include "fade.h"
#include "scene.h"
#include "direct3d.h"
#include "sprite.h"
#include "texture.h"
#include <math.h>
#include <fstream>


static double g_AccumulatedTime = 0.0;
static double g_KeyInputTime = 0.0;
static int g_texId = -1;

enum TitleState
{
	TITLE_STATE_FADE_IN,
	TITLE_STATE_KEYINPUT_WAIT,
	TITLE_STATE_KEYINPUT_ACTION,
	TITLE_STATE_FADE_OUT,
	TITLE_STATE_MAX,
};

static TitleState g_State = TITLE_STATE_FADE_IN;

void Title_Initialize()
{
	g_AccumulatedTime = 0.0;

	g_texId = Texture_Load(L"resource/texture/title.png");

	Fade_Start(1.0f, true);

	g_State = TITLE_STATE_FADE_IN;
}

void Title_Finalize()
{
}

void Title_Update(double elapsed_time)
{
	g_AccumulatedTime += elapsed_time;
	switch (g_State)
	{
	case TITLE_STATE_FADE_IN:
		if (Fade_GetState() == FADE_STATE_FADE_IN_FINISHED)
		{
			g_State = TITLE_STATE_KEYINPUT_WAIT;
		}
		break;
	case TITLE_STATE_KEYINPUT_WAIT:
		if (KeyLogger_IsTrigger(KK_ENTER))
		{
			g_State = TITLE_STATE_KEYINPUT_ACTION;
			g_KeyInputTime = g_AccumulatedTime;
		}
		break;
	case TITLE_STATE_KEYINPUT_ACTION:
		if (g_AccumulatedTime - g_KeyInputTime > 1.0)
		{
			g_State = TITLE_STATE_FADE_OUT;
			Fade_Start(1.0f, false);
		}
		break;
	case TITLE_STATE_FADE_OUT:
		if (Fade_GetState() == FADE_STATE_FADE_OUT_FINISHED)
		{
			//ゲームシーンへ移行
			Scene_SetNextScene(SCENE_GAME);
		}
		break;
	default:
		break;
	}
}

void Title_Draw()
{
	Sprite_Draw(g_texId, 0.0f, 0.0f, 1920.0f, 1080.0f);
}
