// 定义了 BVH 算法相关类和函数

#pragma once
#include "Object.hpp"
#include "Scene.hpp"

// 坐标轴
enum axis
{
    xAxis,
    yAxis,
    zAxis
};
// 返回点的最大坐标对应的坐标轴
inline axis maxAxis(const Vec3 &v)
{
    if (v.x > v.y)
    {
        if (v.x > v.z)
            return xAxis;
        else
            return zAxis;
    }
    else
    {
        if (v.y > v.z)
            return yAxis;
        else
            return zAxis;
    }
}

// AABB 包围盒类 通过两个顶点定义
class AABB
{
public:
    Vec3 min, max; // 坐标最小的和最大的顶点
public:
    AABB(const Vec3 &min, const Vec3 &max) : min(min), max(max) {}

    // AABB 盒是否与光线相交
    bool intersect(const Ray &r, float &t)
    {
        float t1, t2, t3, T1, T2, T3, t_enter, t_exit;

        t1 = (min.x - r.orig.x) / r.dir.x;
        t2 = (min.y - r.orig.y) / r.dir.y;
        t3 = (min.z - r.orig.z) / r.dir.z;
        T1 = (max.x - r.orig.x) / r.dir.x;
        T2 = (max.y - r.orig.y) / r.dir.y;
        T3 = (max.z - r.orig.z) / r.dir.z;

        t_enter = std::max(std::max(t1, t2), t3);
        t_exit = std::min(std::min(T1, T2), T3);

        t = t_enter;
        return t_enter < t_exit && t_exit >= 0;
    }

    // 获取要切分的坐标轴
    axis cutAxis() const
    {
        Vec3 size = max - min;
        if (size.x < 0)
            size = -size; // 防止输入写反
        return maxAxis(size);
    }
    // 用引用的形式获取 切分点所在轴坐标 两个子包围盒(newMin,max)(min,newMax)
    void getMessage(float &partition, Vec3 &newMin, Vec3 &newMax) const
    {
        switch (cutAxis())
        {
        case xAxis:
            partition = (min.x + max.x) / 2;
            newMin = Vec3(partition, min.y, min.z);
            newMax = Vec3(partition, max.y, max.z);
            break;
        case yAxis:
            partition = (min.y + max.y) / 2;
            newMin = Vec3(min.x, partition, min.z);
            newMax = Vec3(max.x, partition, max.z);
            break;
        case zAxis:
            partition = (min.z + max.z) / 2;
            newMin = Vec3(min.x, min.y, partition);
            newMax = Vec3(max.x, max.y, partition);
            break;
        }
    }

    std::function<bool(const std::shared_ptr<Object> &, float partition)> divideCondition()
    {
        switch (cutAxis())
        {
        case xAxis:
            return [](const std::shared_ptr<Object> &object, float partition) -> bool
            { return object->getBound().x < partition; };
            break;
        case yAxis:
            return [](const std::shared_ptr<Object> &object, float partition) -> bool
            { return object->getBound().y < partition; };
            break;
        case zAxis:
            return [](const std::shared_ptr<Object> &object, float partition) -> bool
            { return object->getBound().z < partition; };
            break;
        default:
            return [](const std::shared_ptr<Object> &object, float partition) -> bool
            { return object->getBound().x < partition; };
        }
    }
};

// BVH 树节点
class BVHnode
{
public:
    AABB Box;
    std::shared_ptr<std::vector<std::shared_ptr<Object>>> objects; // 非叶节点的物体向量仅在构造时保存一次
    std::shared_ptr<BVHnode> left = nullptr;
    std::shared_ptr<BVHnode> right = nullptr;

public:
    BVHnode(const AABB &Box, std::shared_ptr<std::vector<std::shared_ptr<Object>>> objects)
        : Box(Box), objects(objects) {};
    // BVHnode(const AABB &Box, std::shared_ptr<std::vector<std::shared_ptr<Object>>> objects,
    //         std::shared_ptr<BVHnode> left, std::shared_ptr<BVHnode> right)
    //     : Box(Box), objects(std::move(objects)), left(left), right(right) {}
    bool isLeaf() const { return left == nullptr && right == nullptr; }
};

// 输入某节点的指针，完成该节点下 BVH 树的构建
void buildBVH(std::shared_ptr<BVHnode>);

void searchBVH(std::shared_ptr<BVHnode>);

void traceBVH(std::shared_ptr<BVHnode> it, const Ray &r, float &t, int &hitIndex);