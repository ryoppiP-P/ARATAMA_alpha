
//==============================================================================
//
//   スプライト描画 [sprite.cpp]
//														 Author : Yumito Yokozuka
//														 Date   : 2025/05/15
//------------------------------------------------------------------------------
//
//==============================================================================
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
#include "direct3d.h"
#include "shader.h"
#include "debug_ostream.h"
#include "texture.h"
#include "sprite.h"
#include "camera.h"

static Camera* camera{nullptr};

static constexpr int NUM_VERTEX = 4; // 頂点数


static ID3D11Buffer* g_pVertexBuffer = nullptr; // 頂点バッファ

// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;


// 頂点構造体
struct Vertex
{
	XMFLOAT3 position;	//座標
	XMFLOAT4 color;		//カラー
	XMFLOAT2 texcoord;	//テクスチャ座標(UV座標)
};


void Sprite_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	// デバイスとデバイスコンテキストのチェック
	if (!pDevice || !pContext) {
		hal::dout << "Sprite_Initialize() : 与えられたデバイスかコンテキストが不正です" << std::endl;
		return;
	}

	// デバイスとデバイスコンテキストの保存
	g_pDevice = pDevice;
	g_pContext = pContext;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex) * NUM_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	g_pDevice->CreateBuffer(&bd, NULL, &g_pVertexBuffer);
}

void Sprite_Finalize(void)
{
	SAFE_RELEASE(g_pVertexBuffer);
}

//void Sprite_Draw(int texid, float x, float y, float w, float h,int tx, int ty, int tw, int th,XMFLOAT4 color,SpriteDirection dir)
//{
//	Shader_Begin();
//
//	D3D11_MAPPED_SUBRESOURCE msr;
//	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
//	Vertex* v = (Vertex*)msr.pData;
//
//	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
//	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();
//
//	// 位置設定（左上基準）
//	v[0].position = { x,       y,       0.0f };
//	v[1].position = { x + w,   y,       0.0f };
//	v[2].position = { x,       y + h,   0.0f };
//	v[3].position = { x + w,   y + h,   0.0f };
//
//	// 頂点カラー
//	v[0].color = color;
//	v[1].color = color;
//	v[2].color = color;
//	v[3].color = color;
//
//	// テクスチャサイズ取得
//	XMUINT2 size = Texture_GetSize(texid);
//
//	// UV計算
//	float u0 = tx / (float)size.x;
//	float v0 = ty / (float)size.y;
//	float u1 = (tx + tw) / (float)size.x;
//	float v1 = (ty + th) / (float)size.y;
//
//	// 向きによってUVを反転
//	switch (dir)
//	{
//	case SpriteDirection::NORMAL:
//		break;
//	case SpriteDirection::FLIP_X:
//		std::swap(u0, u1);
//		break;
//	case SpriteDirection::FLIP_Y:
//		std::swap(v0, v1);
//		break;
//	case SpriteDirection::FLIP_XY:
//		std::swap(u0, u1);
//		std::swap(v0, v1);
//		break;
//	}
//
//	// テクスチャ座標設定
//	v[0].texcoord = { u0, v0 };
//	v[1].texcoord = { u1, v0 };
//	v[2].texcoord = { u0, v1 };
//	v[3].texcoord = { u1, v1 };
//
//	// 頂点バッファ解放
//	g_pContext->Unmap(g_pVertexBuffer, 0);
//
//	// 頂点バッファ設定
//	UINT stride = sizeof(Vertex);
//	UINT offset = 0;
//	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
//
//	// 行列設定
//	Shader_SetWorldMatrix(XMMatrixIdentity());
//	Shader_SetProjectionMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));
//	Shader_SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
//
//	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
//	Texture_SetTexture(texid);
//
//	g_pContext->Draw(NUM_VERTEX, 0);
//}


void Sprite_Draw(int texid, float x, float y, float w, float h, int tx, int ty, int tw, int th, XMFLOAT4 color)
{
	// シェーダーを描画パイプラインに設定
	Shader_Begin();

	// 頂点バッファをロックする
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// 頂点バッファへの仮想ポインタを取得
	Vertex* v = (Vertex*)msr.pData;

	// 頂点情報を書き込み
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();

	// 画面の左上から右下に向かう線分を描画する
	v[0].position = { x,	 y,		0.0f };
	v[1].position = { x + w ,y,		0.0f };
	v[2].position = { x,	 y + h, 0.0f };
	v[3].position = { x + w, y + h, 0.0f };

	//頂点カラー
	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	XMUINT2 size = Texture_GetSize(texid);

	float u0 = tx / (float)size.x;
	float v0 = ty / (float)size.y;
	float u1 = (tx + tw) / (float)size.x;
	float v1 = (ty + th) / (float)size.y;
	
	//テクスチャ座標
	v[0].texcoord = { u0, v0 };
	v[1].texcoord = { u1, v0 };
	v[2].texcoord = { u0, v1 };
	v[3].texcoord = { u1, v1 };

	// 頂点バッファのロックを解除
	g_pContext->Unmap(g_pVertexBuffer, 0);

	// 頂点バッファを描画パイプラインに設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	//頂点シェーダーにWorld変換行列を設定
	Shader_SetWorldMatrix(XMMatrixIdentity());

	// 頂点シェーダーにProjection変換行列を設定
	Shader_SetProjectionMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));

	Shader_SetColor({ 1.0f,1.0f,1.0f, 1.0f });

	// プリミティブトポロジ設定
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//テクスチャの設定
	Texture_SetTexture(texid);

	// スプライト描画命令発行
	g_pContext->Draw(NUM_VERTEX, 0);
}

