//==============================================================================
//
// ÉQÅ[ÉÄÇÃêßå‰ [game.cpp]
// Author : Yumito Yokozuka
// Date   : 2025/11/07
//------------------------------------------------------------------------------
//
//==============================================================================
#include "game.h"
#include "key_logger.h"
#include "Audio.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"
#include "fade.h"
#include "camera.h"
#include "CollisionManager.h"
#include "game_player.h"
#include "game_stage.h"
#include "game_enemy_manager.h"

static float g_timeScale = 1.0f;

static CollisionManager collisionManager;
static EnemyManager g_enemyManager;
static Player player({ 100.0f,100.0f }, { 128.0f,80.0f });
static Camera camera;
static Stage g_stage;

void Game_Initialize()
{
	Fade_Start(1.0, true);
	g_stage.Initialize();
	DrawSetCamera(&camera); //égÇ§ÉJÉÅÉâÇÃê›íË
	player.Initialize();

	g_enemyManager.Initialize();
	collisionManager.AddObject(&player);

	for (auto enemy : g_enemyManager.GetEnemys())
	{
		collisionManager.AddObject(enemy);
	}

	for (auto block : g_stage.GetBlocks())
	{
		collisionManager.AddObject(block);
	}
}

void Game_Finalize()
{
	g_enemyManager.Finalize();
	g_stage.Finalize();
}

void Game_Update(double elapsed_time)
{
	if (KeyLogger_IsTrigger(KK_ENTER))
	{
		Scene_SetNextScene(SCENE_TITLE);
	}
	player.Update(elapsed_time);
	g_stage.Update(elapsed_time);

	g_enemyManager.SetPlayerPos(player.GetPosition());

	g_enemyManager.Update(elapsed_time);
	collisionManager.Update();
	camera.SetPosition(player.GetPosition().x, player.GetPosition().y);
	camera.Update(elapsed_time);
}

void Game_Draw()
{
	g_stage.Draw();
	player.Draw();

	g_enemyManager.Draw();
	collisionManager.Draw();
}

float GetTimeScale()
{
	return g_timeScale;
}

void SetTimeScale(float timeScale)
{
	g_timeScale = timeScale;
}
