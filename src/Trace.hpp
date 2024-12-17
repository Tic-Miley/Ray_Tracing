// 生成光线 路径追踪上色 储存颜色

#pragma once
#include "Scene.hpp"
#include "Tools.hpp"
#include <vector>
#include <memory>

// 光线与场景中的所有物体的相交判断
Vec3 trace(const Ray &r, const Scene &scene);

// 光线与非光源的任意物体相交判断
bool traceLight(const Ray &r, const std::vector<std::unique_ptr<Object>> &objects);

// 将 Vec3 类型的像素颜色存进颜色缓冲区
void storeColor(std::vector<unsigned char> &color_buffer, Vec3 &color, const int index);

// 光线追踪主函数
void PathTracing(Scene &scene);