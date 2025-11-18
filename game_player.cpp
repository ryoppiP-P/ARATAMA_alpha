//==============================================================================
//
//   プレイヤークラス [game_player.cpp]
// Author : Yumito Yokozuka
// Date   : 2025/11/07
//------------------------------------------------------------------------------
//
//==============================================================================
#include "game.h"
#include "game_player.h"
#include "texture.h"
#include "sprite.h"
#include "key_logger.h"
#include "mouse.h"

Mouse_State g_mouse{};

static XMFLOAT2 dir{};
static float g_add = 0.0f;
static bool g_atk = false;
static bool g_isleft = false;

Player::Player(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size)
	: GameObject(position, size, ObjectTag::PLAYER)
{
	//当たり判定の分だけ事前に容量確保
	GetColliders().reserve(2);

	// BODY用の当たり判定を生成
	AddCollider({ GetPosition().x + 20.0f, GetPosition().y }, { 60.0f,80.0f }, ColliderType::BODY);
	m_bodyCollider = &GetColliders().back();

	// ATTACK用の当たり判定を生成
	AddCollider(GetPosition(), GetSize(), ColliderType::ATTACK);
	m_attackCollider = &GetColliders().back();

	
	m_state = PLAYER_STATE_FALL;
	m_weapon = Player_Weapon::KATANA;
	m_isJump = false;
	m_canDoubleJump = true;
	m_isGround = false;
	m_attackCollider->SetActive(false);
}

void Player::Initialize()
{
	m_textureId = Texture_Load(L"resource/texture/Katana.png");

	m_state = PLAYER_STATE_FALL;
	PlayerSetWeapon(Player_Weapon::KATANA);
	m_isJump = false;
	m_isGround = false;
	m_attackCollider->SetActive(false);
}

void Player::Finalize()
{
}

void Player::Update(double elapsed_time)
{
	Mouse_GetState(&g_mouse);

	switch (m_state)
	{
	case PLAYER_STATE_NONE:
		break;
	case PLAYER_STATE_IDLE:
		PlayerIdle();
		break;
	case PLAYER_STATE_WALK:
		PlayerWalk();
		break;
	case PLAYER_STATE_JUMP:
		PlayerJump();
		break;
	case PLAYER_STATE_FALL:
		PlayerFall();
		break;
	case PLAYER_STATE_ATK:
		switch (m_weapon)
		{
		case Player_Weapon::KATANA:
			PlayerAtkKatana();
			break;
		case Player_Weapon::BOW:
			PlayerAtkBow();
			break;
		case Player_Weapon::KANABO:
			PlayerAtkKanabo();
			break;
		default:
			break;
		}

		break;
	default:
		break;
	}

	if (!m_isGround && m_state != PLAYER_STATE_JUMP && m_state != PLAYER_STATE_FALL && m_state != PLAYER_STATE_ATK)
	{
		m_isJump = true;
		m_state = PLAYER_STATE_FALL;
	}

	if (KeyLogger_IsTrigger(KK_I))
	{
		PlayerSetWeapon(Player_Weapon::KATANA);
	}
	if (KeyLogger_IsTrigger(KK_O))
	{
		PlayerSetWeapon(Player_Weapon::BOW);
	}
	if (KeyLogger_IsTrigger(KK_P))
	{
		PlayerSetWeapon(Player_Weapon::KANABO);
	}

	m_isGround = false;

	auto pos = GetPosition();
	pos.x += m_vel.x;
	pos.y += m_vel.y;
	SetPosition(pos);

	UpdateColliderPosition();
}

void Player::Draw()
{
	if (IsUse())
	{
		Sprite_ScrollDraw(m_textureId, GetPosition().x, GetPosition().y, GetSize().x, GetSize().y, { 0.3f,0.3f,0.7f,1.0f });
	}
}

