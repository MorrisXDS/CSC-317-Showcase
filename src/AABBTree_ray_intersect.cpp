#include "AABBTree.h"
#include "MeshTriangle.h"
#include "ray_intersect_box.h"

// See AABBTree.h for API
bool AABBTree::ray_intersect(
    const Ray& ray,
    const double min_t,
    const double max_t,
    Eigen::Vector3d & normal,
    double & t,
    std::shared_ptr<Object> & descendant) const
{
    ////////////////////////////////////////////////////////////////////////////
    // Replace with your code here:
    if (!ray_intersect_box(ray, this->box, min_t, max_t))
        return false;

    bool is_left_hit = false, is_right_hit = false;
    std::shared_ptr<Object> left_descendant, right_descendant;
    double left_t = std::numeric_limits<double>::infinity();
    double right_t = std::numeric_limits<double>::infinity();

    Eigen::Vector3d left_normal;
    Eigen::Vector3d right_normal;

    std::shared_ptr<AABBTree> left_ptr = std::dynamic_pointer_cast<AABBTree>(this->left);
    std::shared_ptr<AABBTree> right_ptr = std::dynamic_pointer_cast<AABBTree>(this->right);

    if (this->left)
    {
        is_left_hit = this->left->ray_intersect(ray, min_t, max_t, left_normal, left_t, left_descendant);

        if (is_left_hit && !left_ptr)
        {
            left_descendant = this->left;
            normal = left_normal;
        }
    }

    if (this->right)
    {
        is_right_hit = this->right->ray_intersect(ray, min_t, (is_left_hit) ? left_t : max_t, right_normal, right_t, right_descendant);
        if (is_right_hit && !right_ptr)
        {
            right_descendant = this->right;
            normal = right_normal;
        }
    }

    if (is_left_hit && is_right_hit)
    {
        if (left_t < right_t)
        {
            t = left_t;
            descendant = left_descendant;
            normal = left_normal;
        }
        else
        {
            t = right_t;
            descendant = right_descendant;
            normal = right_normal;
        }
        return true;
    }
    else if (is_left_hit)
    {
        t = left_t;
        descendant = left_descendant;
        normal = left_normal;
        return true;
    }
    else if (is_right_hit)
    {
        t = right_t;
        descendant = right_descendant;
        normal = right_normal;
        return true;
    }

    return false;
    ////////////////////////////////////////////////////////////////////////////
}

