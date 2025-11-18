//==============================================================================
//
//   ブロッククラス [game_block.h]
// Author : Yumito Yokozuka
// Date   : 2025/11/07
//------------------------------------------------------------------------------
//
//==============================================================================
#ifndef GAME_BLOCK_H
#define GAME_BLOCK_H

#include <DirectXMath.h>
#include "game_object.h"

class Block : public GameObject
{
private:
    static constexpr float BLOCK_SIZE_W = 64.0f;
    static constexpr float BLOCK_SIZE_H = 64.0f;

private:
    static int m_BlockTexId;

    Collider* m_bodyCollider{ nullptr }; // ブロックの当たり判定

public:
    Block(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size);
    ~Block() = default;

    void Initialize() override;
    void Finalize() override;
    void Update(double elapsed_time) override;
    void Draw() override;

    // 当たり判定が起きたときに呼ばれる
    void OnCollision(const GameObject* otherObject, const Collider* myCollider, const Collider* otherCollider, COLLISION_VEC hitVec) override;

private:
    // コライダーの位置更新
    void UpdateColliderPosition();
};

#endif // GAME_BLOCK_H
