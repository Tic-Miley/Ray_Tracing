// 定义常用的常量 内联函数

#pragma once
#include <limits>
#include <random>
#include <iostream>
#include "Vector.hpp"

#define PI 3.141592653589793

// float 类型最大值
const float MAXf = std::numeric_limits<float>::max();

// 背景色
const Vec3 BackgroundColor = Vec3(0.235294, 0.67451, 0.843137);

// 俄罗斯轮盘赌概率设置
const float P_RR = 0.2;

// 角度转弧度
inline float rad(const float &deg) { return deg * PI / 180; }

// 转换为线性颜色空间
inline void linear(Vec3 &color)
{
    color.x = pow(color.x, 2.2f);
    color.y = pow(color.y, 2.2f);
    color.z = pow(color.z, 2.2f);
}
// 伽马矫正
inline void gammaCorrect(Vec3 &color)
{
    color.x = pow(color.x, 1 / 2.2f);
    color.y = pow(color.y, 1 / 2.2f);
    color.z = pow(color.z, 1 / 2.2f);
}

// 生成随机数 [left, right] 区间中 均匀分布
inline float randomf(const float left, const float right)
{
    static std::random_device dev;
    static std::mt19937 gen(dev());
    std::uniform_real_distribution<float> dist(left, right);
    return dist(gen);
}