void Sprite_Draw(int texid, float x, float y, float w, float h, XMFLOAT4 color)
{
	// シェーダーを描画パイプラインに設定
	Shader_Begin();

	// 頂点バッファをロックする
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// 頂点バッファへの仮想ポインタを取得
	Vertex* v = (Vertex*)msr.pData;

	// 頂点情報を書き込み
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();

	XMUINT2 size = Texture_GetSize(texid);

	// 画面の左上から右下に向かう線分を描画する
	v[0].position = { x,	 y,		0.0f };
	v[1].position = { x + w ,y,		0.0f };
	v[2].position = { x,	 y + h, 0.0f };
	v[3].position = { x + w, y + h, 0.0f };

	//頂点カラー
	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	//テクスチャ座標
	v[0].texcoord = { 0.0f, 0.0f };
	v[1].texcoord = { 1.0f, 0.0f };
	v[2].texcoord = { 0.0f, 1.0f };
	v[3].texcoord = { 1.0f, 1.0f };

	// 頂点バッファのロックを解除
	g_pContext->Unmap(g_pVertexBuffer, 0);

	// 頂点バッファを描画パイプラインに設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	//頂点シェーダーにWorld変換行列を設定
	Shader_SetWorldMatrix(XMMatrixIdentity());

	// 頂点シェーダーにProjection変換行列を設定
	Shader_SetProjectionMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));

	Shader_SetColor({ 1.0f,1.0f,1.0f, 1.0f });

	// プリミティブトポロジ設定
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//テクスチャの設定
	Texture_SetTexture(texid);

	// スプライト描画命令発行
	g_pContext->Draw(NUM_VERTEX, 0);
}


void Sprite_Draw(int texid, float x, float y, XMFLOAT4 color)
{
	// シェーダーを描画パイプラインに設定
	Shader_Begin();

	// 頂点バッファをロックする
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// 頂点バッファへの仮想ポインタを取得
	Vertex* v = (Vertex*)msr.pData;

	// 頂点情報を書き込み
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();

	XMUINT2 size = Texture_GetSize(texid);

	// 画面の左上から右下に向かう線分を描画する
	v[0].position = { x,	 y,		0.0f };
	v[1].position = { x + size.x ,y,		0.0f };
	v[2].position = { x,	 y + size.y, 0.0f };
	v[3].position = { x + size.x, y + size.y, 0.0f };

	//頂点カラー
	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	//テクスチャ座標
	v[0].texcoord = { 0.0f, 0.0f };
	v[1].texcoord = { 1.0f, 0.0f };
	v[2].texcoord = { 0.0f, 1.0f };
	v[3].texcoord = { 1.0f, 1.0f };

	// 頂点バッファのロックを解除
	g_pContext->Unmap(g_pVertexBuffer, 0);

	// 頂点バッファを描画パイプラインに設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	//頂点シェーダーにWorld変換行列を設定
	Shader_SetWorldMatrix(XMMatrixIdentity());

	// 頂点シェーダーにProjection変換行列を設定
	Shader_SetProjectionMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));

	Shader_SetColor({ 1.0f,1.0f,1.0f, 1.0f });

	// プリミティブトポロジ設定
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//テクスチャの設定
	Texture_SetTexture(texid);

	// スプライト描画命令発行
	g_pContext->Draw(NUM_VERTEX, 0);
}

