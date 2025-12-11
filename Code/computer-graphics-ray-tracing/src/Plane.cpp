#include "Plane.h"
#include "Ray.h"

bool Plane::intersect(
    const Ray &ray, const double min_t, double &t, Eigen::Vector3d &n) const
{
  double nd = ray.direction.dot(normal);
  if (nd == 0)
  {
    return false;
  }

  t = (normal.dot(point - ray.origin)) / nd;
  if (t < min_t)
  {
    return false;
  }

  n = normal;

  return true;
}