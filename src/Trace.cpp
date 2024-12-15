//

#include "Trace.hpp"
#include <iostream>
#include <cmath>

// 光线与场景中的所有物体的相交判断 输入光线、物体和光源坐标 并返回光线颜色 暂时使用朗伯反射模型实现效果
Vec3 trace(const Ray &r, const std::vector<std::unique_ptr<Sphere>> &spheres, const Vec3 &light)
{
    float t = MAXf;                          // 获取最近的相交时间
    Vec3 color(0.235294, 0.67451, 0.843137); // 背景色
    // 遍历所有物体（暂时只为球体）
    for (const auto &sphere : spheres)
    {
        float dist;
        if (sphere->intersect(r, dist) && dist < t)
        {
            t = dist; // 更新最近相交时间

            // 暂时 每相交一次都计算光线颜色 需要后续为物体编序号
            Vec3 point = r.orig + r.dir * t;                                                                      // 交点坐标
            Vec3 normalVector = (point - sphere->center).normalize();                                             // 法向量
            Vec3 lightVector = (light - point).normalize();                                                       // 光源方向
            Vec3 viewVector = (r.orig - point).normalize();                                                       // 照相机方向
            Vec3 reflectVector = (normalVector * 2.0f * lightVector.dot(normalVector) - lightVector).normalize(); // 反射光线方向

            Vec3 ambient = sphere->color * 0.1f; // 环境光

            float diff = std::max(0.0f, normalVector.dot(lightVector));
            Vec3 diffuse = sphere->color * diff * 0.8f; // 漫反射光

            float spec = pow(std::max(0.0f, viewVector.dot(reflectVector)), 5);
            Vec3 specular = Vec3(1.0, 1.0, 1.0) * spec * 0.5f; // 反射光

            color = ambient + diffuse + specular;

            // 伽马校正
            gammaCorrect(color);
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

// 光线追踪主函数 生成光线
void PathTracing(Scene &scene)
{
    Vec3 camPos(0, 0, 0); // 照相机暂时固定为原点
    for (int j = 0; j < scene.height; j++)
    {
        for (int i = 0; i < scene.width; i++)
        {
            // 生成光线 ray 每个像素生成一条光线
            // 将坐标归一化到 [-1,1]
            float x = 2.0 * (i + 0.5) / scene.width - 1.0;
            float y = -2.0 * (j + 0.5) / scene.height + 1.0;
            // 将坐标缩放到 z=-1 平面
            float scale = std::tan(rad(scene.visualAngle / 2.0));         // 二分视角的正切
            float ratio = static_cast<float>(scene.width) / scene.height; // 屏幕的宽高比
            x = x * scale * ratio;                                        // 按比例放大 x
            y = y * scale;
            Vec3 dir = Vec3(x, y, -1).normalize(); // dir 单位化
            Ray ray(camPos, dir);

            Vec3 color = trace(ray, scene.objects, scene.light);

            int index = (j * scene.width + i) * 3;
            storeColor(scene.color_buffer, color, index);
        }
    }
}
