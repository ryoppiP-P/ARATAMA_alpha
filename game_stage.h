//==============================================================================
//
//   ステージクラス [game_stage.h]
// Author : Yumito Yokozuka
// Date   : 2025/11/07
//------------------------------------------------------------------------------
//
//==============================================================================
#ifndef GAME_STAGE_H
#define GAME_STAGE_H

#include <vector>
#include "game_block.h"

class Stage
{
private:
    std::vector<Block*> m_blocks;

public:
    Stage() = default;
    ~Stage();

    void Initialize();
    void Finalize();
    void Update(double elapsed_time);
    void Draw();

    // ステージにブロックを配置
    void AddBlock(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size);
    const std::vector<Block*>& GetBlocks() const { return m_blocks; }
};

#endif // GAME_STAGE_H
