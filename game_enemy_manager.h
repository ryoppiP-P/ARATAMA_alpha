//==============================================================================
//
//   ステージクラス [game_enemy_manager.h]
// Author : Yumito Yokozuka
// Date   : 2025/11/07
//------------------------------------------------------------------------------
//
//==============================================================================
#ifndef GAME_ENEMY_MANAGER_H
#define GAME_ENEMY_MANAGER_H

#include <vector>
#include "game_enemy.h"

class EnemyManager
{
private:
    std::vector<Enemy*> m_enemys;
    DirectX::XMFLOAT2 m_playerPos{};      // 追加内容 ← プレイヤー座標

public:
    EnemyManager() = default;
    ~EnemyManager();

    void Initialize();
    void Finalize();
    void Update(double elapsed_time);
    void Draw();

    // ステージにブロックを配置
    void AddEnemy(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size, Enemy::Enemy_Type type);
    const std::vector<Enemy*>& GetEnemys() const { return m_enemys; }

public:
    // 追加内容
    void SetPlayerPos(DirectX::XMFLOAT2 playerPos);
    DirectX::XMFLOAT2 GetPlayerPos() const;
};

#endif // GAME_ENEMY_MANAGER_H
