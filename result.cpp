//==============================================================================
//
//   リザルト画面表示 [result.cpp]
// Author : Yumito Yokozuka
// Date   : 2025/09/12
//------------------------------------------------------------------------------
//
//==============================================================================
#include "result.h"
#include "key_logger.h"
#include "fade.h"
#include "scene.h"
#include "direct3d.h"
#include <math.h>

static double g_AccumulatedTime = 0.0;
static double g_KeyInputTime = 0.0;

enum ResultState
{
	RESULT_STATE_FADE_IN,
	RESULT_STATE_KEYINPUT_WAIT,
	RESULT_STATE_KEYINPUT_ACTION,
	RESULT_STATE_FADE_OUT,
	RESULT_STATE_MAX,
};

static ResultState g_State = RESULT_STATE_FADE_IN;

void Result_Initialize()
{
	g_AccumulatedTime = 0.0;

	Fade_Start(1.0f, true);

	g_State = RESULT_STATE_FADE_IN;
}

void Result_Finalize()
{
}

void Result_Update(double elapsed_time)
{
	g_AccumulatedTime += elapsed_time;
	switch (g_State)
	{
	case RESULT_STATE_FADE_IN:
		if (Fade_GetState() == FADE_STATE_FADE_IN_FINISHED)
		{
			g_State = RESULT_STATE_KEYINPUT_WAIT;
		}
		break;
	case RESULT_STATE_KEYINPUT_WAIT:

		if (KeyLogger_IsTrigger(KK_ENTER))
		{
			g_State = RESULT_STATE_KEYINPUT_ACTION;
			g_KeyInputTime = g_AccumulatedTime;
		}
		break;
	case RESULT_STATE_KEYINPUT_ACTION:
		if (g_AccumulatedTime - g_KeyInputTime > 1.0)
		{
			g_State = RESULT_STATE_FADE_OUT;
			Fade_Start(1.0f, false);
		}
		break;
	case RESULT_STATE_FADE_OUT:
		if (Fade_GetState() == FADE_STATE_FADE_OUT_FINISHED)
		{
			//タイトルシーンへ移行
			Scene_SetNextScene(SCENE_TITLE);
		}
		break;
	default:
		break;
	}
}

void Result_Draw()
{
}
