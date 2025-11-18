#include "game_object.h"
#include "sprite.h"
#include "texture.h"
#include "game_enemy_kanabo.h"

int EnemyKanabo::m_textureId = -1;

EnemyKanabo::EnemyKanabo(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size)
	: Enemy(position, size, ObjectTag::ENEMY_KANABO, Enemy_Type::KANABO)
{
	//当たり判定の分だけ事前に容量確保
	GetColliders().reserve(2);

	// BODY用の当たり判定を生成
	AddCollider({ GetPosition().x + 20.0f, GetPosition().y }, { 60.0f,80.0f }, ColliderType::BODY);
	m_bodyCollider = &GetColliders().back();

	// ATTACK用の当たり判定を生成
	AddCollider(GetPosition(), { 0.0f,0.0f }, ColliderType::ATTACK);
	m_attackCollider = &GetColliders().back();
	m_attackCollider->SetActive(false);

	SetIsGround(false);
	SetState(ENEMY_STATE_IDLE);
	m_colliderOffset = { 85.0f,-20.0f };

	m_spawnPosition = position; // 初期位置を記録
}

void EnemyKanabo::Initialize()
{
	m_textureId = Texture_Load(L"resource/texture/Konbo.png");
	SetIsGround(false);
	SetState(ENEMY_STATE_IDLE);
	m_attackCollider->SetActive(false);
}

void EnemyKanabo::Finalize()
{
}

void EnemyKanabo::Update(double elapsed_time)
{
	// ===== 死亡中の処理 =====
	if (m_isDead)
	{
		m_deathTimer += elapsed_time;
		if (m_deathTimer >= m_respawnTime)
		{
			EnemyRespawn(); // 復活
		}
		return; // 死亡中は他の行動をしない
	}


	switch (GetState())
	{
	case ENEMY_STATE_NONE:
		break;
	case ENEMY_STATE_IDLE:
		EnemyIdle();
		break;
	case ENEMY_STATE_WALK:
		EnemyWalk();
		break;
	case ENEMY_STATE_JUMP:
		EnemyJump();
		break;
	case ENEMY_STATE_FALL:
		EnemyFall();
		break;
	default:
		break;
	}

	if (!GetIsGround() && GetState() != ENEMY_STATE_JUMP && GetState() != ENEMY_STATE_FALL)
	{
		SetState(ENEMY_STATE_FALL);
	}

	SetIsGround(false);

	auto pos = GetPosition();
	pos.x += GetVel().x;
	pos.y += GetVel().y;
	SetPosition(pos);

	UpdateColliderPosition();
}

void EnemyKanabo::Draw()
{
	if (IsUse())
	{
		// 死んでいるとき以外は描画
		if (!m_isDead)
		{
			Sprite_ScrollDraw(m_textureId, GetPosition().x, GetPosition().y, GetSize().x, GetSize().y, { 10.0f,0.0f,0.0f,1.0f });
		}
	}
}

void EnemyKanabo::OnCollision(const GameObject* otherObject, const Collider* myCollider, const Collider* otherCollider, COLLISION_VEC hitVec)
{
	if (!myCollider || !otherCollider || !otherObject) return;

	ColliderType myType = myCollider->GetType();
	ColliderType otherType = otherCollider->GetType();

	//ブロック
	if (myType == ColliderType::BODY &&
		otherType == ColliderType::BODY &&
		otherObject->GetTag() == ObjectTag::BLOCK)
	{
		if (GetState() == ENEMY_STATE_FALL)
		{
			if (hitVec == COLLISION_VEC::BOTTOM)
			{
				SetVel({ GetVel().x,0.0f });
				SetIsGround(true);
				auto pos = GetPosition();
				pos.y = otherObject->GetPosition().y - GetSize().y;
				SetPosition(pos);
				SetState(ENEMY_STATE_IDLE);
			}
		}

		if (hitVec == COLLISION_VEC::TOP)
		{
			SetVel({ GetVel().x,0.0f });
			auto pos = GetPosition();
			pos.y = otherObject->GetPosition().y + otherObject->GetSize().y;
			SetPosition(pos);
		}

		if (hitVec == COLLISION_VEC::LEFT)
		{
			SetVel({ 0.0f,GetVel().y });
			auto pos = GetPosition();
			//pos.x = otherObject->GetPosition().x + otherObject->GetSize().x;
			pos.x = otherObject->GetPosition().x + otherObject->GetSize().x - 20.0f;
			SetPosition(pos);
		}
		if (hitVec == COLLISION_VEC::RIGHT)
		{
			SetVel({ 0.0f,GetVel().y });
			auto pos = GetPosition();
			//pos.x = otherObject->GetPosition().x - GetSize().x;
			pos.x = otherObject->GetPosition().x - m_bodyCollider->GetColliderSize().x - 20.0f;
			SetPosition(pos);
		}
	}

	//攻撃
	if (myType == ColliderType::ATTACK &&
		otherType == ColliderType::BODY &&
		otherObject->GetTag() == ObjectTag::PLAYER)
	{
		// 敵の位置にテレポート処理
	}

	// プレイヤーの攻撃
	if (myType == ColliderType::BODY &&
		otherType == ColliderType::ATTACK &&
		otherObject->GetTag() == ObjectTag::PLAYER)
	{
		// ダメージ処理(現在は当たった瞬間に死ぬようにしている)
		EnemyDie();
	}
}

