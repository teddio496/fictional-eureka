#include "blinn_phong_shading.h"
// Hint:
#include "first_hit.h"
#include <iostream>

Eigen::Vector3d blinn_phong_shading(
    const Ray &ray,
    const int &hit_id,
    const double &t,
    const Eigen::Vector3d &n,
    const std::vector<std::shared_ptr<Object>> &objects,
    const std::vector<std::shared_ptr<Light>> &lights)
{

  Eigen::Vector3d result = 0.1 * objects[hit_id]->material->ka;
  Eigen::Vector3d point = ray.origin + ray.direction * t;
  Eigen::Vector3d v = (-ray.direction).normalized();

  for (auto &light : lights)
  {
    Eigen::Vector3d l;
    double max_t;
    light->direction(point, l, max_t);

    Ray shadowray;
    shadowray.origin = point + 0.001 * n;
    shadowray.direction = l;

    int hit_id2;
    double t2;
    Eigen::Vector3d n2;

    bool blocked = first_hit(shadowray, 0.001, objects, hit_id2, t2, n2) && t2/n2.norm() < max_t;

    if (!blocked)
    {
      // Diffuse (Lambert)
      Eigen::Vector3d id = ((objects[hit_id]->material->kd).array() * (light->I).array()).matrix() * std::max(0.0, n.normalized().dot(l.normalized()));

      // Specular (Blinn-Phong)
      Eigen::Vector3d h = (l + v).normalized();
      Eigen::Vector3d is = ((objects[hit_id]->material->ks).array() * (light->I).array()).matrix() * std::pow(std::max(0.0, n.normalized().dot(h)), objects[hit_id]->material->phong_exponent);
      result += id + is;
    }
  }

  return result.cwiseMin(1.0);
}
