#include "DirectionalLight.h"
#include <limits>

void DirectionalLight::direction(
    const Eigen::Vector3d &q, Eigen::Vector3d &d, double &max_t) const
{
  Eigen::Vector3d d_not_normalized = -this->d;
  d = d_not_normalized.normalized();
  max_t = std::numeric_limits<double>::infinity();
}
