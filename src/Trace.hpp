//

#pragma once
#include "Scene.hpp"
#include "Tools.hpp"
#include <vector>
#include <memory>

// 光线与场景中的所有物体的相交判断
Vec3 trace(const Ray &r, const std::vector<std::unique_ptr<Sphere>> &spheres);

// 将 Vec3 类型的像素颜色存进颜色缓冲区
void storeColor(std::vector<unsigned char> &color_buffer, const Vec3 &color, const int index);

// 光线追踪主函数
void PathTracing(Scene &scene);