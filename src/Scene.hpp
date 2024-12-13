// 定义场景

#pragma once
#include <vector>
#include <memory>
#include "../include/stb_image_write.h"
#include "Sphere.hpp"

class Scene
{
public:
    int width, height; // 屏幕的宽度、高度
    // 相机视角
    std::vector<std::unique_ptr<Sphere>> objects; // 场景内的所有物体 目前仅支持球类 智能指针
    std::vector<unsigned char> color_buffer;      // 储存屏幕像素点颜色 RGB 255
public:
    Scene(const int width, const int height) : width(width), height(height) { color_buffer.resize(width * height * 3); }
    // 向场景中添加物体
    void add(std::unique_ptr<Sphere> object) { objects.emplace_back(std::move(object)); }
    // 打印图片 使用 stb_image_write 库
    void print();
};