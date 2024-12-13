// 定义球 判断球与光线相交

#pragma once
#include "Vector.hpp"
#include "Ray.hpp"

// 球
class Sphere
{
public:
    Vec3 center;  // 球心坐标
    float radius; // 半径
public:
    Sphere(const Vec3 &center, float radius) : center(center), radius(radius) {}

    // 方程代入求解是否相交 通过参数返回较近的交点对应的光线时间 t
    bool intersect(const Ray &r, float &t) const
    {
        Vec3 dir = r.dir.normalize(); // 因默认 dir^2 = 0, 故 ray.dir 需要单位化
        Vec3 oc = r.orig - center;
        float b = 2 * oc * dir;
        float c = oc * oc - radius * radius;

        float discriminant = b * b - 4 * c;
        if (discriminant < 0)
            return false;
        else
        {
            discriminant = sqrt(discriminant);
            float t0 = -b - discriminant;
            float t1 = -b + discriminant;
            t = (t0 < t1) ? t0 : t1;
            return true;
        }
    }
};