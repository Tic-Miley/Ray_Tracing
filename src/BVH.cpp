//

#include "BVH.hpp"

// 输入某一节点，将其划分为左右两孩子节点，并决定递归传递 it 指针
void buildBVH(std::shared_ptr<BVHnode> it)
{
    if (it->objects->size() < 10) // 包含物体数小于 10 为叶节点，直接返回
        return;

    float partition;
    Vec3 newMin, newMax;
    it->Box.getMessage(partition, newMin, newMax);
    auto condition = it->Box.divideCondition();

    // std::shared_ptr<std::vector<std::shared_ptr<Object>>> leftObjects, rightObjects;
    auto leftObjects = std::make_shared<std::vector<std::shared_ptr<Object>>>();
    auto rightObjects = std::make_shared<std::vector<std::shared_ptr<Object>>>();

    for (auto iter : *(it->objects))
    {
        if (condition(iter, partition))
            leftObjects->emplace_back(iter);
        else
            rightObjects->emplace_back(iter);
    }
    it->objects.reset(); // 删除父节点物体向量的占用内存

    // 生成子节点
    it->left = std::make_shared<BVHnode>(AABB(it->Box.min, newMax), leftObjects);
    it->right = std::make_shared<BVHnode>(AABB(newMin, it->Box.max), rightObjects);

    // 递归调用
    buildBVH(it->left);
    buildBVH(it->right);
}

// DFS 搜索 BVH 树
void searchBVH(std::shared_ptr<BVHnode> it)
{
    if (it->isLeaf())
    {
        std::cout << "Leaf:" << std::endl;
        for (auto object : *(it->objects))
        {
            std::cout << object->index << ", ";
        }
        std::cout << std::endl;
        return;
    }
    else
    {
        searchBVH(it->left);
        searchBVH(it->right);
    }
}

void traceBVH(std::shared_ptr<BVHnode> it, const Ray &r, float &t, int &hitIndex)
{
    if (it->isLeaf())
    {
        for (auto object : *(it->objects))
        {
            float dist;
            if (object->intersect(r, dist) && dist < t)
            {
                t = dist;                 // 更新最近相交时间
                hitIndex = object->index; // 更新相交物体的索引
            }
        }
    }
    else
    {
        float dist_left, dist_right;
        bool flag_left, flag_right;

        flag_left = it->Box.intersect(r, dist_left);
        flag_right = it->Box.intersect(r, dist_right);

        if (!flag_right) // 仅与左盒子相交
        {
            traceBVH(it->left, r, t, hitIndex);
            return;
        }
        if (flag_left && dist_left < dist_right) // 均相交且左盒子更近
        {
            traceBVH(it->left, r, t, hitIndex);
            return;
        }
        traceBVH(it->right, r, t, hitIndex); // 仅剩搜索右盒子的情况
    }
}