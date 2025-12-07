#include "viewing_ray.h"
#include <iostream>

#define X 0
#define Y 1
#define Z 2

void viewing_ray(
    const Camera &camera,
    const int i,
    const int j,
    const int width,
    const int height,
    Ray &ray)
{
    // ray = eye + t * direction where t is a number belongs to [0.00)

    // structure of Ray
    //     struct Ray
    // {
    // Eigen::Vector3d origin;

    // Eigen::Vector3d direction;
    // };

    // structure of camera
    //     struct Camera
    // {
    //   // Origin or "eye"
    //   Eigen::Vector3d e;
    //   // orthonormal frame so that -w is the viewing direction.
    //   Eigen::Vector3d u,v,w;
    //   // image plane distance / focal length
    //   double d;
    //   // width and height of image plane
    //   double width, height;
    // };

    double u = (camera.width / (double)width) * ((double)j + 0.5) - (camera.width) / 2.0;
    double v = (camera.height) / 2.0 - (camera.height / (double)height) * ((double)i + 0.5);

    // origin is where the camera is located
    ray.origin = camera.e;

    // construct direction
    ray.direction = -camera.d * camera.w +
                    u * camera.u +
                    v * camera.v;

    // std::cout << "Pixel (" << i << ", " << j << "): ";
    // std::cout << "  Origin:    " << ray.origin.transpose();
    // std::cout << "  Direction: " << ray.direction.transpose() << std::endl;
}
