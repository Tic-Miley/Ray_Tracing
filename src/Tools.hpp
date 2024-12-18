// 定义常用的常量 内联函数

#pragma once
#include <limits>
#include <random>
#include <iostream>
#include "Vector.hpp"

#define PI 3.141592653589793

// float 类型最大值 最小值
const float MAXf = std::numeric_limits<float>::max();
const float MINf = std::numeric_limits<float>::lowest();

// 背景色
// const Vec3 BackgroundColor = Vec3(0.235294, 0.67451, 0.843137);
const Vec3 BackgroundColor = Vec3(0, 0, 0);

// 俄罗斯轮盘赌概率设置
const float P_RR = 0.9;

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
static std::random_device dev;
static std::mt19937 gen(dev());
static std::uniform_real_distribution<float> dist(0, 1);
inline float randomf(const float left, const float right)
{
    return dist(gen) * (right - left) + left;
}

// 进度条 输入完成进度
inline void UpdateProgress(float rate)
{
    int barWidth = 70;

    std::cout << "[";
    int pos = barWidth * rate;
    for (int i = 0; i < barWidth; ++i)
    {
        if (i < pos)
            std::cout << "=";
        else if (i == pos)
            std::cout << ">";
        else
            std::cout << " ";
    }
    std::cout << "] " << int(rate * 100.0) << " %\r";
    std::cout.flush();
};