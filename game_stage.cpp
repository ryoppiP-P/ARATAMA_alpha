//==============================================================================
//
//   ステージクラス [game_stage.cpp]
// Author : Yumito Yokozuka
// Date   : 2025/11/07
//------------------------------------------------------------------------------
//
//==============================================================================
#include "game_stage.h"
#include "texture.h"

Stage::~Stage()
{
    Finalize();
}

void Stage::Initialize()
{
    // ステージ用にブロックを配置
    AddBlock({ -700.0f, 300.0f }, { 3400.0f, 300.0f });
    AddBlock({ -700.0f, -1100.0f }, { 3400.0f, 100.0f });
    AddBlock({ -700.0f, -1000.0f }, { 100.0f, 1300.0f });
    AddBlock({ 2600.0f, -1000.0f }, { 100.0f, 1300.0f });

    AddBlock({ -100.0f, -100.0f }, { 500.0f, 75.0f });
    AddBlock({ 800.0f, -100.0f }, { 500.0f, 75.0f });
    AddBlock({ 1600.0f, -100.0f }, { 500.0f, 75.0f });

    AddBlock({ 300.0f, -500.0f }, { 500.0f, 75.0f });
    AddBlock({ 1200.0f, -500.0f }, { 500.0f, 75.0f });

    for (auto block : m_blocks)
    {
        block->Initialize();
    }
}

void Stage::Finalize()
{
    for (auto block : m_blocks)
    {
        if (block)
        {
            block->Finalize();
            delete block;
        }
    }
    m_blocks.clear();
}

void Stage::Update(double elapsed_time)
{
    for (auto block : m_blocks)
    {
        if (block && block->IsUse())
        {
            block->Update(elapsed_time);
        }
    }
}

void Stage::Draw()
{
    for (auto block : m_blocks)
    {
        if (block && block->IsUse())
        {
            block->Draw();
        }
    }
}

void Stage::AddBlock(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size)
{
    Block* newBlock = new Block(position, size);
    m_blocks.push_back(newBlock);
}
