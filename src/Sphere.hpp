// 定义球 判断球与光线相交

#pragma once
#include "Object.hpp"

// 球
class Sphere : public Object
{
public:
    Vec3 center;  // 球心坐标
    float radius; // 半径
    // Vec3 color;   // 颜色（暂时支持纯色）
    // float reflectivity; // 反射率
public:
    // Sphere(const Vec3 &center, float radius) : center(center), radius(radius) {}
    Sphere(const Vec3 &center, float radius, const Vec3 &color)
        : center(center), radius(radius), Object(color) {}

    // 获取某点法线
    Vec3 getNormalVector(const Vec3 &point) const { return (point - center).normalize(); }

    // 方程代入求解是否相交 通过参数返回较近的交点对应的光线时间 t
    bool intersect(const Ray &r, float &t) const
    {
        Vec3 dir = r.dir.normalize(); // 因默认 dir^2 = 0, 故 ray.dir 需要单位化 // 保护措施 外部一般已经初始化
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
            t = t / 2; // 结果除以二
            return true;
        }
    }
};