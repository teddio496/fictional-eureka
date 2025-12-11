#include "first_hit.h"

bool first_hit(
    const Ray &ray,
    const double min_t,
    const std::vector<std::shared_ptr<Object>> &objects,
    int &hit_id,
    double &t,
    Eigen::Vector3d &n)
{
  double min = std::numeric_limits<double>::infinity();
  for (size_t i = 0; i < objects.size(); i++)
  {
    double temp_t;
    Eigen::Vector3d temp_n;
    if (objects[i]->intersect(ray, min_t, temp_t, temp_n) && temp_t >= min_t && temp_t < min)
    {
      min = temp_t;
      t = temp_t;
      n = temp_n;
      hit_id = i;
    }
  }
  if (min == std::numeric_limits<double>::infinity())
  {
    return false;
  }
  return true;
}
