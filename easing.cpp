//==============================================================================
//
//   イージング管理 [easing.cpp]
// Author : Yumito Yokozuka
// Date   : 2025/09/21
//------------------------------------------------------------------------------
//
//==============================================================================
#include "easing.h"

double easeOutElastic(double t)
{
	const double c4 = (2.0 * M_PI) / 3.0;

	if (t == 0.0)
		return 0.0;
	else if (t == 1.0)
		return 1.0;
	else
		return std::pow(2.0, -10.0 * t) * std::sin((t * 10.0 - 0.75) * c4) + 1.0;
}

double easeOutExpo(double t)
{
	return 1 - pow(2, -8 * t);
}

double easeOutQuad(double t)
{
	return t * (2 - t);
}

double easeOutBounce(double t)
{
	return 1 - pow(2, -6 * t) * abs(cos(t * 3.14159 * 3.5));
}

double easeInOutBounce(double t)
{
	if (t < 0.5)
	{
		return 8 * pow(2, 8 * (t - 1)) * abs(sin(t * 3.14159 * 7));
	}
	else
	{
		return 1 - 8 * pow(2, -8 * (t - 1)) * abs(sin(t * 3.14159 * 7));
	}
}
