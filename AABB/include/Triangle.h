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


    virtual bool point_squared_distance(
    const Eigen::RowVector3d & query,
    const double min_sqrd,
    const double max_sqrd,
    double & sqrd,
    std::shared_ptr<Object> & descendant) const override
    {
        assert(false && "you should never call point_squared_distance on Triangle!\n");
        return false;
    };

    bool ray_intersect(
    const Ray& ray,
    const double min_t,
    const double max_t,
    double & t,
    std::shared_ptr<Object> & descendant) const override
    {
        return ray_intersect_triangle(ray,
            std::get<0>(this->corners).transpose(), std::get<1>(this->corners).transpose(), std::get<2>(this->corners).transpose(),
            min_t, max_t, t);
    };
};

#endif
