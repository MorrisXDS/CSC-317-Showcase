#include "../include/raycolor.h"
#include "../include/blinn_phong_shading.h"
#include "reflect.h"
#include "Triangle.h"
#include <Eigen/Dense>

bool raycolor(
  const Ray & ray,
  const double min_t,
  const std::shared_ptr<AABBTree>& root,
  const std::vector< std::shared_ptr<Light> > & lights,
  const int num_recursive_calls,
  Eigen::Vector3d & rgb)
{
  double t;
  Eigen::Vector3d n;
  double epsilon = std::pow(10, -9);

  std::shared_ptr<Object> descendant;
  bool found = root->ray_intersect(ray, min_t, std::numeric_limits<double>::infinity(),n, t, descendant);

  if (found)
  {
    // add blinn_phong_shading when a hit occurs
    rgb += blinn_phong_shading(ray, descendant, t, n, root, lights);
    if (num_recursive_calls == 0)
      // end of recursive call of reflections
      return true;

    // add an epsilon normal vector value to avoid FPRE
    // (Floating Point Representation Errors)
    Eigen::Vector3d origin = ray.origin + t * ray.direction + epsilon * n;
    // construct a mirror ray for reflection
    Ray mirrored_ray({origin, reflect(ray.direction, n)});
    Eigen::Vector3d rgb_reflected({0, 0, 0});

    raycolor(mirrored_ray, epsilon, root, lights, num_recursive_calls - 1, rgb_reflected);
    // reflected color is revealed with the mirror coefficient of the object
    // on which reflection happens
    rgb += descendant->material->km.cwiseProduct(rgb_reflected);
    return true;
  }

  return false;
}
