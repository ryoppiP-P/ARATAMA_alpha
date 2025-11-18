//==============================================================================
//
//   ステージクラス [game_stage.cpp]
// Author : Yumito Yokozuka
// Date   : 2025/11/07
//------------------------------------------------------------------------------
//
//==============================================================================
#include "game_enemy_manager.h"
#include "game_enemy_katana.h"
#include "game_enemy_bow.h"
#include "game_enemy_kanabo.h"
#include "texture.h"


EnemyManager::~EnemyManager()
{
    Finalize();
}

void EnemyManager::Initialize()
{
    // ステージ用に敵を配置
    // 地面の敵
    AddEnemy({ 100.0f,200.0f }, { 128.0f,80.0f }, Enemy::Enemy_Type::BOW);
    AddEnemy({ 1000.0f,200.0f }, { 128.0f,80.0f }, Enemy::Enemy_Type::KANABO);
    AddEnemy({ 1800.0f,200.0f }, { 128.0f,80.0f }, Enemy::Enemy_Type::KATANA);

    AddEnemy({ 100.0f, -300.0f }, { 128.0f,80.0f }, Enemy::Enemy_Type::BOW);
    AddEnemy({ 1000.0f, -300.0f }, { 128.0f,80.0f }, Enemy::Enemy_Type::KANABO);
    AddEnemy({ 1800.0f, -300.0f }, { 128.0f,80.0f }, Enemy::Enemy_Type::BOW);

    AddEnemy({ 400.0f, -600.0f }, { 128.0f,80.0f }, Enemy::Enemy_Type::KATANA);
    AddEnemy({ 1300.0f, -600.0f }, { 128.0f,80.0f }, Enemy::Enemy_Type::BOW);

    for (auto enemy : m_enemys)
    {
        enemy->Initialize();
    }
}

void EnemyManager::Finalize()
{
    for (auto enemy : m_enemys)
    {
        if (enemy)
        {
            enemy->Finalize();
            delete enemy;
        }
    }
    m_enemys.clear();
}

void EnemyManager::Update(double elapsed_time)
{
    for (auto enemy : m_enemys)
    {
        if (enemy && enemy->IsUse())
        {
            enemy->Update(elapsed_time);
        }
    }
}

void EnemyManager::Draw()
{
    for (auto enemy : m_enemys)
    {
        if (enemy && enemy->IsUse())
        {
            enemy->Draw();
        }
    }
}

void EnemyManager::AddEnemy(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size, Enemy::Enemy_Type type)
{
    Enemy* newEnemy = { nullptr };
    switch (type)
    {
    case Enemy::Enemy_Type::NONE:
        break;
    case Enemy::Enemy_Type::KATANA:
        newEnemy = new EnemyKatana(position, size);
        break;
    case Enemy::Enemy_Type::BOW:
        newEnemy = new EnemyBow(position, size);
        break;
    case Enemy::Enemy_Type::KANABO:
        newEnemy = new EnemyKanabo(position, size);
        break;
    }

    m_enemys.push_back(newEnemy);
}


void EnemyManager::SetPlayerPos(DirectX::XMFLOAT2 playerPos)
{
    m_playerPos = playerPos;
    // 敵全員に配る
    for (auto enemy : m_enemys)
    {
        if (enemy)
        {
            enemy->SetPlayerPos(playerPos);
        }
    }
}

DirectX::XMFLOAT2 EnemyManager::GetPlayerPos() const
{
    return m_playerPos;
}