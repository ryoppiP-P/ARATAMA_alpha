#include "sprite_anim.h"
#include "sprite.h"

void AnimPattern::Draw(float dx, float dy, float dw, float dh, int patternNum, DirectX::XMFLOAT4 color)
{
	//int pattern = patternNum % m_patternMax;

	Sprite_Draw(m_texId, dx, dy, dw, dh,
		m_startPosition.x + m_patternSize.x * (patternNum % m_horizontalPatternMax),
		m_startPosition.y + m_patternSize.y * (patternNum / m_horizontalPatternMax),
		m_patternSize.x, m_patternSize.y, color);
}

void AnimPatternPlayer::Update(double elapsed_time)
{
	m_accumulatedTime += elapsed_time;
	if (m_accumulatedTime >= m_pAnimPattern->GetSecondPerPattern())
	{
		m_pattern = (m_pattern + 1) % m_pAnimPattern->GetPattrenMax();
		m_accumulatedTime -= m_pAnimPattern->GetSecondPerPattern();
	}
}

void AnimPatternPlayer::Draw(float dx, float dy, float dw, float dh, DirectX::XMFLOAT4 color)
{
	m_pAnimPattern->Draw(dx, dy, dw, dh, m_pattern, color);
}
