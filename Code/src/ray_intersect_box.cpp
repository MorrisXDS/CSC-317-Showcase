#include "ray_intersect_box.h"
#include <utility>

bool ray_intersect_box(
    const Ray& ray,
    const BoundingBox& box,
    const double min_t,
    const double max_t)
{
    const double origin_x = ray.origin.x();
    const double origin_y = ray.origin.y();
    const double origin_z = ray.origin.z();

    if (origin_x >= box.min_corner.x() && origin_x <= box.max_corner.x() &&
        origin_y >= box.min_corner.y() && origin_y <= box.max_corner.y() &&
        origin_z >= box.min_corner.z() && origin_z <= box.max_corner.z())
    {
        return true;
    }

    const double direction_x = ray.direction.x();
    const double direction_y = ray.direction.y();
    const double direction_z = ray.direction.z();

    const double min_x = box.min_corner.x();
    const double min_y = box.min_corner.y();
    const double min_z = box.min_corner.z();

    const double max_x = box.max_corner.x();
    const double max_y = box.max_corner.y();
    const double max_z = box.max_corner.z();

    double t_min_x = (min_x - origin_x) / direction_x;
    double t_max_x = (max_x - origin_x) / direction_x;
    double t_min_y = (min_y - origin_y) / direction_y;
    double t_max_y = (max_y - origin_y) / direction_y;
    double t_min_z = (min_z - origin_z) / direction_z;
    double t_max_z = (max_z - origin_z) / direction_z;

    if (t_min_x > t_max_x) std::swap(t_min_x, t_max_x);
    if (t_min_y > t_max_y) std::swap(t_min_y, t_max_y);
    if (t_min_z > t_max_z) std::swap(t_min_z, t_max_z);

    const double max_min_t = std::max({t_min_x, t_min_y, t_min_z});
    const double min_max_t = std::min({t_max_x, t_max_y, t_max_z});

    if (max_min_t > min_max_t)
        return false;

    if (min_max_t < min_t || max_min_t > max_t)
        return false;

    return true;
}
