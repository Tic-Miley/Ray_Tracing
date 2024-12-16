// 定义场景

#pragma once
#include <vector>
#include <memory>
#include "../include/stb_image_write.h"
#include "Sphere.hpp"
#include "Plane.hpp"

class Scene
{
public:
    int width = 400, height = 400;                // 屏幕的宽度、高度
    float visualAngle = 90;                       // 视角默认为 90 度
    std::shared_ptr<Plane> light;                 // 光源位置 暂时为单点光源
    Vec3 camPos = Vec3(0, 0, 0);                  // 照相机位置
    std::vector<std::unique_ptr<Object>> objects; // 场景内的所有物体 目前仅支持球类和平面 默认 objects[0] 是光源平面 智能指针
    std::vector<unsigned char> color_buffer;      // 储存屏幕像素点颜色 RGB 255
    int index = 0;                                // 目前新增物体的索引 索引与物体在向量中的下标一致
public:
    Scene(const int width, const int height) : width(width), height(height) { color_buffer.resize(width * height * 3); }
    // 向场景中添加物体
    void add(std::unique_ptr<Object> object)
    {
        object->index = index; // 在Object类外部设置其索引 对用户不可见
        objects.emplace_back(std::move(object));
        ++index;
    }
    // 设置光源
    void setLight(std::shared_ptr<Plane> lightx) { light = std::move(lightx); }
    // 设置照相机位置
    void setCamPos(const Vec3 camPosx) { camPos = camPosx; }
    // 打印图片 使用 stb_image_write 库
    void print(const char *filename);
    // 清除场景
    void clear() { objects.clear(); }
};