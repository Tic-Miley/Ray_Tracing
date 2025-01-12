//

#include "BlinnPhong.hpp"
#include "Trace.hpp"
#include <iostream>
#include <cmath>

// 光线与场景中的所有物体的相交判断 输入光线、物体和光源坐标 并返回光线颜色 暂时使用朗伯反射模型实现效果
Vec3 RealtimeTracing::trace(const Ray &r, const Scene &scene)
{
    float t = MAXf;    // 获取最近的相交时间
    int hitIndex = -1; // 相交物体的索引
    Vec3 color = BackgroundColor;
    // 遍历所有物体
    for (const auto &object : scene.objects)
    {
        float dist;
        if (object->intersect(r, dist) && dist < t)
        {
            t = dist;                 // 更新最近相交时间
            hitIndex = object->index; // 更新相交物体的索引
        }
    }

    if (hitIndex == -1)         // 不与物体相交时
        return BackgroundColor; // 背景色
    if (hitIndex == 0)          // 初始光线直接打到光源 光源索引为0
        return Vec3(1, 1, 1);   // 光源为白色

    const std::shared_ptr<Object> &object = scene.objects[hitIndex];                                      // 相交物体
    Vec3 point = r.orig + r.dir * t;                                                                      // 交点坐标
    Vec3 normalVector = object->getNormalVector(point);                                                   // 法向量
    Vec3 lightVector = (scene.light->getCenter() - point).normalize();                                    // 光源方向
    Vec3 viewVector = (r.orig - point).normalize();                                                       // 照相机方向
    Vec3 reflectVector = (normalVector * 2.0f * lightVector.dot(normalVector) - lightVector).normalize(); // 反射光线方向

    Vec3 ambient = object->color * 0.1f; // 环境光
    Vec3 diffuse(0, 0, 0), specular(0, 0, 0);

    if (PathTracing::traceLight(Ray(point, lightVector), scene.objects))
    {
        float diff = std::max(0.0f, normalVector.dot(lightVector));
        diffuse = diffuse + object->color * diff * 0.7f;

        float spec = pow(std::max(0.0f, viewVector.dot(reflectVector)), 3);
        specular = specular + (1.0, 1.0, 1.0) * spec * 0.5f;
    }
    else // 打上阴影 扩大光源面积再次尝试 以实现阴影边缘柔化
    {
        Vec3 x(0, 0, 0);                                                                // 光源取样点
        Vec3 newLightHead = scene.light->head - scene.light->right + scene.light->down; // 假光源面积扩大到9倍
        for (int i = 0; i < 9; ++i)                                                     // 对光源取样九点 实现软阴影
        {
            x = scene.light->head + scene.light->right * 1.5 * (i % 3) + scene.light->down * 1.5 * (static_cast<int>(i / 3));
            lightVector = (x - point).normalize();                               // 取样的光线方向
            if (PathTracing::traceLight(Ray(point, lightVector), scene.objects)) // 不被光线直射的点只赋环境光
            {
                // float diff = pow(std::max(0.0f, normalVector.dot(lightVector)), 2);
                float diff = std::max(0.0f, normalVector.dot(lightVector));
                diffuse = diffuse + object->color * diff * 0.7f / 9; // 漫反射光
                float spec = pow(std::max(0.0f, viewVector.dot(reflectVector)), 3);
                specular = specular + (1.0, 1.0, 1.0) * spec * 0.5f / 9; // 反射光
            }
        }
    }

    color = ambient + diffuse + specular;

    // 伽马校正
    // gammaCorrect(color);

    return color;
}

// 将 Vec3 类型的像素颜色存进颜色缓冲区
void RealtimeTracing::storeColor(std::vector<unsigned char> &color_buffer, const Vec3 &color, const int index)
{
    unsigned char r = static_cast<unsigned char>(std::max(0.0f, std::min(1.0f, color.x)) * 255);
    unsigned char g = static_cast<unsigned char>(std::max(0.0f, std::min(1.0f, color.y)) * 255);
    unsigned char b = static_cast<unsigned char>(std::max(0.0f, std::min(1.0f, color.z)) * 255);

    color_buffer[index + 0] = b;
    color_buffer[index + 1] = g;
    color_buffer[index + 2] = r; // OpenCV 储存顺序
}

// 光线追踪主函数 生成光线
void RealtimeTracing::Render(Scene &scene)
{
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
            Ray ray(scene.camPos, dir);

            Vec3 color = RealtimeTracing::trace(ray, scene);

            int index = (j * scene.width + i) * 3;
            RealtimeTracing::storeColor(scene.color_buffer, color, index);
        }
    }
}
