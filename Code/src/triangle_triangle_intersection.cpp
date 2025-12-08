#include "triangle_triangle_intersection.h"
#include <algorithm>

inline Eigen::RowVector3d cross_product(const Eigen::RowVector3d& a, const Eigen::RowVector3d& b)
{
    return Eigen::RowVector3d(
        a.y() * b.z() - a.z() * b.y(),
        a.z() * b.x() - a.x() * b.z(),
        a.x() * b.y() - a.y() * b.x()
        );
}

void triangle_interval_onto_axis(const std::vector<Eigen::RowVector3d>& vertices,
                                 const Eigen::RowVector3d& axis,
                                 double& min, double& max)
{
    min = std::numeric_limits<double>::infinity();
    max = std::numeric_limits<double>::lowest();

    for (const auto& vertex : vertices)
    {
        double projection = vertex.dot(axis);
        min = std::min(min, projection);
        max = std::max(max, projection);
    }
}

inline bool is_intervals_overlapped(const double& minA, const double& maxA,
                                    const double& minB, const double& maxB)
{
    return !(maxA < minB || maxB < minA);
}

bool triangle_triangle_intersection(
    const Eigen::RowVector3d& A0,
    const Eigen::RowVector3d& A1,
    const Eigen::RowVector3d& A2,
    const Eigen::RowVector3d& B0,
    const Eigen::RowVector3d& B1,
    const Eigen::RowVector3d& B2)
{
    // reference:
    // https://www.r-5.org/files/books/computers/algo-list/realtime-3d/Christer_Ericson-Real-Time_Collision_Detection-EN.pdf
    // Page 173 Testing Triangle Against Triangle. Second Approach
    const std::vector<Eigen::RowVector3d> A_vertices = {A0, A1, A2};
    const std::vector<Eigen::RowVector3d> B_vertices = {B0, B1, B2};

    const double epsilon = std::pow(10, -9);

    const std::vector<Eigen::RowVector3d> A_edges = {
        A1 - A0,
        A2 - A1,
        A0 - A2
    };

    const std::vector<Eigen::RowVector3d> B_edges = {
        B1 - B0,
        B2 - B1,
        B0 - B2
    };

    const Eigen::RowVector3d normal_A = cross_product(A_edges[0], A_edges[1]);
    const Eigen::RowVector3d normal_B = cross_product(B_edges[0], B_edges[1]);

    double min_terminal_A, max_terminal_A, min_terminal_B, max_terminal_B;

    if (normal_A.norm() > epsilon)
    {
        Eigen::RowVector3d axis = normal_A.normalized();
        triangle_interval_onto_axis(A_vertices, axis, min_terminal_A, max_terminal_A);
        triangle_interval_onto_axis(B_vertices, axis, min_terminal_B, max_terminal_B);
        if (!is_intervals_overlapped(min_terminal_A, max_terminal_A, min_terminal_B, max_terminal_B))
            return false;
    }

    if (normal_B.norm() > epsilon)
    {
        Eigen::RowVector3d axis = normal_B.normalized();
        triangle_interval_onto_axis(A_vertices, axis, min_terminal_A, max_terminal_A);
        triangle_interval_onto_axis(B_vertices, axis, min_terminal_B, max_terminal_B);
        if (!is_intervals_overlapped(min_terminal_A, max_terminal_A, min_terminal_B, max_terminal_B))
            return false;
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            Eigen::RowVector3d axis = cross_product(A_edges[i], B_edges[j]);

            if (axis.norm() < epsilon)
                continue;

            axis.normalize();

            triangle_interval_onto_axis(A_vertices, axis, min_terminal_A, max_terminal_A);
            triangle_interval_onto_axis(B_vertices, axis, min_terminal_B, max_terminal_B);

            if (!is_intervals_overlapped(min_terminal_A, max_terminal_A, min_terminal_B, max_terminal_B))
                return false;
        }
    }

    return true;
}
