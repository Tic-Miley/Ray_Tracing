// 主函数

#include "Trace.hpp"

inline void creatObjects(Scene &scene)
{
    // 创建一个盒子
    auto pd = std::make_unique<Plane>(Vec3(25, 0, -150), Vec3(0, 0, 1), Vec3(-1, 0, 0), 50, 50, Vec3(0.4, 0.4, 0.4));
    scene.add(std::move(pd));
    auto pu = std::make_unique<Plane>(Vec3(25, 100, -100), Vec3(0, 0, -1), Vec3(-1, 0, 0), 50, 50, Vec3(0.4, 0.4, 0.4));
    scene.add(std::move(pu));
    auto pl = std::make_unique<Plane>(Vec3(-25, 100, -100), Vec3(0, 0, -1), Vec3(0, -1, 0), 50, 100, Vec3(0.6, 0.2, 0.2));
    scene.add(std::move(pl));
    auto pr = std::make_unique<Plane>(Vec3(25, 100, -150), Vec3(0, 0, 1), Vec3(0, -1, 0), 50, 100, Vec3(0.2, 0.2, 0.6));
    scene.add(std::move(pr));
    auto pf = std::make_unique<Plane>(Vec3(-25, 100, -150), Vec3(1, 0, 0), Vec3(0, -1, 0), 50, 100, Vec3(0.2, 0.6, 0.2));
    scene.add(std::move(pf));

    // 放置一个球
    auto s1 = std::make_unique<Sphere>(Vec3(5, 15, -125), 15, Vec3(1, 0, 0));
    scene.add(std::move(s1));

    // 光源和相机
    scene.setLight(Vec3(0, 100, -125));
    scene.setCamPos(Vec3(0, 50, -30));
}

// 命令为 main <filename>
int main(int argc, const char *argv[])
{
    Scene scene(800, 600);

    creatObjects(scene);

    PathTracing(scene);

    const char *filename = "../image/output.png";
    if (argc > 1)
        filename = argv[1];
    scene.print(filename);

    return 0;
}
