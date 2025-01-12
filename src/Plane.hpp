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

    Vec3 getNormalVector(const Vec3 &point = Vec3(0, 0, 0)) const { return down.cross(right); } // 规定向上为正面 // 未作单位化
    Vec3 getCenter() const { return head + right * (width / 2) + down * (width / 2); }          // 平面中心点
    Vec3 getBound() const
    {
        Vec3 p1 = head + width * right;
        Vec3 p2 = head + length * down;
        Vec3 p3 = p1 + length * down;
        Vec3 ans;
        ans.x = std::max(std::max(p1.x, p2.x), p3.x);
        ans.y = std::max(std::max(p1.y, p2.y), p3.y);
        ans.z = std::max(std::max(p1.z, p2.z), p3.z);
        return ans;
    }
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
