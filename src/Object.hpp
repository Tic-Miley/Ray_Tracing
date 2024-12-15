// 定义物体 判断光线是否与物体相交 获取法线

#pragma once
#include "Vector.hpp"
#include "Ray.hpp"

class Object
{
public:
    Vec3 color = (0.2, 0.2, 0.2); // 物体颜色 漫反射颜色
public:
    Object(const Vec3 &color) : color(color) {}

    virtual ~Object() = default;

    // 光线与物体相交
    virtual bool intersect(const Ray &ray, float &t) const = 0;
    // 获取某点的法线
    virtual Vec3 getNormalVector(const Vec3 &point) const = 0;
};