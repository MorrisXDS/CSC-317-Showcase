#include "insert_triangle_into_box.h"
#include <algorithm>


void insert_triangle_into_box(
    const Eigen::RowVector3d& a,
    const Eigen::RowVector3d& b,
    const Eigen::RowVector3d& c,
    BoundingBox& B)
{
    // std::cout << B.min_corner;
    // get min_x
    B.min_corner.x() = std::min({B.min_corner.x(), a.x(), b.x(), c.x()});
    B.min_corner.y() = std::min({B.min_corner.y(), a.y(), b.y(), c.y()});
    B.min_corner.z() = std::min({B.min_corner.z(), a.z(), b.z(), c.z()});

    B.max_corner.x() = std::max({B.max_corner.x(), a.x(), b.x(), c.x()});
    B.max_corner.y() = std::max({B.max_corner.y(), a.y(), b.y(), c.y()});
    B.max_corner.z() = std::max({B.max_corner.z(), a.z(), b.z(), c.z()});
}


