//==============================================================================
//
//   当たり判定の管理 [CollisionManager.h]
// Author : Yumito Yokozuka
// Date   : 2025/11/07
//------------------------------------------------------------------------------
//
//==============================================================================
#include <vector>
#include "game_object.h"
#include "collision.h"

class CollisionManager
{
private:
    std::vector<GameObject*> m_objects; // 当たり判定を行うオブジェクトのリスト
    Collision m_collision;              // 実際の当たり判定処理を持つクラス

public:
    CollisionManager() = default;
    ~CollisionManager() = default;

    void AddObject(GameObject* obj) { m_objects.push_back(obj); } // オブジェクト追加
    void Update();
    void Draw();
};
