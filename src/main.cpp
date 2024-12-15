// 主函数

#include "Trace.hpp"

// 命令为 main <filename>
int main(int argc, const char *argv[])
{
    Scene scene(800, 600);
    auto s1 = std::make_unique<Sphere>(Vec3(0, 0, -20), 4, Vec3(1, 0, 0));
    scene.add(std::move(s1));
    // auto s2 = std::make_unique<Sphere>(Vec3(-10, -10, -15), 4, Vec3(0, 1, 0));
    // scene.add(std::move(s2));
    // auto s3 = std::make_unique<Sphere>(Vec3(10, -10, -25), 4, Vec3(0, 0, 1));
    // scene.add(std::move(s3));

    scene.setLight(Vec3(0, -10, 0));

    PathTracing(scene);

    const char *filename = "../image/output.png";
    if (argc > 1)
        filename = argv[1];
    scene.print(filename);

    return 0;
}
