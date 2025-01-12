// 主函数

#include "Trace.hpp"
#include "BlinnPhong.hpp"
#include <chrono>
#include <opencv2/opencv.hpp>

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
    scene.setCamPos(Vec3(0, 25, -50));

    // 构建 BVH 树
    auto objectsPtr = std::make_shared<std::vector<std::shared_ptr<Object>>>(scene.objects);
    auto it = std::make_shared<BVHnode>(AABB(Vec3(-50, -30, -140), Vec3(50, 80, -80)), objectsPtr);
    buildBVH(it);
    // searchBVH(it); // 检查树的结构用
    scene.setBVHRoot(it);
}

// 命令为 main <filename>
int main(int argc, const char *argv[])
{
    Scene scene(804, 804);

    creatObjects(scene);

    // auto start = std::chrono::high_resolution_clock::now();
    // PathTracing::Render(scene);
    // RealtimeTracing::Render(scene);
    // auto end = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // std::cout << "Running time is: " << duration.count() << " milliseconds." << std::endl;

    // const char *filename = "../image/output.png";
    // if (argc > 1)
    //     filename = argv[1];
    // scene.print(filename);

    const int move_speed = 2;
    int key = 0;
    while (key != 27) // key != ESC
    {
        RealtimeTracing::Render(scene);
        cv::Mat image(804, 804, CV_8UC3, scene.color_buffer.data());
        cv::imshow("image", image);
        key = cv::waitKey(10);

        if (key == 'a') {
            scene.camPos.x -= move_speed; // 向左移动
        } else if (key == 'd') {
            scene.camPos.x += move_speed; // 向右移动
        } else if (key == 'w') {
            scene.camPos.z -= move_speed; // 向前移动
        } else if (key == 's') {
            scene.camPos.z += move_speed; // 向后移动
        } else if (key == 'j') {
            scene.camPos.y += move_speed; // 上升
        } else if (key == 'k') {
            scene.camPos.y -= move_speed; // 下降
        } else if (key == 32) {
            scene.camPos.y += move_speed; // 上升 Space
        } else if (key == 9) {
            scene.camPos.y -= move_speed; // 下降 Tab
        }
    }

    return 0;
}
