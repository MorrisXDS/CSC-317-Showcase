#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "read_json.h"
#include "write_ppm.h"
#include "viewing_ray.h"
#include "raycolor.h"
#include "file_name.h"
#include <Eigen/Core>
#include <vector>
#include <iostream>
#include <memory>
#include <limits>
#include <functional>
#include <omp.h>
#include "AABBTree.h"
#include "dirname.h"

int main(int argc, char *argv[])
{
  Camera camera;
  std::vector<std::shared_ptr<Object>> objects;
  std::vector<std::shared_ptr<Light>> lights;
  // Read a camera and scene description from given .json file
  read_json(
      argc <= 1 ? "../data/sphere-and-plane.json" : argv[1],
      camera,
      objects,
      lights);

  std::shared_ptr<AABBTree> root = std::make_shared<AABBTree>(objects);

  std::string filename;
  if (argc < 2)
  {
    filename = "sphere-and-plane.";
  }
  else
  {
    filename = extractFilename(argv[1]) + ".";
  }

  int num_threads = omp_get_num_procs();
  omp_set_num_threads(num_threads);

  int width = 640;
  int height = 360;
  std::vector<unsigned char> rgb_image(3 * width * height);
  // For each pixel (i,j)
#pragma  omp parallel for collapse(2) schedule(static) proc_bind(close)
  for (unsigned i = 0; i < height; ++i)
  {
    for (unsigned j = 0; j < width; ++j)
    {
      // Set background color
      Eigen::Vector3d rgb(0, 0, 0);

      // Compute viewing ray
      Ray ray;
      viewing_ray(camera, i, j, width, height, ray);

      // Shoot ray and collect color
      raycolor(ray, 1.0, root, lights, 10, rgb);

      // Write double precision color into image
      auto clamp = [](double s)
      { return std::max(std::min(s, 1.0), 0.0); };
      rgb_image[0 + 3 * (j + width * i)] = 255.0 * clamp(rgb(0));
      rgb_image[1 + 3 * (j + width * i)] = 255.0 * clamp(rgb(1));
      rgb_image[2 + 3 * (j + width * i)] = 255.0 * clamp(rgb(2));
    }
  }

  write_ppm(filename + "rgb.ppm", rgb_image, width, height, 3);
  std::cout<<"the generated file name is "<<filename<<"rgb.ppm"<< std::endl;
  return 0;
}
