//==============================================================================
//
//   当たり判定の管理 [CollisionManager.cpp]
// Author : Yumito Yokozuka
// Date   : 2025/11/07
//------------------------------------------------------------------------------
//
//==============================================================================
#include "CollisionManager.h"

void CollisionManager::Update()
{
    // 全オブジェクトを二重ループで衝突判定
    for (size_t i = 0; i < m_objects.size(); ++i)
    {
        GameObject* objA = m_objects[i];
        if (!objA || !objA->IsUse()) continue;

        for (size_t j = i + 1; j < m_objects.size(); ++j)
        {
            GameObject* objB = m_objects[j];
            if (!objB || !objB->IsUse()) continue;

            // objA の全 Collider × objB の全 Collider をチェック
            auto& collidersA = objA->GetColliders();
            auto& collidersB = objB->GetColliders();

            for (auto& colA : collidersA)
            {
                if (!colA.IsActive()) continue;

                for (auto& colB : collidersB)
                {
                    if (!colB.IsActive()) continue;

                    // 衝突判定
                    if (m_collision.CheckBoxCollider(&colA, &colB))
                    {
                        // 衝突方向を取得
                        COLLISION_VEC hitVecA = m_collision.CheckCollisionDirection(&colA, &colB);
                        // 衝突信号をオブジェクトに送る
                        objA->OnCollision(objB, &colA, &colB, hitVecA);

                        COLLISION_VEC hitVecB = m_collision.CheckCollisionDirection(&colB, &colA);
                        objB->OnCollision(objA, &colB, &colA, hitVecB);
                    }
                }
            }
        }
    }
}

void CollisionManager::Draw()
{
    for (size_t i = 0; i < m_objects.size(); ++i)
    {
        GameObject* obj = m_objects[i];
        if (!obj || !obj->IsUse()) continue;

        auto& colliders = obj->GetColliders();

        for (auto& col : colliders)
        {
            if (!col.IsActive()) continue;

            m_collision.DebugCheckColliderDraw(&col);
        }
    }
}
