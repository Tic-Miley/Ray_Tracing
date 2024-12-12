#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

// 简单的3D向量类
class Vec3 {
public:
    float x, y, z;
    Vec3(float x=0, float y=0, float z=0) : x(x), y(y), z(z) {}
    Vec3 operator + (const Vec3& v) const { return Vec3(x+v.x, y+v.y, z+v.z); }
    Vec3 operator - (const Vec3& v) const { return Vec3(x-v.x, y-v.y, z-v.z); }
    Vec3 operator * (float d) const { return Vec3(x*d, y*d, z*d); }
    Vec3 operator / (float d) const { return Vec3(x/d, y/d, z/d); }
    float dot(const Vec3& v) const { return x*v.x + y*v.y + z*v.z; }
    Vec3 normalize() const {
        float mg = sqrt(x*x + y*y + z*z);
        return Vec3(x/mg, y/mg, z/mg);
    }
};

// 简单的光线类
class Ray {
public:
    Vec3 orig, dir;
    Ray(const Vec3& orig, const Vec3& dir) : orig(orig), dir(dir) {}
};

// 简化版的球体
class Sphere {
public:
    Vec3 center;
    float radius;
    Sphere(const Vec3& center, float radius) : center(center), radius(radius) {}
    
    bool intersect(const Ray& r, float &t) const {  // ray.dir 需要单位化
        Vec3 oc = r.orig - center;
        float b = 2 * oc.dot(r.dir);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * c;
        if (discriminant < 0) return false;
        else {
            discriminant = sqrt(discriminant);
            float t0 = -b - discriminant;           // ray = orig + t · dir, t 为距离
            float t1 = -b + discriminant; 
            t = (t0 < t1) ? t0 : t1;
            return true;
        }
    }
};

// 光线追踪主函数
Vec3 trace(const Ray& r, const std::vector<Sphere>& spheres) {
    float t = 20000; // 最大距离
    Vec3 color;
    for (const auto& sphere : spheres) {
        float dist;
        if (sphere.intersect(r, dist) && dist < t) {
            t = dist;   // t 还未用到
            color = Vec3(1, 0, 0); // 红色为示例
        }
    }
    return color;
}

int main() {
    std::vector<Sphere> spheres;
    spheres.emplace_back(Vec3(0,0,-20), 4);
    // spheres.emplace_back(Vec3(-10,-10,-15), 4);
    // spheres.emplace_back(Vec3(10,-10,-25), 4);
    Vec3 camPos(0, 0, 0);
    const int width = 800, height = 600;    // 图像尺寸

    std::vector<unsigned char> image(width * height * 3); // 存储像素数据

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            // 生成光线     // i \in [0,width-1] 映射到 x \in [-1,1]
            // float x = (i - width/2.0) / (width/2.0);        
            float x = (i - width/2.0) / (height/2.0);        // 按比例映射
            float y = -(j - height/2.0) / (height/2.0);
            Vec3 dir = Vec3(x, y, -1).normalize();          // dir 单位化
            Ray ray(camPos, dir);
            Vec3 color = trace(ray, spheres);

            // 将颜色由 [0.0,1.0] 映射到 [0,255]
            int r = static_cast<int>(std::max(0.0f, std::min(1.0f, color.x)) * 255);
            int g = static_cast<int>(std::max(0.0f, std::min(1.0f, color.y)) * 255);
            int b = static_cast<int>(std::max(0.0f, std::min(1.0f, color.z)) * 255);

            int index = (j * width + i) * 3;
            image[index + 0] = r;
            image[index + 1] = g;
            image[index + 2] = b;
        }
    }
    // 保存为 png
    stbi_write_png("./images/output.png", width, height, 3, image.data(), width * 3);    // vector.data() 返回数组第一个元素的指针

    return 0;
}
