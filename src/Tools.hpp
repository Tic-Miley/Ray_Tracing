// 定义常用的常量 内联函数

#pragma once
#include <limits>

#define PI 3.141592653589793

const float MAXf = std::numeric_limits<float>::max();

// 角度转弧度
inline float rad(const float &deg) { return deg * PI / 180; }

// 伽马矫正
inline void gammaCorrect(Vec3 &color)
{
    color.x = pow(color.x, 1 / 2.2f);
    color.y = pow(color.y, 1 / 2.2f);
    color.z = pow(color.z, 1 / 2.2f);
}