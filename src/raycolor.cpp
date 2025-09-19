#include "raycolor.h"
#include "first_hit.h"
#include "blinn_phong_shading.h"
#include "reflect.h"
#include <iostream>

bool raycolor(
    const Ray &ray,
    const double min_t,
    const std::vector<std::shared_ptr<Object>> &objects,
    const std::vector<std::shared_ptr<Light>> &lights,
    const int num_recursive_calls,
    Eigen::Vector3d &rgb)
{
  int hit_id;
  double t;
  Eigen::Vector3d n;
  double epsilon = std::pow(10, -9);

  if (first_hit(ray, min_t, objects, hit_id, t, n))
  {
    rgb += blinn_phong_shading(ray, hit_id, t, n, objects, lights);
    if (num_recursive_calls == 0)
      return true;

    Eigen::Vector3d origin = ray.origin + t * ray.direction + epsilon * n;
    Ray mirrored_ray({origin, reflect(ray.direction, n)});
    Eigen::Vector3d rgb_reflected({0, 0, 0});

    raycolor(mirrored_ray, 0, objects, lights, num_recursive_calls - 1, rgb_reflected);
    rgb += objects[hit_id].get()->material.get()->km.cwiseProduct(rgb_reflected);
    return true;
  }

  return false;
}
