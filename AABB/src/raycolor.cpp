#include "../include/raycolor.h"
#include "first_hit.h"
#include "../include/blinn_phong_shading.h"
#include "reflect.h"
#include "Triangle.h"
#include <Eigen/Dense>

#define NUMBER_RECURSIVE_CALLS 15

bool raycolor(
  const Ray & ray,
  const double min_t,
  const std::shared_ptr<AABBTree>& root,
  const std::vector< std::shared_ptr<Light> > & lights,
  const int num_recursive_calls,
  Eigen::Vector3d & rgb)
{
  int hit_id;
  double t;
  Eigen::Vector3d n;
  double epsilon = std::pow(10, -9);

  std::shared_ptr<Object> descendant;
  bool found = root->ray_intersect(ray, min_t, std::numeric_limits<double>::infinity(),t, descendant);

  if (found)
  {
    // for triangle only !
    auto triangle = std::dynamic_pointer_cast<Triangle>(descendant);
    Eigen::Vector3d edge_1 = std::get<1>(triangle->corners) - std::get<0>(triangle->corners);
    Eigen::Vector3d edge_2 = std::get<2>(triangle->corners) - std::get<0>(triangle->corners);
    n = (edge_1.cross(edge_2)).normalized();

    // add blinn_phong_shading when a hit occurs
    rgb += blinn_phong_shading(ray, descendant, t, n, root, lights);
    if (num_recursive_calls == NUMBER_RECURSIVE_CALLS)
      // end of recursive call of reflections
      return true;

    // add an epsilon normal vector value to avoid FPRE
    // (Floating Point Representation Errors)
    Eigen::Vector3d origin = ray.origin + t * ray.direction + epsilon * n;
    // construct a mirror ray for reflection
    Ray mirrored_ray({origin, reflect(ray.direction, n)});
    Eigen::Vector3d rgb_reflected({0, 0, 0});

    raycolor(mirrored_ray, epsilon, root, lights, num_recursive_calls + 1, rgb_reflected);
    // reflected color is revealed with the mirror coefficient of the object
    // on which reflection happens
    rgb += descendant->material->km.cwiseProduct(rgb_reflected);
    return true;
  }

  return false;
}
