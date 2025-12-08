#include "insert_box_into_box.h"
#include <algorithm>

void insert_box_into_box(
    const BoundingBox& A,
    BoundingBox& B)
{
    B.min_corner = {
        std::min(B.min_corner.x(), A.min_corner.x()),
        std::min(B.min_corner.y(), A.min_corner.y()),
        std::min(B.min_corner.z(), A.min_corner.z()),
    };
    B.max_corner = {
        std::max(B.max_corner.x(), A.max_corner.x()),
        std::max(B.max_corner.y(), A.max_corner.y()),
        std::max(B.max_corner.z(), A.max_corner.z()),
    };
}

