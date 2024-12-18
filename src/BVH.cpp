// 

#include "BVH.hpp"

// 输入某一节点，将其划分为左右两孩子节点，并决定递归传递 it 指针
void buildBVH(std::shared_ptr<BVHnode> it)
{
    float partition;
    Vec3 newMin,newMax;
    it->Box.getMessage(partition,newMin,newMax);
    auto condition = it->Box.divideCondition();

    std::unique_ptr<std::vector<std::shared_ptr<Object>>> leftObjects,rightObjects;

    for(auto iter:*(it->objects))
    {
        if(condition(iter,partition))
            leftObjects->emplace_back(iter);
        else
            rightObjects->emplace_back(iter);
    }
    it->objects.reset(); //删除父节点物体向量的占用内存

    // 生成子节点
    it->left = std::make_shared<BVHnode>(AABB(it->Box.min,newMax),leftObjects);
    it->right = std::make_shared<BVHnode>(AABB(newMin,it->Box.max),rightObjects);

    // 递归调用
    if(leftObjects->size() < 3)//包含的物体数小于 3
        buildBVH(it->left);
    if(rightObjects->size() <3)
        buildBVH(it->right);
}

// DFS 搜索 BVH 树
void searchBVH(std::shared_ptr<BVHnode> it)
{
    if(it->isLeaf())
    {
        for(auto object:*(it->objects))
        {
            std::cout<<object->index<<", ";
        }
        std::cout<<std::endl;
        return;
    }
    searchBVH(it->left);
    searchBVH(it->right);
}