//============================================================
// 当たり判定時の処理
//============================================================
void Player::OnCollision(const GameObject* otherObject, const Collider* myCollider, const Collider* otherCollider,COLLISION_VEC hitVec)
{
	if (!myCollider || !otherCollider|| !otherObject) return;

	ColliderType myType = myCollider->GetType();
	ColliderType otherType = otherCollider->GetType();

	//ブロック
	if (myType == ColliderType::BODY &&
		otherType == ColliderType::BODY &&
		otherObject->GetTag() == ObjectTag::BLOCK)
	{
		if (m_state == PLAYER_STATE_FALL)
		{
			if (hitVec == COLLISION_VEC::BOTTOM)
			{
				m_vel.y = 0.0f;
				m_isJump = false;
				m_canDoubleJump = true;
				m_isGround = true;
				auto pos = GetPosition();
				pos.y = otherObject->GetPosition().y - GetSize().y;
				SetPosition(pos);
				m_state = PLAYER_STATE_IDLE;
			}
		}

		if (hitVec == COLLISION_VEC::TOP)
		{
			m_vel.y = 0.0f;
			auto pos = GetPosition();
			pos.y = otherObject->GetPosition().y + otherObject->GetSize().y;
			SetPosition(pos);
		}

		if (hitVec == COLLISION_VEC::LEFT)
		{
			m_vel.x = 0.0f;
			auto pos = GetPosition();
			//pos.x = otherObject->GetPosition().x + otherObject->GetSize().x;
			pos.x = otherObject->GetPosition().x + otherObject->GetSize().x - 20.0f;
			SetPosition(pos);
		}
		if (hitVec == COLLISION_VEC::RIGHT)
		{
			m_vel.x = 0.0f;
			auto pos = GetPosition();
			//pos.x = otherObject->GetPosition().x - GetSize().x;
			pos.x = otherObject->GetPosition().x - m_bodyCollider->GetColliderSize().x - 20.0f;
			SetPosition(pos);
		}
	}

	//攻撃
	if (myType == ColliderType::ATTACK &&
		otherType == ColliderType::BODY &&
		otherObject->GetTag() == ObjectTag::ENEMY_KATANA)
	{
		if (m_weapon == Player_Weapon::KANABO)
		{
			if (g_isleft)
			{
				m_vel = { -60.0f, -25.0f };
			}
			if (!g_isleft)
			{
				m_vel = { 60.0f, -25.0f };
			}
		}
		else
		{
			m_vel = { 0.0f, 0.0f };
		}

		PlayerSetWeapon(Player_Weapon::KATANA);

		// 敵の位置にテレポート処理
		SetPosition(otherObject->GetPosition());

		//game_objectにvel追加
		//m_vel = (otherObject->GetPosition());
		g_add = 0.0f;
		m_state = PLAYER_STATE_WALK;
		m_attackCollider->SetActive(false);
	}

	//攻撃
	if (myType == ColliderType::ATTACK &&
		otherType == ColliderType::BODY &&
		otherObject->GetTag() == ObjectTag::ENEMY_BOW)
	{
		if (m_weapon == Player_Weapon::KANABO)
		{
			if (g_isleft)
			{
				m_vel = { -60.0f, -25.0f };
			}
			if (!g_isleft)
			{
				m_vel = { 60.0f, -25.0f };
			}
		}
		else
		{
			m_vel = { 0.0f, 0.0f };
		}

		PlayerSetWeapon(Player_Weapon::BOW);

		// 敵の位置にテレポート処理
		SetPosition(otherObject->GetPosition());

		//game_objectにvel追加
		//m_vel = (otherObject->GetPosition());
		g_add = 0.0f;
		m_state = PLAYER_STATE_WALK;
		m_attackCollider->SetActive(false);
	}

	//攻撃
	if (myType == ColliderType::ATTACK &&
		otherType == ColliderType::BODY &&
		otherObject->GetTag() == ObjectTag::ENEMY_KANABO)
	{
		if (m_weapon == Player_Weapon::KANABO)
		{
			if (g_isleft)
			{
				m_vel = { -60.0f, -25.0f };
			}
			if (!g_isleft)
			{
				m_vel = { 60.0f, -25.0f };
			}
		}
		else
		{
			m_vel = { 0.0f, 0.0f };
		}

		PlayerSetWeapon(Player_Weapon::KANABO);

		// 敵の位置にテレポート処理
		SetPosition(otherObject->GetPosition());

		//game_objectにvel追加
		//m_vel = (otherObject->GetPosition());
		m_state = PLAYER_STATE_WALK;
		m_attackCollider->SetActive(false);
	}

	//敵の攻撃
	if (myType == ColliderType::BODY &&
		otherType == ColliderType::ATTACK &&
		otherObject->GetTag() == ObjectTag::ENEMY_KATANA)
	{
		// ダメージ処理
	}
}

