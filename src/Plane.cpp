// #include "../include/Plane.h"
// #include "Ray.h"
//
// bool Plane::ray_intersect(
//     const Ray &ray, const double min_t, double &t, Eigen::Vector3d &n) const
// {
//   double denominator = this->normal.dot(ray.direction);
//
//   // if denominator is 0, the camera is looking parallel to the plane.
//   // there is no way the ray intersects with the plane
//   if (denominator == 0.0)
//     return false;
//
//   Eigen::Vector3d point_to_origin = ray.origin - this->point;
//   double numerator = -this->normal.dot(point_to_origin);
//
//   double t_temp = numerator / denominator;
//   if (t_temp < min_t)
//     return false;
//
//   t = t_temp;
//   n = this->normal / (this->normal.norm());
//
//   return true;
// }
