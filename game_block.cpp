//==============================================================================
//
//   ブロッククラス [game_block.cpp]
// Author : Yumito Yokozuka
// Date   : 2025/11/07
//------------------------------------------------------------------------------
//
//==============================================================================
#include "game_block.h"
#include "sprite.h"
#include "texture.h"
using namespace DirectX;


int Block::m_BlockTexId = -1;

Block::Block(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size)
	:GameObject(position, size, ObjectTag::BLOCK)
{
	AddCollider(GetPosition(), GetSize(), ColliderType::BODY);
	m_bodyCollider = &GetColliders().back();
}

void Block::Initialize()
{
	m_BlockTexId = Texture_Load(L"resource/texture/white.png");
}

void Block::Finalize()
{
}

void Block::Update(double elapsed_time)
{
	UpdateColliderPosition();
}

void Block::Draw()
{
	if (IsUse())
	{
		Sprite_ScrollDraw(m_BlockTexId, GetPosition().x, GetPosition().y, GetSize().x, GetSize().y);
	}
}

void Block::OnCollision(const GameObject* otherObject, const Collider* myCollider, const Collider* otherCollider, COLLISION_VEC hitVec)
{
}

//============================================================
// コライダー追従
//============================================================
void Block::UpdateColliderPosition()
{
	if (m_bodyCollider)
	{
		m_bodyCollider->SetColliderPos(GetPosition());
	}
}