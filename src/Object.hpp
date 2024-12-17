// 定义物体 判断光线是否与物体相交 获取法线 获取 BRDF

#pragma once
#include <functional>
#include "Vector.hpp"
#include "Ray.hpp"
#include "Tools.hpp"

// 材质类型 暂时支持漫反射
enum MaterialType
{
    DIFFUSE
};

class Object
{
public:
    int index = 0;                   // 物体索引
    Vec3 color = (0.2, 0.2, 0.2);    // 物体颜色 漫反射颜色
    MaterialType material = DIFFUSE; // 漫反射材质 各方向反射率相同
public:
    Object(const Vec3 &color) : color(color) {}

    virtual ~Object() = default;

    // 光线与物体相交
    virtual bool intersect(const Ray &ray, float &t) const = 0;
    // 获取某点的法线
    virtual Vec3 getNormalVector(const Vec3 &point) const = 0;
    // 获取 BRDF 返回 lambda 函数
    std::function<float()> BRDF() const
    {
        if (material == DIFFUSE)
            return []() -> float
            { return 0.6f / (2 * PI); };
        else
            return []() -> float
            { return 1.0f; };
    }
};