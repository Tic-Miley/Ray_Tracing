// 主函数

#include "Trace.hpp"

inline void creatObjects(Scene &scene)
{
    // 创建光源 光源为一个平面
    auto plight = std::make_unique<Plane>(Vec3(7, 50, -107), Vec3(0, 0, -1), Vec3(-1, 0, 0), 6, 14, Vec3(1, 1, 1));
    scene.add(std::move(plight));

    // 创建一个盒子
    auto pd = std::make_unique<Plane>(Vec3(25, 0, -120), Vec3(0, 0, 1), Vec3(-1, 0, 0), 20, 50, Vec3(0.89412, 0.75686, 0.61176));
    scene.add(std::move(pd));
    auto pu = std::make_unique<Plane>(Vec3(25, 50, -100), Vec3(0, 0, -1), Vec3(-1, 0, 0), 20, 50, Vec3(0.89412, 0.75686, 0.61176));
    scene.add(std::move(pu));
    auto pl = std::make_unique<Plane>(Vec3(-25, 50, -100), Vec3(0, 0, -1), Vec3(0, -1, 0), 20, 50, Vec3(0.6, 0.2, 0.2));
    scene.add(std::move(pl));
    auto pr = std::make_unique<Plane>(Vec3(25, 50, -120), Vec3(0, 0, 1), Vec3(0, -1, 0), 20, 50, Vec3(0.2, 0.2, 0.6));
    scene.add(std::move(pr));
    auto pf = std::make_unique<Plane>(Vec3(-25, 50, -120), Vec3(1, 0, 0), Vec3(0, -1, 0), 50, 50, Vec3(0.89412, 0.75686, 0.61176));
    scene.add(std::move(pf));

    // 放置两个球
    auto s1 = std::make_unique<Sphere>(Vec3(-11, 8, -113), 8, Vec3(0.8, 0.69412, 0.52941));
    scene.add(std::move(s1));
    auto s2 = std::make_unique<Sphere>(Vec3(10, 8, -107), 8, Vec3(0.8, 0.69412, 0.52941));
    scene.add(std::move(s2));

    // 光源和相机
    auto light = std::make_shared<Plane>(Vec3(7, 50, -107), Vec3(0, 0, -1), Vec3(-1, 0, 0), 6, 14, Vec3(1, 1, 1));
    scene.setLight(light);
    scene.setCamPos(Vec3(0, 25, -70));
}

// 命令为 main <filename>
int main(int argc, const char *argv[])
{
    Scene scene(500, 500);

    creatObjects(scene);

    PathTracing(scene);

    const char *filename = "../image/output.png";
    if (argc > 1)
        filename = argv[1];
    scene.print(filename);

    return 0;
}
