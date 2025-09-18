#include "blinn_phong_shading.h"
// Hint:
#include "first_hit.h"
#include <iostream>

Eigen::Vector3d blinn_phong_shading(
    const Ray &ray,
    const int &hit_id,
    const double &t,
    const Eigen::Vector3d &n,
    const std::vector<std::shared_ptr<Object>> &objects,
    const std::vector<std::shared_ptr<Light>> &lights)
{
  double epsilon = std::pow(10, -9);
  std::cout << "epsilon: " << epsilon << std::endl;

  Eigen::Vector3d point = ray.origin + t * ray.direction + epsilon * n;
  Eigen::Vector3d ks = objects[hit_id].get()->material.get()->ks;
  Eigen::Vector3d kd = objects[hit_id].get()->material.get()->kd;
  Eigen::Vector3d ka = objects[hit_id].get()->material.get()->ka;
  double phong_exp = objects[hit_id].get()->material.get()->phong_exponent;
  double ambient_factor = 0.1;

  Eigen::Vector3d color({0.0, 0.0, 0.0});

  for (auto &&light_ptr : lights)
  {
    const Light *light = light_ptr.get();
    Eigen::Vector3d direction;
    double t_max;
    light->direction(point, direction, t_max);
    Ray shadow_ray({point, direction});

    int hit;
    double t_temp;
    Eigen::Vector3d n_temp;

    if (first_hit(shadow_ray, epsilon, objects, hit, t_temp, n_temp))
    {
      std::cout << "t_max: " << t_max << std::endl;
      std::cout << "t_temp: " << t_temp << std::endl;
      if (t_max > t_temp)
      {

        //""
        std::cout << "should be black at here" << std::endl;
        continue;
      }
    }

    Eigen::Vector3d v = -(ray.direction.normalized());
    Eigen::Vector3d l = shadow_ray.direction.normalized();
    Eigen::Vector3d h = (l + v).normalized();

    color += light->I.cwiseProduct(ks) * std::pow(std::max(0.0, n.dot(h)), phong_exp);
    color += light->I.cwiseProduct(kd) * std::max(0.0, n.dot(l));
  }

  color += ka * ambient_factor;

  return color;
}
