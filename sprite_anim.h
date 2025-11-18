//======================================================================================
//	スプライトアニメーションの描画 [sprite_anim.h]
//	Author : yumito yokozuka
//	Date   : 2025/6/13
//--------------------------------------------------------------------------------------
//
//======================================================================================
#ifndef SPRITE_ANIM_H
#define SPRITE_ANIM_H

#include <DirectXMath.h>

class AnimPattern
{
private:
	int m_texId;
	int m_patternMax;
	int m_horizontalPatternMax;
	double m_secondPerPattern;
	DirectX::XMUINT2 m_startPosition;
	DirectX::XMUINT2 m_patternSize;
	bool m_isLoop;

public:
	AnimPattern() = delete;
	AnimPattern(int texId, int patternMax,int horizontalPatternMax, double secondPerPattern, DirectX::XMUINT2 startPosition, DirectX::XMUINT2 patternSize, bool isLoop = true)
		:m_texId(texId), m_patternMax(patternMax),m_horizontalPatternMax(horizontalPatternMax), m_secondPerPattern(secondPerPattern), m_startPosition(startPosition), m_patternSize(patternSize),m_isLoop(isLoop)
	{
	}

	int GetPattrenMax() { return m_patternMax; }
	double GetSecondPerPattern() { return m_secondPerPattern; }
	bool IsLoop() { return m_isLoop; }

	void Draw(float dx, float dy, float dw, float dh, int patternNum, DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });
};

class AnimPatternPlayer
{
private:
	int m_pattern = 0; //パターン番号
	double m_accumulatedTime = 0.0; //累積時間
	AnimPattern* m_pAnimPattern = nullptr;

public:
	AnimPatternPlayer() = delete;
	AnimPatternPlayer(AnimPattern* pAnimPattern)
		:m_pAnimPattern(pAnimPattern) {
	}

	void Update(double elapsed_time);
	void Draw(float dx, float dy, float dw, float dh, DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });
};

#endif //SPRITE_ANIM_H