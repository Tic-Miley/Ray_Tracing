// 定义常用的常量 内联函数

#pragma once
#include <limits>

#define PI 3.141592653589793

const float MAXf = std::numeric_limits<float>::max();

inline float rad(const float &deg) { return deg * PI / 180; }