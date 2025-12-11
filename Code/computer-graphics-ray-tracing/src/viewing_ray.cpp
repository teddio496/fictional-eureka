#include "viewing_ray.h"

void viewing_ray(
    const Camera &camera,
    const int i,
    const int j,
    const int width,
    const int height,
    Ray &ray)
{
  double u_s = camera.width * ((j + 0.5) / width - 0.5);
  double v_s = -camera.height * ((i + 0.5) / height - 0.5);

  ray.origin = camera.e;
  ray.direction = u_s * camera.u + v_s * camera.v - camera.d * camera.w;
}
