#include "PointLight.h"

void PointLight::direction(
    const Eigen::Vector3d &q, Eigen::Vector3d &d, double &max_t) const
{
  Eigen::Vector3d d_not_normalized = this->p - q;
  d = d_not_normalized.normalized();
  max_t = d_not_normalized.norm();
}
