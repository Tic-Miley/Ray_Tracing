//

#include "Trace.hpp"
#include <iostream>
#include <cmath>
#include <thread>
#include <mutex>

// 光线与场景中的所有物体的相交判断 输入光线、物体和光源坐标 并返回光线颜色 路径追踪主函数
Vec3 PathTracing::trace(const Ray &r, const Scene &scene)
{
    float t = MAXf;    // 最近的相交时间
    int hitIndex = -1; // 相交物体的索引
    // 遍历所有物体
    // BVH 遍历
    traceBVH(scene.root, r, t, hitIndex);
    // for (const auto &object : scene.objects)
    // {
    //     float dist;
    //     if (object->intersect(r, dist) && dist < t)
    //     {
    //         t = dist;                 // 更新最近相交时间
    //         hitIndex = object->index; // 更新相交物体的索引
    //     }
    // }
    
    // 为光线着色
    if (hitIndex == -1)                          // 不与物体相交时
        return BackgroundColor;                  // 背景色
    if (hitIndex == 0 && r.orig == scene.camPos) // 初始光线直接打到光源 光源索引为0
        return Vec3(1, 1, 1);                    // 光源为白色
    if (hitIndex == 0 && r.orig != scene.camPos) // 取样反射光线直接打到光源
        return Vec3(0.5, 0.5, 0.5);              // 返回反射点颜色

    ////路径追踪部分
    Vec3 point = r.orig + t * r.dir; // 交点坐标 p
    const std::shared_ptr<Object> &hitObject = scene.objects[hitIndex];
    const std::shared_ptr<Plane> &light = scene.light;
    Vec3 L_dir(0, 0, 0), L_indir(0, 0, 0); // 不满足以下两个 if 即为黑色

    // 光源贡献
    Vec3 x = light->head + randomf(0.0f, light->width) * light->right + randomf(0.0f, light->length) * light->down; // 在光源上取点 x 的坐标
    Vec3 normalVecotr = hitObject->getNormalVector(point);                                                          // 相交物体 交点处法线
    float cosTheta = (x - point).normalize() * normalVecotr;                                                        // p->x 与 n of object 夹角
    auto f_r = hitObject->BRDF();                                                                                   // 当前物体的 BRDF
    if (PathTracing::traceLight(Ray(point, (x - point).normalize()), scene.objects))                                             // 若 p->x 未被遮挡
    {
        // 走公式
        float cosTheta2 = (point - x).normalize() * light->getNormalVector(); // x->p 与 n of light 夹角
        float distance = sqrt((x - point) * (x - point));                     // 不应开方 暂时凑参
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

        L_indir = PathTracing::trace(Ray(point, -wi), scene) * (f_r() * 1.5) * cosTheta / pdf_wi / P_RR;
        // L_indir = PathTracing::trace(Ray(point, -wi), scene) * f_r() * cosTheta / pdf_wi / P_RR;
    }
    return L_dir + L_indir;
}

// 光线能否到达光源判断
bool PathTracing::traceLight(const Ray &r, const std::vector<std::shared_ptr<Object>> &objects)
{
    float dist;
    for (int i = 6; i < objects.size(); ++i) // 暂时从 6 开始 直接排除盒子 应该判断 t 的大小
    {
        if (objects[i]->intersect(r, dist) && dist > 0)
            return false;
    }
    return true;
}

// 多线程处理 POSIX
void PathTracing::renderBlock(Scene &scene, int thread, int startRow, int endRow, std::mutex &mutex, std::vector<std::vector<unsigned char>> &color_thread)
{
    Vec3 color(0, 0, 0);
    int sqrtSSP = 4;
    int ssp = sqrtSSP * sqrtSSP;

    for (int j = startRow; j < endRow; j++)
    {
        for (int i = 0; i < scene.width; i++)
        {
            color.clear();
            for (int a = 0; a < sqrtSSP; a++)
            {
                for (int b = 0; b < sqrtSSP; b++)
                {
                    float x = 2.0 * (i + 0.03125 * a) / scene.width - 1.0;
                    float y = -2.0 * (j + 0.03125 * b) / scene.height + 1.0;
                    float scale = std::tan(rad(scene.visualAngle / 2.0));
                    float ratio = static_cast<float>(scene.width) / scene.height;
                    x = x * scale * ratio;
                    y = y * scale;
                    Vec3 dir = Vec3(x, y, -1).normalize();
                    Ray ray(scene.camPos, dir);
                    color = color + PathTracing::trace(ray, scene) / ssp;
                }
            }

            unsigned char r = static_cast<unsigned char>(std::max(0.0f, std::min(1.0f, color.x)) * 255);
            unsigned char g = static_cast<unsigned char>(std::max(0.0f, std::min(1.0f, color.y)) * 255);
            unsigned char b = static_cast<unsigned char>(std::max(0.0f, std::min(1.0f, color.z)) * 255);

            int index = ((j - startRow) * scene.width + i) * 3;
            // 线程各自的颜色缓冲区
            color_thread[thread][index + 0] = r;
            color_thread[thread][index + 1] = g;
            color_thread[thread][index + 2] = b;
        }
    }
}

// 渲染主函数 生成光线 存储颜色
void PathTracing::Render(Scene &scene)
{
    int threadCount = 12; // 12 条线程
    // int threadCount = std::thread::hardware_concurrency();
    int rowsPerThread = scene.height / threadCount;
    std::vector<std::thread> threads;
    std::mutex mutex;

    int block_size = rowsPerThread * scene.width * 3;
    std::vector<std::vector<unsigned char>> color_thread(threadCount, std::vector<unsigned char>(block_size));

    for (int t = 0; t < threadCount; t++)
    {
        int startRow = t * rowsPerThread;
        int endRow = (t == threadCount - 1) ? scene.height : startRow + rowsPerThread;
        threads.emplace_back(PathTracing::renderBlock, std::ref(scene), t, startRow, endRow, std::ref(mutex), std::ref(color_thread));
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    // 从线程颜色缓冲区提取颜色
    for (int t = 0; t < threadCount; t++)
        for (int i = 0; i < block_size; i++)
            scene.color_buffer[t * block_size + i] = color_thread[t][i];
}
