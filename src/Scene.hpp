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
    Vec3 light = Vec3(0, -10, 0);                 // 光源位置 暂时为单点光源
    Vec3 camPos = Vec3(0, 0, 0);                  // 照相机位置
    std::vector<std::unique_ptr<Object>> objects; // 场景内的所有物体 目前仅支持球类 智能指针
    std::vector<unsigned char> color_buffer;      // 储存屏幕像素点颜色 RGB 255
public:
    Scene(const int width, const int height) : width(width), height(height) { color_buffer.resize(width * height * 3); }
    // 向场景中添加物体
    void add(std::unique_ptr<Object> object) { objects.emplace_back(std::move(object)); }
    // 设置光源
    void setLight(const Vec3 lightx) { light = lightx; }
    // 设置照相机位置
    void setCamPos(const Vec3 camPosx) { camPos = camPosx; }
    // 打印图片 使用 stb_image_write 库
    void print(const char *filename);
    // 清除场景
    void clear() { objects.clear(); }
};