//==============================================================================
//
//   シーン管理 [scene.cpp]
// Author : Yumito Yokozuka
// Date   : 2025/09/17
//------------------------------------------------------------------------------
//
//==============================================================================
#include "scene.h"
#include "title.h"
#include "game.h"
#include "result.h"

static Scene g_Scene = SCENE_TITLE;	//
static Scene g_SceneNext = g_Scene;

typedef void (*FuncVoid)(void);
typedef void (*FuncUpdate)(double);

static FuncVoid g_Initialize[]
{//※enumと同じ順番
	Title_Initialize,//()がない⇒関数の実行アドレス
	Game_Initialize,
	Result_Initialize
};

static FuncVoid g_Finalize[]
{
	Title_Finalize,
	Game_Finalize,
	Result_Finalize
};

static FuncUpdate g_Update[]
{
	Title_Update,
	Game_Update,
	Result_Update
};

static FuncVoid g_Draw[]
{
	Title_Draw,
	Game_Draw,
	Result_Draw
};

void Scene_Initialize()
{
	g_Initialize[g_Scene]();
}

void Scene_Finalize()
{
	g_Finalize[g_Scene]();
}

void Scene_Update(double elapsed_time)
{
	g_Update[g_Scene](elapsed_time);
}

void Scene_Draw()
{
	g_Draw[g_Scene]();
}

void Scene_SetNextScene(Scene scene)
{
	g_SceneNext = scene;
}

void Scene_ChangeScene()
{
	if (g_Scene != g_SceneNext)
	{
		//現在のシーンの後かたずけ
		Scene_Finalize();
		//シーンを切り替える
		g_Scene = g_SceneNext;
		//シーンを初期化
		Scene_Initialize();
	}
}
