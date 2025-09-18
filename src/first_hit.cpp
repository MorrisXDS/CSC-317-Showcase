#include "first_hit.h"
#include <limits>

bool first_hit(
    const Ray &ray,
    const double min_t,
    const std::vector<std::shared_ptr<Object>> &objects,
    int &hit_id,
    double &t,
    Eigen::Vector3d &n)
{
  size_t num_objects = objects.size();

  // temp values
  double t_smallest = std::numeric_limits<double>::max();
  double t_temp = 0;
  hit_id = -1;

  for (size_t idx = 0; idx < num_objects; idx++)
  {
    if (objects[idx].get()->intersect(ray, min_t, t_temp, n))
    {
      if (t_temp < t_smallest && t_temp > min_t)
      {
        t_smallest = t_temp;
        hit_id = idx;
      }
    }
  }

  if (hit_id == -1)
    return false;

  objects[hit_id].get()->intersect(ray, min_t, t, n);
  return true;
}