//============================================================
//プレイヤーの状態
//============================================================
//------------------------------------------------------------
//　停止
//------------------------------------------------------------
void Player::PlayerIdle()
{
	m_vel.x *= FRICTION_FORCE * FRICTION_FORCE * FRICTION_FORCE;
	if (m_vel.x < 0.01 && m_vel.x > -0.01)
	{
		m_vel.x = 0.0f;
	}
	m_vel.y = 0.0f;

	if (KeyLogger_IsPressed(KK_A) || KeyLogger_IsPressed(KK_D))
	{
		m_state = PLAYER_STATE_WALK;
	}

	if (KeyLogger_IsTrigger(KK_SPACE))
	{
		m_state = PLAYER_STATE_JUMP;
		m_vel.y = JUMP_POWER;
		m_isJump = true;
	}

	//攻撃----------------------------------------
	PlayerAtk();
	//--------------------------------------------
}

//------------------------------------------------------------
//　歩く
//------------------------------------------------------------
void Player::PlayerWalk()
{
	if (KeyLogger_IsPressed(KK_A))
	{
		m_vel.x = -WALK_SPEED;
	}
	else if (KeyLogger_IsPressed(KK_D))
	{
		m_vel.x = WALK_SPEED;
	}
	else
	{
		m_state = PLAYER_STATE_IDLE;
	}

	if (KeyLogger_IsTrigger(KK_SPACE))
	{
		m_state = PLAYER_STATE_JUMP;
		m_vel.y = JUMP_POWER;
		m_isJump = true;
	}

	//攻撃----------------------------------------
	PlayerAtk();
	//--------------------------------------------
}

//------------------------------------------------------------
//　走る
//------------------------------------------------------------
void Player::PlayerRun()
{
}

//------------------------------------------------------------
//　ジャンプ
//------------------------------------------------------------
void Player::PlayerJump()
{
	m_vel.x *= FRICTION_FORCE;
	if (m_vel.x < 0.01 && m_vel.x > -0.01)
	{
		m_vel.x = 0.0f;
	}

	m_vel.y += GRAVITY;

	if (m_vel.y >= 0.0f)
	{
		m_state = PLAYER_STATE_FALL;
	}

	if (KeyLogger_IsPressed(KK_A))
	{
		m_vel.x = -WALK_SPEED;
	}
	if (KeyLogger_IsPressed(KK_D))
	{
		m_vel.x = WALK_SPEED; 
	}

	if (KeyLogger_IsTrigger(KK_SPACE))
	{
		if (m_isJump && m_canDoubleJump)
		{
			m_state = PLAYER_STATE_JUMP;
			m_vel.y = JUMP_POWER;
			m_canDoubleJump = false;
		}
	}

	//攻撃----------------------------------------
	PlayerAtk();
	//--------------------------------------------
}

//------------------------------------------------------------
//　落下
//------------------------------------------------------------
void Player::PlayerFall()
{
	m_vel.x *= FRICTION_FORCE;
	if (m_vel.x < 0.01 && m_vel.x > -0.01)
	{
		m_vel.x = 0.0f;
	}

	m_vel.y += GRAVITY;

	//速度上限
	if (m_vel.y >= 30.0f)
	{
		m_vel.y = 30.0f;
	}

	if (KeyLogger_IsPressed(KK_A))
	{
		m_vel.x = -WALK_SPEED;
	}
	if (KeyLogger_IsPressed(KK_D))
	{
		m_vel.x = WALK_SPEED;
	}

	if (KeyLogger_IsTrigger(KK_SPACE))
	{
		if (m_isJump && m_canDoubleJump)
		{
			m_state = PLAYER_STATE_JUMP;
			m_vel.y = JUMP_POWER;
			m_canDoubleJump = false;
		}
	}

	//攻撃----------------------------------------
	PlayerAtk();
	//--------------------------------------------

	if (GetPosition().y > 350)
	{
		m_isJump = false;
		m_canDoubleJump = true;
		SetPosition({ GetPosition().x, 350.0f });
		m_state = PLAYER_STATE_WALK;
	}
}

