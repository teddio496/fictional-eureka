#include "Sphere.h"
#include "Ray.h"
#include <cmath>

bool Sphere::intersect(
    const Ray &ray, const double min_t, double &t, Eigen::Vector3d &n) const
{
  double a = ray.direction.dot(ray.direction);
  double b = 2 * ray.direction.dot(ray.origin - center);
  double c = (ray.origin - center).dot(ray.origin - center) - radius * radius;

  double discriminant = b * b - 4 * a * c;
  if (discriminant < 0)
  {
    return false;
  }

  double t1, t2;
  if (discriminant == 0)
  {
    t1 = t2 = (-b) / (2 * a);
  }
  else
  {
    t1 = (-b - std::sqrt(discriminant)) / (2 * a);
    t2 = (-b + std::sqrt(discriminant)) / (2 * a);
  }

  if (t1 >= min_t)
  {
    t = t1;
  }
  else if (t2 >= min_t)
  {
    t = t2;
  }
  else
  {
    return false;
  }

  Eigen::Vector3d intersection_point = ray.origin + t * ray.direction;
  n = (intersection_point - center).normalized();
  return true;
}
