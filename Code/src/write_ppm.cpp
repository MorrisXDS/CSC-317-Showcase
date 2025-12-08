#include "write_ppm.h"
#include <fstream>
#include <cassert>

bool write_ppm(
    const std::string &filename,
    const std::vector<unsigned char> &data,
    const int width,
    const int height,
    const int num_channels)
{
  assert(
      (num_channels == 3 || num_channels == 1) &&
      ".ppm only supports RGB or grayscale images");

  // take care of cases where either the data input
  // or the filename is not valid
  if (filename.empty() || data.empty())
  {
    return false;
  }

  std::ofstream ppm(filename);

  // return false if there is anything wrong
  // with the outfile operation
  if (ppm.fail() || ppm.bad())
  {
    return false;
  }

  if (num_channels == 3)
  {
    ppm << "P3" << std::endl;
  }
  else if (num_channels == 1)
  {
    ppm << "P2" << std::endl;
  }

  ppm << width << " " << height << std::endl;
  ppm << "255" << std::endl;

  for (int row = 0; row < height; row++)
  {
    for (int col = 0; col < width; col++)
    {
      int current_pixel_index = num_channels * (row * width + col);

      for (int chn = 0; chn < num_channels; chn++)
      {
        ppm << (int)data[current_pixel_index + chn] << " ";
      }
    }
    ppm << std::endl;
  }

  ppm.close();

  return true;
}
