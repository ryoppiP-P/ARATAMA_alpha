//==============================================================================
//
//   イージング管理 [easing.h]
// Author : Yumito Yokozuka
// Date   : 2025/09/21
//------------------------------------------------------------------------------
//
//==============================================================================
#ifndef EASING_H
#define EASING_H

#include <cmath>
#include <algorithm>

#define M_PI (3.14159265358979323846)

double easeOutElastic(double t);
double easeOutExpo(double t); 
double easeOutQuad(double t);
double easeOutBounce(double t);
double easeInOutBounce(double t);


#endif // EASING_H