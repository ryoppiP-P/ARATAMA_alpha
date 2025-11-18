#ifndef DIRECT3D_H
#define DIRECT3D_H

#include <windows.h>
#include <d3d11.h>

//セーフリリースマクロ
#define SAFE_RELEASE(o) if (o) { (o)->Release(); o = NULL;}

bool Direct3D_Initialize(HWND hWnd);	//Direct3Dの初期化
void Direct3D_Finalize();	//Direct3Dの終了処理

void Direct3D_Clear();	//バックバッファのクリア
void Direct3D_Present();	//バックバッファの表示

unsigned int Direct3D_GetBackBufferWidth();
unsigned int Direct3D_GetBackBufferHeight();

ID3D11Device* Direct3D_GetDevice();
ID3D11DeviceContext* Direct3D_GetDeviceContext();

//void SetViewport(int n);

enum AlphaBlendMode
{
	BLEND_TRANSPARENT,
	BLEND_ADD,
};

void Direct3D_SetAlphaBlend(AlphaBlendMode blend);

#endif
