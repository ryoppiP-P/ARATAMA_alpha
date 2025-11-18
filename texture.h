//======================================================================================
// テクスチャの管理 [texture.h]
// Author : yumito yokozuka
// Date   : 2025/6/6
//--------------------------------------------------------------------------------------
// 
//======================================================================================
#ifndef TEXTURE_H
#define TEXTURE_H

#include <d3d11.h>
#include <DirectXmath.h>

void Texture_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Texture_Finalize();

//テクスチャの読み込み
//
//引数
//pFileName		画像ファイル名
//bMipMap		MipMapを作るときはtrue
//
//戻り値		テクスチャ管理番号。読み込めなかった場合は-1。
//
int Texture_Load(const wchar_t* pFileName, bool bMipMap = true);

void Texture_AllRelease();

void Texture_SetTexture(int texture_id);

DirectX::XMUINT2 Texture_GetSize(int texuture_id);

#endif // TEXTURE_H