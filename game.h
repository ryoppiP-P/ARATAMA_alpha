//==============================================================================
//
// ÉQÅ[ÉÄÇÃêßå‰ [game.h]
// Author : Yumito Yokozuka
// Date   : 2025/07/02
//------------------------------------------------------------------------------
//
//==============================================================================
#ifndef GAME_H
#define GAME_H

constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;

void Game_Initialize();
void Game_Finalize();
void Game_Update(double elapsed_time);
void Game_Draw();

float GetTimeScale();
void SetTimeScale(float timeScale);

#endif //GAME_H