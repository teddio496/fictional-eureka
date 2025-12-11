#include "Triangle.h"
#include "Ray.h"
#include <Eigen/Dense>

bool Triangle::intersect(
    const Ray &ray, const double min_t, double &t, Eigen::Vector3d &n) const
{
  // using the derivation from the textbook based on cramer's thing
  const auto &[A, B, C] = corners;

  const Eigen::Vector3d e1 = B - A;
  const Eigen::Vector3d e2 = C - A;
  const Eigen::Vector3d md = -ray.direction;
  const Eigen::Vector3d r = ray.origin - A;

  const double M = e1.dot(e2.cross(md));
  if (M == 0)
    return false;

  const double beta = r.dot(e2.cross(md)) / M;
  const double gamma = e1.dot(r.cross(md)) / M;
  const double tt = e1.dot(e2.cross(r)) / M;

  if (beta < 0 || gamma < 0 || beta + gamma > 1.0)
    return false;
  if (tt < min_t)
    return false;

  t = tt;
  n = e1.cross(e2).normalized();

  return true;
}
