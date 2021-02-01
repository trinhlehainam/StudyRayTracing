#include "BVHNode.h"

#include <algorithm>
#include <cassert>

#include "Common/MathHelper.h"
#include "HitableList.h"
#include "HitRecord.h"

namespace
{
    bool CompareBox(const std::shared_ptr<IHitable> a, const std::shared_ptr<IHitable> b, unsigned int axis)
    {
        AABB boxA;
        AABB boxB;
        if (!a->IsBoundingBox(boxA) || !b->IsBoundingBox(boxB))
            return false;

        return boxA.Min[axis] < boxB.Min[axis];
    }

    bool CompareBoxInAxisX(const std::shared_ptr<IHitable> a, const std::shared_ptr<IHitable> b)
    {
        return CompareBox(a, b, 0);
    }

    bool CompareBoxInAxisY(const std::shared_ptr<IHitable> a, const std::shared_ptr<IHitable> b)
    {
        return CompareBox(a, b, 1);
    }

    bool CompareBoxInAxisZ(const std::shared_ptr<IHitable> a, const std::shared_ptr<IHitable> b)
    {
        return CompareBox(a, b, 2);
    }
}

BVHNode::BVHNode(const HitableList& list) :
    BVHNode(list.Objects, 0, list.Objects.size())
{
}

BVHNode::BVHNode(const std::vector<std::shared_ptr<IHitable>>& srcObjects, size_t start,
    size_t end)
{
    auto objects = srcObjects;
    int axis = static_cast<unsigned int>(MathHelper::Random<float>(0.0f, 3.0f));
    auto comparator = axis == 0 ? CompareBoxInAxisX :
                   axis == 1 ? CompareBoxInAxisY :
                   CompareBoxInAxisZ;

    const size_t num_object = end - start;

    switch (num_object)
    {
    case 1:
        Left = Right = objects[start];
        break;
    case 2:
        {
            if (comparator(objects[start], objects[start + 1]))
            {
                Left = objects[start];
                Right = objects[start + 1];
            }
            else
            {
                Left = objects[start + 1];
                Right = objects[start];
            }
        }
        break;
    default:
        {
            std::sort(objects.begin() + start, objects.begin() + end, comparator);
            size_t mid = start + num_object / 2;
            Left = std::make_shared<BVHNode>(objects, start, mid);
            Right = std::make_shared<BVHNode>(objects, mid, end);
        }
        break;
    }

    AABB boxLeft;
    AABB boxRight;

    if (!Left->IsBoundingBox(boxLeft) || !Right->IsBoundingBox(boxRight))
        assert(false);

    Volume = AABB::SurroundingBox(boxLeft, boxRight);
}

bool BVHNode::IsHit(const Ray& ray, float rangeMin, float rangeMax, HitRecord& record) const
{
    if (!Volume.IsHit(ray, rangeMin, rangeMax)) return false;

    bool hit_left = Left->IsHit(ray, rangeMin, rangeMax, record);
    bool hit_right = Right->IsHit(ray, rangeMin, hit_left ? record.t : rangeMax, record);

    return hit_left || hit_right;
}

bool BVHNode::IsBoundingBox(AABB& output) const
{
    output = Volume;
    return true;
}
