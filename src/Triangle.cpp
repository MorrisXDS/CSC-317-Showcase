#include "Triangle.h"
#include "Ray.h"
#include <Eigen/Dense>

#define LEFT 0
#define MID 1
#define RIGHT 2

#define ALPHA 0
#define BETA 1
#define STEP 2

bool Triangle::intersect(
    const Ray &ray, const double min_t, double &t, Eigen::Vector3d &n) const
{
  Eigen::Vector3d edge_1 = std::get<MID>(this->corners) - std::get<LEFT>(this->corners);
  Eigen::Vector3d edge_2 = std::get<RIGHT>(this->corners) - std::get<LEFT>(this->corners);

  Eigen::Matrix3d A;
  A << edge_1, edge_2, -ray.direction;
  Eigen::Vector3d x = A.colPivHouseholderQr().solve(ray.origin - std::get<LEFT>(this->corners));

  if (x[ALPHA] < 0.0)
    return false;
  if (x[BETA] < 0.0)
    return false;
  if (x[ALPHA] + x[BETA] > 1.0)
    return false;

  if (x[STEP] < min_t)
    return false;

  t = x[STEP];

  Eigen::Vector3d normal_vector = edge_1.cross(edge_2);

  n = (normal_vector) / (normal_vector.norm());

  return true;
}
