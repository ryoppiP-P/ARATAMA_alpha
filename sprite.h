//==============================================================================
//
//   スプライト描画 [sprite.h]
//														 Author : Yumito Yokozuka
//														 Date   : 2025/05/15
//------------------------------------------------------------------------------
//
//==============================================================================
#ifndef SPRITE_H
#define SPRITE_H

#include <d3d11.h>
#include <DirectXMath.h>

#include "camera.h"

// ============================================
// スプライトの向き
// ============================================
enum SpriteDirection
{
	NORMAL,     // 通常（左→右）
	FLIP_X,     // 左右反転
	FLIP_Y,     // 上下反転
	FLIP_XY     // 左右＋上下反転
};

void Sprite_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Sprite_Finalize(void);
//void Sprite_Draw(int texid, float x, float y, float w, float h, int tx, int ty, int tw, int th, XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }, SpriteDirection dir = SpriteDirection::NORMAL);
void Sprite_Draw(int texid, float x, float y, float w, float h, int tx, int ty, int tw, int th, DirectX::XMFLOAT4 color = {1.0f,1.0f,1.0f,1.0f});
void Sprite_Draw(int texid, float x, float y, float w, float h, DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });
void Sprite_Draw(int texid, float x, float y, DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });
//angle付き
void Sprite_Draw(int texid, float x, float y, float w, float h, int tx, int ty, int tw, int th, float angle, DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });
void Sprite_Draw_Move(int texid, float x, float y, float w, float h, DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });

void Sprite_ScrollDraw(int texid, float x, float y, float w, float h, int tx, int ty, int tw, int th, DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });
void Sprite_ScrollDraw(int texid, float x, float y, float w, float h, DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });
//angle付き
void Sprite_ScrollDraw(int texid, float x, float y, float w, float h, int tx, int ty, int tw, int th, float angle, DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });

void DrawSetCamera(Camera* camera);

#endif // SPRITE_H
