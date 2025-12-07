#include "../include/blinn_phong_shading.h"
// Hint:
#include "first_hit.h"

double double_infinity = std::numeric_limits<double>::infinity();

Eigen::Vector3d blinn_phong_shading(
    const Ray &ray,
    std::shared_ptr<Object> hit_object,
    const double &t,
    const Eigen::Vector3d &n,
    const std::shared_ptr<AABBTree> root,
    const std::vector<std::shared_ptr<Light>> &lights)
{
  // epsilon idea credit: tutorial
  double epsilon = std::pow(10, -9);

  // added an epsilon amount along the normal vector to avoid
  // floating point representation errors
  Eigen::Vector3d point = ray.origin + t * ray.direction + epsilon * n;
  Eigen::Vector3d ks = hit_object->material->ks;
  Eigen::Vector3d kd = hit_object->material->kd;
  Eigen::Vector3d ka = hit_object->material->ka;
  double phong_exp = hit_object->material->phong_exponent;
  double ambient_factor = 0.1;

  Eigen::Vector3d color({0.0, 0.0, 0.0});

  std::shared_ptr<Object> descendant;

  for (auto &&light_ptr : lights)
  {
    const Light *light = light_ptr.get();
    Eigen::Vector3d direction;
    double t_max;
    // get direction from point to light
    light->direction(point, direction, t_max);
    Ray shadow_ray({point, direction});

    int hit;
    double t_temp;
    Eigen::Vector3d n_temp;

    if ( root->ray_intersect(ray,0, double_infinity, t_temp, descendant))
    {
      if (t_max > t_temp)
      {
        continue;
      }
    }

    Eigen::Vector3d v = -(ray.direction.normalized());
    Eigen::Vector3d l = shadow_ray.direction.normalized();
    Eigen::Vector3d h = (l + v).normalized();

    // add specular light and diffusion light components for the correspondent light
    color += light->I.cwiseProduct(ks) * std::pow(std::max(0.0, n.dot(h)), phong_exp);
    color += light->I.cwiseProduct(kd) * std::max(0.0, n.dot(l));
  }

  // ambient light, independent of light source
  color += ka * ambient_factor;

  return color;
}
