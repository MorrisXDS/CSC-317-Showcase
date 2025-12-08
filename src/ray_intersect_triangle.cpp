#include <Eigen/Dense>
#include "ray_intersect_triangle.h"


#define ALPHA 0
#define BETA 1
#define STEP 2

bool ray_intersect_triangle(
  const Ray& ray,
  const Eigen::RowVector3d& A,
  const Eigen::RowVector3d& B,
  const Eigen::RowVector3d& C,
  const double min_t,
  const double max_t,
  Eigen::Vector3d & normal,
  double& t)
{
  const Eigen::Vector3d AB = (B - A).transpose();
  const Eigen::Vector3d AC = (C - A).transpose();
  normal = AB.cross(AC).normalized();

  Eigen::Matrix3d Equation;
  Equation << AB, AC, -ray.direction;
  Eigen::Vector3d x = Equation.colPivHouseholderQr().solve(ray.origin - A.transpose());

  if (x[ALPHA] < 0.0)
    return false;
  if (x[BETA] < 0.0)
    return false;
  if (x[ALPHA] + x[BETA] > 1.0)
    return false;

  if (x[STEP] < min_t)
    return false;

  if (x[STEP] > max_t)
  {
    return false;
  }

  t = x[STEP];

  return true;
}
