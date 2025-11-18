//==============================================================================
//
//   フェード処理 [fade.h]
// Author : Yumito Yokozuka
// Date   : 2025/09/10
//------------------------------------------------------------------------------
//
//==============================================================================
#ifndef FADE_H
#define FADE_H

#include <DirectXMath.h>

void Fade_Initialize();
void Fade_Finalize();
void Fade_Update(double elapsed_time);
void Fade_Draw();

void Fade_Start(double fadeTime, bool isFadeIn, DirectX::XMFLOAT4 fadeColor = { 0.0f,0.0f,0.0f,1.0f });

enum FadeState
{
	FADE_STATE_NONE,
	FADE_STATE_FADE_OUT,
	FADE_STATE_FADE_OUT_FINISHED,
	FADE_STATE_FADE_IN,
	FADE_STATE_FADE_IN_FINISHED,
	FADE_STATE_MAX,
};

FadeState Fade_GetState();

#endif // FADE_H
