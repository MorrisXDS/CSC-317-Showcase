#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H

#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"
#include <Eigen/Core>
#include <utility>
#include "Ray.h"

class Sphere : public Object
{
public:
    Eigen::Vector3d center;
    double radius;
public:
    explicit Sphere(Eigen::Vector3d centre, double r):center(std::move(centre)), radius(r)
    {

    }

    bool ray_intersect(
    const Ray& ray,
    const double min_t,
    const double max_t,
    Eigen::Vector3d & normal,
    double & t,
    std::shared_ptr<Object> & descendant) const override
    {
        double t_1 = -1;
        double t_2 = -1;

        Eigen::Vector3d centre_distance = ray.origin - this->center;
        double a = ray.direction.dot(ray.direction);
        double b = 2.0 * ray.direction.dot(centre_distance);
        double c = centre_distance.dot(centre_distance) - std::pow(this->radius, 2);

        double discriminant = std::pow(b, 2) - 4.0 * a * c;

        if (discriminant < 0.0)
          return false;

        if (discriminant == 0.0)
        {
          t_1 = -b / (2.0 * a);

          if (t_1 < min_t)
            return false;

          t = t_1;
          Eigen::Vector3d hit = ray.origin + t * (ray.direction);
          normal = (hit - this->center) / (this->radius);
          return true;
        }

        // discriminant > 0

        t_1 = (-b + std::sqrt(discriminant)) / (2.0 * a);
        t_2 = (-b - std::sqrt(discriminant)) / (2.0 * a);

        if (t_1 > min_t && t_2 > min_t)
        {
          if (t_1 > t_2)
          {
            t = t_2;
          }
          else
          {
            t = t_1;
          }
        }
        else if (t_1 > min_t)
        {
          t = t_1;
        }
        else if (t_2 > min_t)
        {
          t = t_2;
        }
        else
        {
          return false;
        }

        Eigen::Vector3d hit = ray.origin + t * (ray.direction);
        normal = (hit - this->center) / (this->radius);

        return true;

    };
};

#endif


#endif //RAYTRACING_SPHERE_H