void EnemyKanabo::EnemyIdle()
{
	// プレイヤーとの距離を計算
	DirectX::XMFLOAT2 playerPos = GetPlayerPos();
	DirectX::XMFLOAT2 enemyPos = GetPosition();
	float dx = playerPos.x - enemyPos.x;
	float dy = playerPos.y - enemyPos.y;
	float distance = sqrtf(dx * dx + dy * dy);

	// ===== 状態遷移 =====
	if (distance < 500.0f)// 感知距離
	{
		SetState(ENEMY_STATE_WALK);
		return;
	}

	// ===== 静止処理 =====
	DirectX::XMFLOAT2 vel = GetVel();
	vel.x *= 0.8f; // 少し減速
	if (fabs(vel.x) < 0.05f)
	{
		vel.x = 0.0f;

		vel.y = 0.0f; // Idle中は上下移動しない
		SetVel(vel);
	}
}

void EnemyKanabo::EnemyWalk()
{
	DirectX::XMFLOAT2 playerPos = GetPlayerPos();
	DirectX::XMFLOAT2 enemyPos = GetPosition();

	float dx = playerPos.x - enemyPos.x;
	float dy = playerPos.y - enemyPos.y;
	float distance = sqrtf(dx * dx + dy * dy);

	const float chaseRange = 500.0f;// 感知距離(プレイヤーの座標との)
	const float stopRange = 100.0f;	// この座標まで近づいたら止まる

	// 一定距離外なら停止
	if (distance > chaseRange)
	{
		SetState(ENEMY_STATE_IDLE);
		return;
	}
	else if (distance < stopRange)
	{
		SetState(ENEMY_STATE_IDLE);
		return;
	}

	// プレイヤー方向（左：-1、右：1）
	float dir = (dx > 0) ? 1.0f : -1.0f;

	// ===== ゆるやかに加速する =====
	DirectX::XMFLOAT2 vel = GetVel();
	float targetSpeed = dir * (GetWalkSpeed() * 0.5f); // 通常速度の半分くらいに抑える
	float accelRate = 0.05f; // 加速の速さ（0.1f → すぐ最高速、0.05f → ゆっくり）

	// 線形補間でなめらかに速度変化
	vel.x = vel.x + (targetSpeed - vel.x) * accelRate;
	vel.y = 0.0f; // 歩行中はY固定
	SetVel(vel);
}

void EnemyKanabo::EnemyRun()
{
}

void EnemyKanabo::EnemyJump()
{
	SetVel({ GetVel().x * GetFrictionForce(), GetVel().y });
	if (GetVel().x < 0.01 && GetVel().x > -0.01)
	{
		SetVel({ 0.0f, GetVel().y });
	}

	SetVel({ GetVel().x ,GetVel().y + GetGravity() });

	if (GetVel().y >= 0.0f)
	{
		SetState(ENEMY_STATE_FALL);
	}
}

void EnemyKanabo::EnemyFall()
{
	SetVel({ GetVel().x * GetFrictionForce(), GetVel().y });
	if (GetVel().x < 0.01 && GetVel().x > -0.01)
	{
		SetVel({ 0.0f, GetVel().y });
	}

	SetVel({ GetVel().x ,GetVel().y + GetGravity() });

	//速度上限
	if (GetVel().y >= 30.0f)
	{
		SetVel({ GetVel().x,30.0f });
	}
}

void EnemyKanabo::EnemyAtk()
{

}

void EnemyKanabo::UpdateColliderPosition()
{
	if (m_bodyCollider)
	{
		m_bodyCollider->SetColliderPos({ GetPosition().x + 20.0f, GetPosition().y });
	}
	if (m_attackCollider)
	{
		m_attackCollider->SetColliderPos({ GetPosition().x + m_colliderOffset.x, GetPosition().y + m_colliderOffset.y });
	}
}

// 死んだとき
void EnemyKanabo::EnemyDie()
{
	if (m_isDead) return; // すでに死亡していたら何もしない

	m_isDead = true;
	m_deathTimer = 0.0;

	// アニメーションやエフェクト

	// ===== 当たり判定を無効化 =====
	if (m_bodyCollider)  m_bodyCollider->SetActive(false);
	if (m_attackCollider) m_attackCollider->SetActive(false);
}

// 復活処理
void EnemyKanabo::EnemyRespawn()
{
	m_isDead = false;
	SetPosition(m_spawnPosition); // 初期位置に戻す
	SetState(ENEMY_STATE_IDLE);
	SetIsGround(false);
	SetVel({ 0.0f, 0.0f });

	// ===== 当たり判定を再有効化 =====
	if (m_bodyCollider)  m_bodyCollider->SetActive(true);
	if (m_attackCollider) m_attackCollider->SetActive(true);
}
