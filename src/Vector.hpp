// 定义向量 运算符重载

#pragma once
#include <cmath>

// 三维向量
class Vec3
{
public:
    float x, y, z;

public:
    Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    Vec3 operator+(const Vec3 &v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator-(const Vec3 &v) const { return Vec3(x - v.x, y - v.y, z - v.z); }

    // 取负
    Vec3 operator-() const { return Vec3(-x, -y, -z); }
    Vec3 negative() const { return Vec3(-x, -y, -z); }

    // 数乘
    Vec3 operator*(const float d) const { return Vec3(x * d, y * d, z * d); }
    friend Vec3 operator*(const float d, const Vec3 &v) { return Vec3(v.x * d, v.y * d, v.z * d); }

    // 点乘
    float dot(const Vec3 &v) const { return x * v.x + y * v.y + z * v.z; }
    float operator*(const Vec3 &v) const { return x * v.x + y * v.y + z * v.z; }

    // 叉乘
    Vec3 cross(const Vec3 &v) const
    {
        return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    Vec3 operator/(const float &d) const { return Vec3(x / d, y / d, z / d); }

    // 相等
    bool operator==(const Vec3 &v) const { return x == v.x && y == v.y && z == v.z; }
    bool operator!=(const Vec3 &v) const { return !(x == v.x && y == v.y && z == v.z); }
    bool equal(const Vec3 &v) const { return x == v.x && y == v.y && z == v.z; }

    // 归零
    void clear()
    {
        x = 0;
        y = 0;
        z = 0;
    }

    Vec3 normalize() const
    {
        float mg = sqrt(x * x + y * y + z * z);
        return Vec3(x / mg, y / mg, z / mg);
    }
};