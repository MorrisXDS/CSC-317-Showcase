#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object.h"
#include <Eigen/Core>
#include "ray_intersect_triangle.h"

class Triangle : public Object
{
  public:
    // A triangle has three corners
    std::tuple< Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d> corners;

    explicit Triangle(Eigen::Vector3d A,Eigen::Vector3d B, Eigen::Vector3d C): corners({A,B,C})
    {
    }

    bool ray_intersect(
    const Ray& ray,
    const double min_t,
    const double max_t,
    Eigen::Vector3d & normal,
    double & t,
    [[maybe_unused]]std::shared_ptr<Object> & descendant) const override
    {
        return ray_intersect_triangle(ray,
            std::get<0>(this->corners).transpose(), std::get<1>(this->corners).transpose(), std::get<2>(this->corners).transpose(),
            min_t, max_t, normal, t);
    };
};

#endif
