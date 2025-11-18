// ===================================================
// collision.cpp 当たり判定
// 
// 制作者：yokozuka yumito	日付：2025/11/06
// ===================================================
#include "collision.h"
#include "texture.h"
#include "sprite.h"
#include <algorithm>


bool Collision::CheckBoxCollider(Collider* pCollider1, Collider* pCollider2)
{
	float ALeft = pCollider1->GetColliderPos().x;
	float ARight = pCollider1->GetColliderPos().x + pCollider1->GetColliderSize().x;
	float ATop = pCollider1->GetColliderPos().y;
	float ABottom = pCollider1->GetColliderPos().y + pCollider1->GetColliderSize().y;

	float BLeft = pCollider2->GetColliderPos().x;
	float BRight = pCollider2->GetColliderPos().x + pCollider2->GetColliderSize().x;
	float BTop = pCollider2->GetColliderPos().y;
	float BBottom = pCollider2->GetColliderPos().y + pCollider2->GetColliderSize().y;

	if ((ARight >= BLeft) &&
		(ALeft <= BRight) &&
		(ABottom >= BTop) &&
		(ATop <= BBottom))
	{
		return true; // 当たっている
	}

	return false;
}

COLLISION_VEC Collision::CheckCollisionDirection(Collider* pCollider1, Collider* pCollider2)
{
	float ALeft = pCollider1->GetColliderPos().x;
	float ARight = pCollider1->GetColliderPos().x + pCollider1->GetColliderSize().x;
	float ATop = pCollider1->GetColliderPos().y;
	float ABottom = pCollider1->GetColliderPos().y + pCollider1->GetColliderSize().y;

	float BLeft = pCollider2->GetColliderPos().x;
	float BRight = pCollider2->GetColliderPos().x + pCollider2->GetColliderSize().x;
	float BTop = pCollider2->GetColliderPos().y;
	float BBottom = pCollider2->GetColliderPos().y + pCollider2->GetColliderSize().y;

	bool isHit =
		(ARight >= BLeft) &&
		(ALeft <= BRight) &&
		(ABottom >= BTop) &&
		(ATop <= BBottom);

	if (!isHit)
		return COLLISION_VEC::NONE;

	// 重なり量
	float overlapLeft = ARight - BLeft;
	float overlapRight = BRight - ALeft;
	float overlapTop = BBottom - ATop;
	float overlapBottom = ABottom - BTop;

	float minOverlap = std::min({ overlapLeft, overlapRight, overlapTop, overlapBottom });

	if (minOverlap == overlapTop)
		return COLLISION_VEC::TOP;       // AがBの下から当たった
	else if (minOverlap == overlapBottom)
		return COLLISION_VEC::BOTTOM;    // AがBの上から当たった
	else if (minOverlap == overlapLeft)
		return COLLISION_VEC::RIGHT;     // AがBの右から当たった
	else if (minOverlap == overlapRight)
		return COLLISION_VEC::LEFT;      // AがBの左から当たった

	return COLLISION_VEC::NONE;
}

bool Collision::CheckCircleCollider(Collider* pCollider1, Collider* pCollider2)
{
	// (円Aの中心座標X - 円Bの中心座標X)の2乗 + (円Aの中心座標Y - 円Bの中心座標Y)の2乗
	float distance = (pCollider1->GetColliderPos().x - pCollider2->GetColliderPos().x) * (pCollider1->GetColliderPos().x - pCollider2->GetColliderPos().x) + (pCollider1->GetColliderPos().y - pCollider2->GetColliderPos().y) * (pCollider1->GetColliderPos().y - pCollider2->GetColliderPos().y);

	// (円1の半径+円2の半径)の2乗
	float rSum = (pCollider1->GetColliderSize().x / 2 + pCollider2->GetColliderSize().x / 2);
	rSum *= rSum;

	// 2つの円の距離が半径の合計を下回った時
	if (distance <= rSum)
	{
		// 当たっている
		return true;
	}

	// 当たっていない
	return false;
}

void Collision::DebugCheckColliderDraw(Collider* pCollider)
{
	static int g_DebugColliderTexId = Texture_Load(L"resource/texture/collision.png");

	Sprite_ScrollDraw(
		g_DebugColliderTexId,
		pCollider->GetColliderPos().x, pCollider->GetColliderPos().y,
		pCollider->GetColliderSize().x, pCollider->GetColliderSize().y,
		0.0f,0.0f,128.0f,128.0f,
		0.0f,
		{ 1.0f, 1.0f, 1.0f, 0.5f }
	);
}