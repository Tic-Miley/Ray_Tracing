# 光线追踪
计算机图形学期末作业

## 约定
+ 效率为先 优先考虑程序功能的实现
+ 保证了实现效果之后 再作代码的格式化和细节优化

## 内容
+ Trace 光线追踪渲染函数 每次渲染一个场景
+ 每个场景包括 屏幕宽高 物体和光线的数组
+ 主函数中，首先设定一个场景，然后为场景添加物体和光源，然后开始渲染，得到图片
+ 使用智能指针 实现派生类在同一个向量中
+ 物体有同一个父类 Object 派生类中实现各自特性

+ 物体本身颜色 只作削弱不作增强

## 草稿
```cpp
    // 保存为 png
    stbi_write_png("./images/output.png", width, height, 3, image.data(), width * 3);    // vector.data() 返回数组第一个元素的指针

    // g++ -Wall 参数 报告更为详细的警告信息

    // 初步的模型
    // auto s1 = std::make_unique<Sphere>(Vec3(0, 0, -20), 4, Vec3(1, 0, 0));
    // scene.add(std::move(s1));
    auto s2 = std::make_unique<Sphere>(Vec3(0, -16, -75), 4, Vec3(1, 0, 0));
    scene.add(std::move(s2));
    // auto s3 = std::make_unique<Sphere>(Vec3(10, -10, -25), 4, Vec3(0, 0, 1));
    // scene.add(std::move(s3));
    auto p1 = std::make_unique<Plane>(Vec3(-25, -20, -100), Vec3(1, 0, 0), Vec3(0, 0, 1), 50, 50, Vec3(0, 0, 1));
    scene.add(std::move(p1));

    scene.setLight(Vec3(0, -10, 0));
    scene.setCamPos(Vec3(0,0,-10));
```
