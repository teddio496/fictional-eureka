#include "write_ppm.h"
#include <fstream>
#include <cassert>
#include <iostream>

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

  std::ofstream out(filename);
  if (!out)
    return false;

  // PPM Header
  out << ((num_channels == 1) ? "P2\n" : "P3\n")
      << width << " " << height << "\n"
      << "255\n";

  // PPM Data
  for (int h = 0; h < height; h++)
  {
    for (int w = 0; w < width; w++)
    {
      if (num_channels == 1)
      {
        out << (int)(data[w + h * width]) << " ";
      }
      else
      {
        for (int rgb = 0; rgb < 3; rgb++)
        {
          out << (int)(data[(w * 3) + (h * width * 3) + rgb]) << " ";
        }
      }
    }
    out << "\n";
  }

  out.close();
  return true;
}
