//

#include "Trace.hpp"

// 光线与场景中的所有物体的相交判断
Vec3 trace(const Ray &r, const std::vector<std::unique_ptr<Sphere>> &spheres)
{
    float t = MAXf; // 最大距离
    Vec3 color;
    for (const auto &sphere : spheres)
    {
        float dist;
        if (sphere->intersect(r, dist) && dist < t)
        {
            t = dist;              // t 还未用到
            color = Vec3(1, 0, 0); // 红色为示例
        }
    }
    return color;
}

// 将 Vec3 类型的像素颜色存进颜色缓冲区
void storeColor(std::vector<unsigned char> &color_buffer, const Vec3 &color, const int index)
{
    unsigned char r = static_cast<unsigned char>(std::max(0.0f, std::min(1.0f, color.x)) * 255);
    unsigned char g = static_cast<unsigned char>(std::max(0.0f, std::min(1.0f, color.y)) * 255);
    unsigned char b = static_cast<unsigned char>(std::max(0.0f, std::min(1.0f, color.z)) * 255);

    color_buffer[index + 0] = r;
    color_buffer[index + 1] = g;
    color_buffer[index + 2] = b;
}

// 光线追踪主函数
void PathTracing(Scene &scene)
{
    Vec3 camPos(0, 0, 0);
    for (int j = 0; j < scene.height; j++)
    {
        for (int i = 0; i < scene.width; i++)
        {
            // 生成光线     // i \in [0,width-1] 映射到 x \in [-1,1]
            float x = (i - scene.width / 2.0) / (scene.width / 2.0);
            float y = -(j - scene.height / 2.0) / (scene.height / 2.0);
            Vec3 dir = Vec3(x, y, -1).normalize(); // dir 单位化
            Ray ray(camPos, dir);
            Vec3 color = trace(ray, scene.objects);
            storeColor(scene.color_buffer, color, j * scene.width + i);
        }
    }
}