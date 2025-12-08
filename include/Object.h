#ifndef OBJECT_H
#define OBJECT_H

// Empty "boxable" and hierarchical Object class
//
#include "BoundingBox.h"
#include <Eigen/Core>
#include <memory>
#include "Material.h"

struct Ray;
struct Object
{
  // enum class type
  // {
  //   triangle = 1,
  //   plane = 2,
  //   sphere = 3,
  //   generic = 4,
  // };
  //
  // type object_type = type::generic;
  BoundingBox box;
  std::shared_ptr<Material> material;
  // Intersect this Object with a ray
  //
  // Inputs:
  //   ray  ray being shot
  //   min_t  minimum parametric distance along ray to consider 
  //   max_t  maximum parametric distance along ray to consider 
  // Object:
  //   t  parameteric distance of hit along ray
  //   descendant  if the hit was found at a descendant, pass along a pointer to
  //     that descendant (only necessary for hierarchical Objects)
  // Returns true iff a valid hit was recorded
  virtual bool ray_intersect(
    const Ray& ray,
    const double min_t,
    const double max_t,
    Eigen::Vector3d & normal,
    double & t,
    std::shared_ptr<Object> & descendant) const = 0;
};

#endif
