// 定义光线

#pragma once
#include "Vector.hpp"

// 光线
class Ray
{
public:
    Vec3 orig, dir; // 源点坐标 方向向量 ray = orig + t * dir
public:
    Ray(const Vec3 &orig, const Vec3 &dir) : orig(orig), dir(dir) {}
};