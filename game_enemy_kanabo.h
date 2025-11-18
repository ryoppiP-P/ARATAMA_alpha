//==============================================================================
//
//   敵クラス [game_enemy_kanabo.h]
// Author : Sumi Rintarou
// Date   : 2025/11/07
//------------------------------------------------------------------------------
//
//==============================================================================
#ifndef ENEMY_KANABO_H
#define ENEMY_KANABO_H

#include <DirectXMath.h>
#include "game_enemy.h"

class EnemyKanabo : public Enemy
{
private:
	static int m_textureId;					// テクスチャのID
	Collider* m_bodyCollider{ nullptr };	// 体の当たり判定
	Collider* m_attackCollider{ nullptr };	// 体の当たり判定
	DirectX::XMFLOAT2 m_colliderOffset{};

	bool m_isDead = false;					// 死亡フラグ
	double m_deathTimer = 0.0;				// 死亡からの経過時間
	const double m_respawnTime = 3.0f;		// 復活までの時間（秒）
	DirectX::XMFLOAT2 m_spawnPosition;		// 初期位置（復活場所）

public:
	EnemyKanabo(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size);
	~EnemyKanabo() = default;

	void Initialize() override;
	void Finalize() override;
	void Update(double elapsed_time) override;
	void Draw() override;

	// 当たり判定が起きたときに呼ばれる
	void OnCollision(const GameObject* otherObject, const Collider* myCollider, const Collider* otherCollider, COLLISION_VEC hitVec) override;

private:
	void EnemyIdle();
	void EnemyWalk();
	void EnemyRun();
	void EnemyJump();
	void EnemyFall();

	void EnemyAtk();

	// コライダーの位置更新
	void UpdateColliderPosition();
	void EnemyDie();
	void EnemyRespawn();
};

#endif // ENEMY_KANABO_H

