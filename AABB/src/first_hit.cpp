// #include "../include/first_hit.h"
// #include <limits>
//
//
//
// bool first_hit(
//   const Ray & ray,
//   const double min_t,
//   const std::shared_ptr<AABBTree>& root,
//   double & t,
//   Eigen::Vector3d & n)
// {
//   // temp values
//   double t_smallest = std::numeric_limits<double>::max();
//   double t_temp = 0;
//   std::shared_ptr<Object> descendant;
//   if (! root->ray_intersect(ray,min_t, double_infinity, t, descendant))
//     return false;
//
//   return true;
// }
