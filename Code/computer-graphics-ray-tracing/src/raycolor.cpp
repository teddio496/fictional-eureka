#include "raycolor.h"
#include "first_hit.h"
#include "blinn_phong_shading.h"
#include "reflect.h"

bool raycolor(
    const Ray &ray,
    const double min_t,
    const std::vector<std::shared_ptr<Object>> &objects,
    const std::vector<std::shared_ptr<Light>> &lights,
    const int num_recursive_calls,
    Eigen::Vector3d &rgb)
{
  Eigen::Vector3d n;
  int hit_id;
  double t;
  if (!first_hit(ray, min_t, objects, hit_id, t, n))
    return false;

  rgb = blinn_phong_shading(ray, hit_id, t, n, objects, lights);

  // Mirror reflection (raytracing)
  if (num_recursive_calls < 10)
  {
    const auto &material = objects[hit_id]->material;
    if (material && material->km.norm() > 0.0001)
    {
      Ray reflected_ray;
      reflected_ray.origin = ray.origin + t * ray.direction + 0.0001 * n;
      reflected_ray.direction = reflect(ray.direction.normalized(), n.normalized());

      Eigen::Vector3d reflected_rgb;
      if (raycolor(reflected_ray, 0.0001, objects, lights, num_recursive_calls + 1, reflected_rgb))
      {
        rgb += (material->km.array() * reflected_rgb.array()).matrix();
      }
    }
  }

  return true;
}
