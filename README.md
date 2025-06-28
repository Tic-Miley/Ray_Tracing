# 计算机图形学 光线追踪项目

## 使用说明
可执行文件 `RayTracer.exe` 在 `.\build\Release\`，此外在主文件夹中有一份拷贝。
**指令使用方法**为：
1. `RayTracer.exe image <path>`，该指令将实现路径追踪效果，生成一张图片。参数为：每像素取样 SSP = 1024，俄罗斯轮盘赌存活概率 P_RR = 90\%，该参数渲染效果较好，一次渲染时间在 20 分钟左右。渲染效果如下：
<div align="center">
  <img src="https://github.com/Tic-Miley/Ray_Tracing/blob/main/image/11_multiThreads.png" width="400" alt="路径追踪渲染效果">
</div>
1. `RayTracer.exe video`，该指令将实现实时渲染效果，创建一个窗口展示内容，可以通过键盘的 w, a, s, d, Space, Tab 等控制视点的移动。窗口显示如下：
<div align="center">
  <img src="https://github.com/Tic-Miley/Ray_Tracing/blob/main/screenshot/%E7%AA%97%E5%8F%A3%E6%98%BE%E7%A4%BA.png" width="400" alt="实时渲染窗口截图">
</div>

关于图像处理部分，项目使用到了 OpenCV 库，和一个开源的 C 文件 `stb_image_write.h`。

## 文件结构
- build：CMake 建立的文件夹
 - Release：包含生成的可执行文件 `RayTracer.exe`
- image：项目实现过程中实现效果的图片 
- include：项目使用到的 OpenCV 库和源文件 `stb_image_write.h`
- screenshot：项目实现过程中的截屏记录
- src：项目的源代码
- RayTracer.exe 生成的可执行文件

## 源代码组成
- `Scene.hpp` `Scene.cpp`：定义场景，包括屏幕大小、视点坐标、像素颜色数组和物体数组等。
- `Object.hpp`：定义物体，包括物体颜色、材质，以及物体需要得到的信息，包括某点法向量、物体边界、是否与光线相交以及某点的 BRDF 函数。
  - `Sphere.hpp`：子类，定义球体，实现父类虚函数。
  - `Plane.hpp`：子类，定义平面，实现父类虚函数。
- `BVH.hpp` `BVH.cpp`: **BVH 加速结构**的实现部分，包括 AABB 盒与 BVH 树的定义，以及 BVH 树的建立与搜索函数。
- `Trace.hpp` `Trace.cpp`: **路径追踪**和**多线程加速**的实现部分，1其中为每个线程新增颜色缓冲区，避免上锁的时间浪费。
其中，多线程加速的效果如下：
在上述路径追踪参数的情况下，使用多线程加速前渲染的时间为 20min 21s，16 线程（本地电脑理论线程数）情况下渲染时间为 9min 39s，12 线程情况下渲染时间为 7 min 51s，8 线程情况下渲染时间为 8 min 32s，故最终选择 12 线程。
- `BlinnPhong.hpp` `BlinnPhong.cpp`: 混合使用光线追踪和光栅化中 BlinnPhong 反射模型，在降低渲染质量的情况下提高单帧渲染速度，为**实时渲染系统**做好准备。
- `Ray.hpp` 简单定义了光线类。
- `Tool.hpp` 统一在该文件中定义了一些常量（如 $\pi$、最大值等），以及一些常用的简短函数（如弧度转换、随机数生成等）。
- `main.cpp` 该文件中**创建模型**，创建了项目使用的模型。首先定义了场景，然后通过判断命令参数来选择执行路径追踪或者实时渲染。
