// 生成光线 Blinn-Phong 模型上色 储存颜色

#pragma once
#include "Scene.hpp"
#include "Tools.hpp"
#include <vector>

namespace RealtimeTracing
{
    Vec3 trace(const Ray &r, const Scene& scene);

    void storeColor(std::vector<unsigned char> &color_buffer, const Vec3 &color, const int index);

    // 实时光线追踪主函数
    void Render(Scene &scene);
}

