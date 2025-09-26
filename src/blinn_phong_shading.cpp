#include "blinn_phong_shading.h"
// Hint:
#include "first_hit.h"

Eigen::Vector3d blinn_phong_shading(
    const Ray &ray,
    const int &hit_id,
    const double &t,
    const Eigen::Vector3d &n,
    const std::vector<std::shared_ptr<Object>> &objects,
    const std::vector<std::shared_ptr<Light>> &lights)
{
  // epsilon idea credit: tutorial
  double epsilon = std::pow(10, -9);

  // added an epsilon amount along the normal vector to avoid
  // floating point representation errors
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
    // get direction from point to light
    light->direction(point, direction, t_max);
    Ray shadow_ray({point, direction});

    int hit;
    double t_temp;
    Eigen::Vector3d n_temp;

    if (first_hit(shadow_ray, 0, objects, hit, t_temp, n_temp))
    {
      // if the object hit is in our way to reach the light source,
      // we are blocked by the object
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
