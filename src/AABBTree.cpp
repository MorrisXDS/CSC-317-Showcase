#include "AABBTree.h"
#include "insert_box_into_box.h"
#include <algorithm>


AABBTree::AABBTree(
    const std::vector<std::shared_ptr<Object>>& objects,
    int a_depth) :
    depth(std::move(a_depth)),
    num_leaves(objects.size())
{
    if (num_leaves == 0)
        return;

    if (num_leaves == 1)
    {
        this->left = objects[0];
        insert_box_into_box(objects[0].get()->box, this->box);
        return;
    }
    if (num_leaves == 2)
    {
        this->left = objects[0];
        this->right = objects[1];
        insert_box_into_box(objects[0].get()->box, this->box);
        insert_box_into_box(objects[1].get()->box, this->box);
        return;
    }

    for (auto object_ptr : objects)
    {
        insert_box_into_box(object_ptr.get()->box, this->box);
    }

    double x_axis = box.max_corner.x() - box.min_corner.x();
    double y_axis = box.max_corner.y() - box.min_corner.y();
    double z_axis = box.max_corner.z() - box.min_corner.z();

    double longest_axis = std::max({x_axis, y_axis, z_axis});

    std::vector<std::shared_ptr<Object>> sorted_objects(objects.begin(), objects.end());
    bool is_too_close = false;

    if (longest_axis == x_axis)
    {
        std::sort(sorted_objects.begin(), sorted_objects.end(),
                  [](const std::shared_ptr<Object>& A, const std::shared_ptr<Object>& B)
                  {
                      return A.get()->box.center().x() < B.get()->box.center().x();
                  });
        double distance = sorted_objects[num_leaves - 1].get()->box.center().x() - sorted_objects[0].get()->box.center().x();
        if (distance <= x_axis / 100)
        {
            is_too_close = true;
        }
    }
    if (longest_axis == y_axis || is_too_close)
    {
        is_too_close = false;
        std::sort(sorted_objects.begin(), sorted_objects.end(),
                  [](const std::shared_ptr<Object>& A, const std::shared_ptr<Object>& B)
                  {
                      return A.get()->box.center().y() < B.get()->box.center().y();
                  });
        double distance = sorted_objects[num_leaves - 1].get()->box.center().y() - sorted_objects[0].get()->box.center().y();
        if (distance <= y_axis / 100)
        {
            is_too_close = true;
        }
    }
    if (longest_axis == z_axis || is_too_close)
    {
        std::sort(sorted_objects.begin(), sorted_objects.end(),
                  [](const std::shared_ptr<Object>& A, const std::shared_ptr<Object>& B)
                  {
                      return A.get()->box.center().z() < B.get()->box.center().z();
                  });
    }
    const size_t mid = (0 + objects.size()) / 2;

    std::vector<std::shared_ptr<Object>> left_objects(sorted_objects.begin(), sorted_objects.begin() + mid);
    std::vector<std::shared_ptr<Object>> right_objects(sorted_objects.begin() + mid, sorted_objects.end());

    this->left = std::make_shared<AABBTree>(left_objects, a_depth + 1);
    this->right = std::make_shared<AABBTree>(right_objects, a_depth + 1);
}
