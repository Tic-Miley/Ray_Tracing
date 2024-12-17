//

#include "Trace.hpp"
#include <iostream>
#include <cmath>

// 光线与场景中的所有物体的相交判断 输入光线、物体和光源坐标 并返回光线颜色 暂时使用朗伯反射模型实现效果
Vec3 trace(const Ray &r, const Scene &scene)
{
    float t = MAXf;    // 最近的相交时间
    int hitIndex = -1; // 相交物体的索引
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
    // 为光线着色
    if (hitIndex == -1)                          // 不与物体相交时
        return BackgroundColor;                  // 背景色
    if (hitIndex == 0 && r.orig == scene.camPos) // 初始光线直接打到光源 光源索引为0
        return Vec3(1, 1, 1);                    // 光源为白色
    if (hitIndex == 0 && r.orig != scene.camPos) // 取样反射光线直接打到光源
        return Vec3(0.5, 0.5, 0.5);              // 返回反射点颜色

    ////路径追踪部分
    Vec3 point = r.orig + t * r.dir; // 交点坐标 p
    const std::unique_ptr<Object> &hitObject = scene.objects[hitIndex];
    const std::shared_ptr<Plane> &light = scene.light;
    Vec3 L_dir(0, 0, 0), L_indir(0, 0, 0); // 不满足以下两个 if 即为黑色
    // 光源贡献
    Vec3 x = light->head + randomf(0.0f, light->width) * light->right + randomf(0.0f, light->length) * light->down; // 在光源上取点 x 的坐标
    Vec3 normalVecotr = hitObject->getNormalVector(point);                                                          // 相交物体 交点处法线
    float cosTheta = (x - point).normalize() * normalVecotr;                                                        // p->x 与 n of object 夹角
    auto f_r = hitObject->BRDF();                                                                                   // 当前物体的 BRDF
    if (traceLight(Ray(point, (x - point).normalize()), scene.objects))                                             // 若 p->x 未被遮挡
    {
        // 走公式
        float cosTheta2 = (point - x).normalize() * light->getNormalVector(); // x->p 与 n of light 夹角
        float distance = sqrt((x - point) * (x - point));
        float pdf_light = 1 / (light->width * light->length);

        L_dir = hitObject->color * cosTheta * cosTheta2 * (f_r() * 2 * PI) / distance / pdf_light;
        // L_dir = hitObject->color * f_r() * cosTheta * cosTheta2 / distance / pdf_light;
    }
    // 反射贡献
    if (randomf(0.0f, 1.0f) < P_RR) // 俄罗斯轮盘赌成功
    {
        // 走公式
        Vec3 wi(randomf(-1, 1), randomf(-1, 1), randomf(-1, 1)); // 在 point 的上半球面取反射光方向 wi
        if (wi * normalVecotr < 0)
            wi = -wi;
        wi = wi.normalize();
        cosTheta = wi * normalVecotr;
        float pdf_wi = 1 / (2 * PI);
        L_indir = trace(Ray(point, -wi), scene) * (f_r() * 1.5) * cosTheta / pdf_wi / P_RR;
        // L_indir = trace(Ray(point, -wi), scene)*cosTheta/P_RR;
    }
    return L_dir + L_indir;
}
// 光线能否到达光源判断
bool traceLight(const Ray &r, const std::vector<std::unique_ptr<Object>> &objects)
{
    float dist;
    for (int i = 6; i < objects.size(); ++i) // 暂时从 6 开始 直接排除盒子 应该判断 t 的大小
    {
        if (objects[i]->intersect(r, dist) && dist > 0)
            return false;
    }
    return true;
}

// 将 Vec3 类型的像素颜色存进颜色缓冲区
void storeColor(std::vector<unsigned char> &color_buffer, Vec3 &color, const int index)
{
    // gammaCorrect(color);

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
    Vec3 color(0, 0, 0);
    for (int j = 0; j < scene.height; j++)
    {
        for (int i = 0; i < scene.width; i++)
        {
            color.clear();
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

            int epp = 512;
            for (int i = 0; i < epp; ++i)
                color = color + trace(ray, scene) / epp;

            int index = (j * scene.width + i) * 3;
            storeColor(scene.color_buffer, color, index);

            if (index % 1000 == 0)
                std::cout << "=";
        }
    }
}
