//==============================================================================
// ゲームオブジェクト基底クラス [game_object.h]
// Author : yokozuka yumito
// Date   : 2025/11/07
//==============================================================================
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <DirectXMath.h>
#include <vector>
#include "collision.h"

// オブジェクトの種類
//enemyType廃止してgameObjectに敵の種類書く
enum class ObjectTag {
    NONE,
    PLAYER,
    ENEMY_KATANA,
    ENEMY_BOW,
    ENEMY_KANABO,
    BLOCK,
    BULLET
};

class GameObject
{
private:
    DirectX::XMFLOAT2 m_position{};
    DirectX::XMFLOAT2 m_size{};
    bool m_use{ true };

    std::vector<Collider> m_colliders;
    ObjectTag m_tag;

public:
    GameObject(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size, ObjectTag tag = ObjectTag::NONE)
        : m_position(position), m_size(size), m_use(true) ,m_tag(tag){
    }
    virtual ~GameObject() {}

    virtual void Initialize() {}
    virtual void Finalize() {}
    virtual void Update(double elapsed_time) {}
    virtual void Draw() {}

    // 当たり判定イベント
    virtual void OnCollision(const GameObject* otherObject, const Collider* myCollider, const Collider* otherCollider, COLLISION_VEC hitVec = COLLISION_VEC::NONE) {}

    //====================================================
    // Collider管理
    // 
    // ※当たり判定の分だけ事前にx個容量確保(コンストラクタ)
    // GetColliders().reserve(x);
    // 
    // Collider追加
    void AddCollider(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size, ColliderType type)
    {
        m_colliders.emplace_back(this, pos, size, type);
    }

    std::vector<Collider>& GetColliders() { return m_colliders; }
    const std::vector<Collider>& GetColliders() const { return m_colliders; }
    //====================================================

    // 有効・無効制御
    bool IsUse() const { return m_use; }
    void SetUse(bool use) { m_use = use; }

    //セッター
    void SetPosition(DirectX::XMFLOAT2 position) { m_position = position; }
    void SetSize(DirectX::XMFLOAT2 size) { m_size = size; }
    void SetTag(ObjectTag tag) { m_tag = tag; }

    //ゲッター
    DirectX::XMFLOAT2 GetPosition() const { return m_position; }
    DirectX::XMFLOAT2 GetSize() const { return m_size; }
    ObjectTag GetTag() const { return m_tag; }
};

#endif // GAME_OBJECT_H

