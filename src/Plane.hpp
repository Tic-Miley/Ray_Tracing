// 定义平面（具有长宽）

#pragma once
#include "Object.hpp"

class Plane : public Object
{
public:
    Vec3 head = (0, 0, 0);  // 平面的左上角点坐标
    Vec3 right = (1, 0, 0); // 平面的右方向
    Vec3 down = (0, 1, 0);  // 平面的下方向
    float width = 10, length = 10;

public:
    Plane(const Vec3 &head, const Vec3 &right, const Vec3 &down, const float width, const float length, const Vec3 &color)
        : head(head), right(right), down(down), width(width), length(length), Object(color) {}

    Vec3 getNormalVector(const Vec3 &point = Vec3(0, 0, 0)) const { return down.cross(right); } // 规定向上为正面
    bool intersect(const Ray &r, float &t) const
    {
        Vec3 normalVector = getNormalVector();
        // 光线与平面相交
        t = (head - r.orig) * normalVector / (r.dir * normalVector);
        // 交点是否在区域内
        Vec3 point = r.orig + t * r.dir;
        Vec3 h_p = point - head;

        // 默认 right 和 down 正交且长度为1
        if (h_p * right >= 0 && h_p * right <= width && h_p * down >= 0 && h_p * down <= length)
            return true;
        else
            return false;
    }
};
