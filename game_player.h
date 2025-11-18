//==============================================================================
//
//   プレイヤークラス [game_player.h]
// Author : Yumito Yokozuka
// Date   : 2025/11/07
//------------------------------------------------------------------------------
//
//==============================================================================
#ifndef PLAYER_H
#define PLAYER_H

#include <DirectXMath.h>
#include "game_object.h"


class Player : public GameObject
{
private:
	static constexpr float GRAVITY{ 9.8f * 0.1f };
	static constexpr float WALK_SPEED{ 20.0f };
	static constexpr float JUMP_POWER{ -25.0f };
	static constexpr float FRICTION_FORCE{ 0.95f };

	enum Player_State
	{
		PLAYER_STATE_NONE = 0,
		PLAYER_STATE_IDLE,	//未入力
		PLAYER_STATE_WALK,	//歩く
		PLAYER_STATE_JUMP,	//ジャンプ
		PLAYER_STATE_FALL,	//落ちる
		PLAYER_STATE_ATK,	//攻撃
	};

	enum class Player_Weapon
	{
		NONE = 0,
		KATANA,
		BOW,
		KANABO,
	};

private:
	DirectX::XMFLOAT2 m_vel{};		//加速
	DirectX::XMFLOAT4 m_color{};	//色
	Player_State m_state{};	//プレイヤーの状態
	Player_Weapon m_weapon{};		//プレイヤーの現在の武器
	bool m_isJump{};		//ジャンプフラグ
	bool m_canDoubleJump{}; //ダブルジャンプフラグ
	bool m_isGround{};		//接地フラグ
	int m_textureId{};		//テクスチャID

	Collider* m_bodyCollider{ nullptr }; // 体の当たり判定
	Collider* m_attackCollider{ nullptr }; // 体の当たり判定
	DirectX::XMFLOAT2 m_colliderOffset{};


public:
	Player(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size);
	~Player() = default;

	void Initialize() override;
	void Finalize() override;
	void Update(double elapsed_time) override;
	void Draw() override;

	// 当たり判定が起きたときに呼ばれる
	void OnCollision(const GameObject* otherObject, const Collider* myCollider, const Collider* otherCollider, COLLISION_VEC hitVec) override;

private:
	void PlayerIdle();
	void PlayerWalk();
	void PlayerRun();
	void PlayerJump();
	void PlayerFall();

	class Weapon
	{
	};

	void PlayerAtk();
	void PlayerAtkKatana();
	void PlayerAtkBow();
	void PlayerAtkKanabo();
	void PlayerSetWeapon(Player_Weapon weapon);

	// コライダーの位置更新
	void UpdateColliderPosition();
};

#endif // PLAYER_H

