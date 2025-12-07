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

    virtual bool point_squared_distance(
    const Eigen::RowVector3d & query,
    const double min_sqrd,
    const double max_sqrd,
    double & sqrd,
    std::shared_ptr<Object> & descendant) const override
        {
            assert(false && "you should never call point_squared_distance on TriangleSoup!\n");
            return false;
        };

    bool ray_intersect(
    const Ray& ray,
    const double min_t,
    const double max_t,
    double & t,
    std::shared_ptr<Object> & descendant) const override
    {
        assert(false && "place holder function for api compatibility. acutal intersection check is done by AABBtree!!\n");
        return false;
    };
};

#endif

