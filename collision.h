// ===================================================
// collision.h 当たり判定
// 
// 制作者：yokozuka yumito	日付：2025/11/06
// ===================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include <DirectXMath.h>

enum class COLLISION_VEC {
	NONE = 0,	//当たってない
	RIGHT,		//右に当たった
	LEFT,		//左
	BOTTOM,		//下
	TOP,		//上
};

enum class ColliderType {
	BODY,
	ATTACK
};

// ===================================================
// Colliderクラス
// 当たり判定の処理に必要な情報を持つクラス
// 
// コンストラクタ：所有者,コライダーの座標,サイズ,判定のタイプ
// ===================================================
class Collider
{
private:
	class GameObject* m_pOwnerObject{ nullptr };	//所有者
	DirectX::XMFLOAT2 m_colliderPos{};				//座標
	DirectX::XMFLOAT2 m_colliderSize{};				//サイズ
	ColliderType m_type{ ColliderType::BODY };		//判定のタイプ
	bool m_isActive{ true };						//有効か?

public:
	Collider() = default;
	Collider(GameObject* pObject, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size, ColliderType type) 
		:m_pOwnerObject(pObject), m_colliderPos(pos), m_colliderSize(size), m_type(type), m_isActive(true) {};

	//セッター
	void SetOwnerObject(GameObject* object) { m_pOwnerObject = object; }
	void SetColliderPos(DirectX::XMFLOAT2 position) { m_colliderPos = position; }
	void SetColliderSize(DirectX::XMFLOAT2 size) { m_colliderSize = size; }
	void SetType(ColliderType type) { m_type = type; }
	void SetActive(bool active) { m_isActive = active; }

	//ゲッター
	GameObject* GetOwnerObject() const { return m_pOwnerObject; }
	DirectX::XMFLOAT2 GetColliderPos() const { return m_colliderPos; }
	DirectX::XMFLOAT2 GetColliderSize() const { return m_colliderSize; }
	ColliderType GetType() const { return m_type; }
	bool IsActive() const { return m_isActive; }
};


// ===================================================
// Collisionクラス
// 当たり判定の処理を行うクラス
// 
// 引数:
// 	ColliderA
// 	ColliderB
// 
// 戻り値
//  true：当たっている
// 	false：当たっていない
// 
// COLLISION_VEC：当たった方向
// ===================================================
class Collision
{
public:
	bool CheckBoxCollider(Collider* pCollider1, Collider* pCollider2);
	COLLISION_VEC CheckCollisionDirection(Collider* pCollider1, Collider* pCollider2);
	bool CheckCircleCollider(Collider* pCollider1, Collider* pCollider2);

	void DebugCheckColliderDraw(Collider* pCollider);
};

#endif //_COLLISION_H_