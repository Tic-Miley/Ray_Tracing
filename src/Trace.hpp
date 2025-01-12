// 生成光线 路径追踪上色 储存颜色

#pragma once
#include "Scene.hpp"
#include "Tools.hpp"
#include "BVH.hpp"
#include <vector>
#include <mutex>
#include <memory>

namespace PathTracing
{
    // 光线与场景中的所有物体的相交判断
    Vec3 trace(const Ray &r, const Scene &scene);

    // 光线与非光源的任意物体相交判断
    bool traceLight(const Ray &r, const std::vector<std::shared_ptr<Object>> &objects);

    void renderBlock(Scene &scene, int thread, int startRow, int endRow, std::mutex &mutex, std::vector<std::vector<unsigned char>> &color_thread);

    // 光线追踪主函数
    void Render(Scene &scene);
}