void Sprite_Draw(int texid, float x, float y, float w, float h, int tx, int ty, int tw, int th, float angle, DirectX::XMFLOAT4 color)
{
	// シェーダーを描画パイプラインに設定
	Shader_Begin();

	// 頂点バッファをロックする
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// 頂点バッファへの仮想ポインタを取得
	Vertex* v = (Vertex*)msr.pData;

	// 頂点情報を書き込み
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();

	// 画面の左上から右下に向かう線分を描画する
	v[0].position = { 0.0f, 0.0f, 0.0f };
	v[1].position = { 1.0f, 0.0f, 0.0f };
	v[2].position = { 0.0f, 1.0f, 0.0f };
	v[3].position = { 1.0f, 1.0f, 0.0f };

	//頂点カラー
	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	XMUINT2 size = Texture_GetSize(texid);

	float u0 = tx / (float)size.x;
	float v0 = ty / (float)size.y;
	float u1 = (tx + tw) / (float)size.x;
	float v1 = (ty + th) / (float)size.y;

	//テクスチャ座標
	v[0].texcoord = { u0, v0 };
	v[1].texcoord = { u1, v0 };
	v[2].texcoord = { u0, v1 };
	v[3].texcoord = { u1, v1 };

	// 頂点バッファのロックを解除
	g_pContext->Unmap(g_pVertexBuffer, 0);

	// 頂点バッファを描画パイプラインに設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	//頂点シェーダーにWorld変換行列を設定
	XMMATRIX s = XMMatrixScaling(w, h, 1.0f);
	XMMATRIX r = XMMatrixRotationZ(angle);
	XMMATRIX t = XMMatrixTranslation(x, y, 0.0f);
	Shader_SetWorldMatrix(s * r * t);

	// 頂点シェーダーにProjection変換行列を設定
	Shader_SetProjectionMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));

	Shader_SetColor({ 1.0f,1.0f,1.0f, 1.0f });

	// プリミティブトポロジ設定
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//テクスチャの設定
	Texture_SetTexture(texid);

	// スプライト描画命令発行
	g_pContext->Draw(NUM_VERTEX, 0);

}

void Sprite_Draw_Move(int texid, float x, float y, float w, float h, XMFLOAT4 color)
{
	// シェーダーを描画パイプラインに設定
	Shader_Begin();

	// 頂点バッファをロックする
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// 頂点バッファへの仮想ポインタを取得
	Vertex* v = (Vertex*)msr.pData;

	// 頂点情報を書き込み
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();

	XMUINT2 size = Texture_GetSize(texid);

	static float b = 0.0f;

	b += 0.001f;

	// 画面の左上から右下に向かう線分を描画する
	v[0].position = { x,	 y,		0.0f };
	v[1].position = { x + w ,y,		0.0f };
	v[2].position = { x,	 y + h, 0.0f };
	v[3].position = { x + w, y + h, 0.0f };

	//頂点カラー
	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	//テクスチャ座標
	v[0].texcoord = { 0.0f + b, 0.0f + b };
	v[1].texcoord = { 1.0f + b, 0.0f + b };
	v[2].texcoord = { 0.0f + b, 1.0f + b };
	v[3].texcoord = { 1.0f + b, 1.0f + b };

	// 頂点バッファのロックを解除
	g_pContext->Unmap(g_pVertexBuffer, 0);

	// 頂点バッファを描画パイプラインに設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	//頂点シェーダーにWorld変換行列を設定
	Shader_SetWorldMatrix(XMMatrixIdentity());

	// 頂点シェーダーにProjection変換行列を設定
	Shader_SetProjectionMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));

	Shader_SetColor({ 1.0f,1.0f,1.0f, 1.0f });

	// プリミティブトポロジ設定
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//テクスチャの設定
	Texture_SetTexture(texid);

	// スプライト描画命令発行
	g_pContext->Draw(NUM_VERTEX, 0);
}

//scroll描画
//---------------------------------------------------------------------------------------------------
void Sprite_ScrollDraw(int texid, float x, float y, float w, float h, int tx, int ty, int tw, int th, XMFLOAT4 color)
{
	if (camera)
	{
		float camX = camera->GetPosition().x;
		float camY = camera->GetPosition().y;

		// カメラのオフセットを反映して描画
		float drawX = x - camX;
		float drawY = y - camY;

		Sprite_Draw(
			texid,
			drawX, drawY, w, h,
			tx, ty, tw, th,
			color
		);
	}
}

void Sprite_ScrollDraw(int texid, float x, float y, float w, float h, XMFLOAT4 color)
{
	if (camera)
	{
		float camX = camera->GetPosition().x;
		float camY = camera->GetPosition().y;

		// カメラのオフセットを反映して描画
		float drawX = x - camX;
		float drawY = y - camY;

		Sprite_Draw(
			texid,
			drawX, drawY, w, h,
			color
		);
	}
}

void Sprite_ScrollDraw(int texid, float x, float y, float w, float h, int tx, int ty, int tw, int th, float angle, DirectX::XMFLOAT4 color)
{
	if (camera)
	{
		float camX = camera->GetPosition().x;
		float camY = camera->GetPosition().y;

		// カメラのオフセットを反映して描画
		float drawX = x - camX;
		float drawY = y - camY;

		Sprite_Draw(
			texid,
			drawX, drawY, w, h,
			tx, ty, tw, th,
			angle,
			color
		);
	}
}

void DrawSetCamera(Camera* setCamera)
{
	camera = setCamera;
}
