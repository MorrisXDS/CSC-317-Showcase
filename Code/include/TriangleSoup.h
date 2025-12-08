#ifndef TRIANGLE_SOUP_H
#define TRIANGLE_SOUP_H

#include "Object.h"
#include <Eigen/Core>
#include <memory>
#include <vector>

// Forward declaration
class Triangle;
class TriangleSoup : public Object
{
  public:
    // A soup is just a set (list) of triangles
    std::vector<std::shared_ptr<Object> > triangles;

    bool ray_intersect(
    const Ray& ray,
    const double min_t,
    const double max_t,
    Eigen::Vector3d & normal,
    double & t,
    std::shared_ptr<Object> & descendant) const override
    {
        assert(false && "place holder function for api compatibility. acutal intersection check is done by AABBtree!!\n");
        return false;
    };
};

#endif

