#ifndef PLANE_H
#define PLANE_H

#include "Object.h"
#include <Eigen/Core>
#include "Ray.h"

class Plane : public Object
{
  public:
    // Point on plane
    Eigen::Vector3d point;
    // Normal of plane
    Eigen::Vector3d normal;

    explicit Plane(Eigen::Vector3d p, Eigen::Vector3d n): point(p), normal(n)
    {

    };

    bool ray_intersect(
    const Ray& ray,
    const double min_t,
    const double max_t,
    Eigen::Vector3d & normal,
    double & t,
    std::shared_ptr<Object> & descendant) const override
    {
      normal = this->normal;
      double denominator = this->normal.dot(ray.direction);

      // if denominator is 0, the camera is looking parallel to the plane.
      // there is no way the ray intersects with the plane
      if (denominator == 0.0)
        return false;

      Eigen::Vector3d point_to_origin = ray.origin - this->point;
      double numerator = -this->normal.dot(point_to_origin);

      double t_temp = numerator / denominator;
      if (t_temp < min_t)
        return false;

      t = t_temp;
      return true;
    }
};

#endif
