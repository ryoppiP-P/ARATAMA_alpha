//==============================================================================
//
//   敵クラス [game_enemy.h]
// Author : Sumi Rintarou
// Date   : 2025/11/07
//------------------------------------------------------------------------------
//
//==============================================================================
#ifndef ENEMY_H
#define ENEMY_H

#include <DirectXMath.h>
#include "game_object.h"
#include "game_player.h"

class Enemy : public GameObject
{
public:
	enum Enemy_State
	{
		ENEMY_STATE_NONE = 0,
		ENEMY_STATE_IDLE,	//未入力
		ENEMY_STATE_WALK,	//歩く
		ENEMY_STATE_JUMP,	//ジャンプ
		ENEMY_STATE_FALL	//落ちる
	};

	enum class Enemy_Type
	{
		NONE = 0,
		KATANA,
		BOW,
		KANABO,
	};

private:
	static constexpr float GRAVITY{ 9.8f * 0.1f };
	static constexpr float WALK_SPEED{ 10.0f };
	static constexpr float JUMP_POWER{ -25.0f };
	static constexpr float FRICTION_FORCE{ 0.95f };

private:
	DirectX::XMFLOAT2 m_vel{};					//加速
	DirectX::XMFLOAT4 m_color{};				//色
	bool m_isGround{};							//接地フラグ
	Enemy_State m_state{ ENEMY_STATE_NONE };	//敵の状態
	Enemy_Type m_type{ Enemy_Type::NONE };		//敵の種類
	DirectX::XMFLOAT2 m_playerPos{};			//プレイヤー座標

public:
	Enemy(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size, ObjectTag tag = ObjectTag::NONE, Enemy_Type type = Enemy_Type::NONE )
		:GameObject(position, size, tag ),m_type(type){
	}
	~Enemy() = default;

	void Initialize() override {};
	void Finalize() override {};
	void Update(double elapsed_time) override {};
	void Draw() override {};

	// 当たり判定が起きたときに呼ばれる
	void OnCollision(const GameObject* otherObject, const Collider* myCollider, const Collider* otherCollider, COLLISION_VEC hitVec) override {};

	void SetVel(DirectX::XMFLOAT2 vel) { m_vel = vel;}
	void SetColor(DirectX::XMFLOAT4 color) { m_color = color; }
	void SetIsGround(bool isground) { m_isGround = isground; }
	void SetState(Enemy_State state) { m_state = state; }
	void SetPlayerPos(DirectX::XMFLOAT2 playerPos) { m_playerPos = playerPos; }

	float GetGravity() { return GRAVITY; }
	float GetWalkSpeed() { return WALK_SPEED; }
	float GetJumpPower() { return JUMP_POWER; }
	float GetFrictionForce() { return FRICTION_FORCE; }
	DirectX::XMFLOAT2 GetVel() { return m_vel; }
	DirectX::XMFLOAT4 GetColor() { return m_color; }
	bool GetIsGround() { return m_isGround; }
	Enemy_Type GetType() { return m_type; }
	Enemy_State GetState() { return m_state; }
	DirectX::XMFLOAT2 GetPlayerPos() const { return m_playerPos; }
};

#endif // ENEMY_H

