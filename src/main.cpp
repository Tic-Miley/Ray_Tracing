// 主函数

#include "Trace.hpp"

int main(int argc, char *argv[])
{
    Scene scene(800, 600);
    auto s1 = std::make_unique<Sphere>(Vec3(0, 0, -20), 4);
    scene.add(std::move(s1));

    PathTracing(scene);

    // char* filename = "../image/output.png";
    // if(argc>1) filename = argv[1];
    scene.print();
    return 0;
}