//============================================================
// 攻撃
//============================================================
void Player::PlayerAtk()
{
	if (g_mouse.leftButton && !g_atk)
	{
		m_attackCollider->SetActive(true);
		g_atk = true;
		m_state = PLAYER_STATE_ATK;
		if (m_weapon == Player_Weapon::KATANA)
		{
			if (g_mouse.x < SCREEN_WIDTH / 2)
			{
				g_isleft = true;
				m_vel.x = -100.0f;
			}
			else
			{
				g_isleft = false;
				m_vel.x = 100.0f;
			}
		}
		if (m_weapon == Player_Weapon::BOW)
		{
			m_attackCollider->SetColliderPos(GetPosition());
		}
		if (m_weapon == Player_Weapon::KANABO)
		{
			if (g_mouse.x < SCREEN_WIDTH / 2)
			{
				g_isleft = true;
			}
			else
			{
				g_isleft = false;
			}
		}
	}
	if (!g_mouse.leftButton && g_atk)
	{
		g_atk = false;
	}
}

void Player::PlayerAtkKatana()
{
	m_vel.x *= FRICTION_FORCE * FRICTION_FORCE * FRICTION_FORCE;
	if (m_vel.x < 5.0f && m_vel.x > -5.0f)
	{
		m_vel.x = 0.0f;
	}
	m_vel.y = 0.0f;

	if (m_vel.x == 0)
	{
		m_attackCollider->SetActive(false);
		m_state = PLAYER_STATE_WALK;
	}
}

void Player::PlayerAtkBow()
{
	m_vel = { 0.0f,0.0f };
	float bulletSpeed = 50.0f;
	XMFLOAT2 cPos = m_attackCollider->GetColliderPos();
	dir = { static_cast<float>(g_mouse.x - SCREEN_WIDTH / 2),static_cast<float>(g_mouse.y - SCREEN_HEIGHT / 2) };
	float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
	if (len != 0)
	{
		dir.x /= len;
		dir.y /= len;
	}
	g_add += bulletSpeed;
	cPos.x += dir.x * bulletSpeed;
	cPos.y += dir.y * bulletSpeed;
	m_attackCollider->SetColliderPos(cPos);

	if (g_add > 1000.0f)
	{
		g_add = 0.0f;
		m_attackCollider->SetActive(false);
		m_state = PLAYER_STATE_WALK;
	}
}

void Player::PlayerAtkKanabo()
{
	m_attackCollider->SetActive(false);
	m_state = PLAYER_STATE_WALK;
}

void Player::PlayerSetWeapon(Player_Weapon weapon)
{
	switch (weapon)
	{
	case Player_Weapon::KATANA:
		m_textureId = Texture_Load(L"resource/texture/Katana.png");
		m_colliderOffset = {85.0f,10.0f};
		m_attackCollider->SetColliderSize({50.0f,60.0f});
		break;
	case Player_Weapon::BOW:
		m_textureId = Texture_Load(L"resource/texture/Bow.png");
		m_colliderOffset = { 85.0f,-5.0f };
		m_attackCollider->SetColliderSize({ 40.0f,40.0f });
		break;
	case Player_Weapon::KANABO:
		m_textureId = Texture_Load(L"resource/texture/Konbo.png");
		m_colliderOffset = { 85.0f,-20.0f };
		m_attackCollider->SetColliderSize({ 70.0f,80.0f });
		break;
	default:
		break;
	}
	m_weapon = weapon;
}

//============================================================
// コライダー追従
//============================================================
void Player::UpdateColliderPosition()
{
	if (m_bodyCollider)
	{
		m_bodyCollider->SetColliderPos({ GetPosition().x + 20.0f, GetPosition().y });
	}
	if (m_weapon == Player_Weapon::KATANA|| m_weapon == Player_Weapon::KANABO)
	{
		if (m_attackCollider && !g_isleft)
		{
			m_attackCollider->SetColliderPos({ GetPosition().x + m_colliderOffset.x, GetPosition().y + m_colliderOffset.y });
		}
		else
		{
			m_attackCollider->SetColliderPos({ GetPosition().x - m_colliderOffset.x / 2, GetPosition().y + m_colliderOffset.y });
		}
	}
	if (m_weapon == Player_Weapon::BOW)
	{
	}
}
