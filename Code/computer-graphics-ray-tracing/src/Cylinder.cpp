#include "Cylinder.h"
#include "Ray.h"
#include <cmath>
#include <iostream>

bool Cylinder::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  Eigen::Vector3d d_vec = axis_b - axis_a;
  double height = d_vec.norm();
  if (height == 0) return false;
  Eigen::Vector3d axis = d_vec / height;

  Eigen::Vector3d oc = ray.origin - axis_a;
  
  // Coefficients for the quadratic equation at^2 + bt + c = 0
  // We project the ray onto the plane perpendicular to the axis
  Eigen::Vector3d ray_d_proj = ray.direction - ray.direction.dot(axis) * axis;
  Eigen::Vector3d oc_proj = oc - oc.dot(axis) * axis;

  double a = ray_d_proj.dot(ray_d_proj);
  double b = 2.0 * oc_proj.dot(ray_d_proj);
  double c = oc_proj.dot(oc_proj) - radius * radius;

  double t_body = std::numeric_limits<double>::infinity();
  bool hit_body = false;

  if (a > 1e-9) {
    double discriminant = b * b - 4 * a * c;
    if (discriminant >= 0) {
      double sqrt_d = sqrt(discriminant);
      double t1 = (-b - sqrt_d) / (2 * a);
      double t2 = (-b + sqrt_d) / (2 * a);

      auto check_body = [&](double t_cand) -> bool {
        if (t_cand < min_t) return false;
        Eigen::Vector3d p = ray.origin + t_cand * ray.direction;
        double proj = (p - axis_a).dot(axis);
        if (proj >= 0 && proj <= height) {
          return true;
        }
        return false;
      };

      if (check_body(t1)) {
        t_body = t1;
        hit_body = true;
      } else if (check_body(t2)) {
        t_body = t2;
        hit_body = true;
      }
    }
  }

  // Check caps
  double t_cap = std::numeric_limits<double>::infinity();
  Eigen::Vector3d n_cap;
  bool hit_cap = false;

  // Cap A (at axis_a, normal -axis)
  double denom_a = ray.direction.dot(-axis);
  if (denom_a > 1e-9) { // Ray opposing normal
    double t_a = (axis_a - ray.origin).dot(-axis) / denom_a;
    if (t_a >= min_t) {
      Eigen::Vector3d p = ray.origin + t_a * ray.direction;
      if ((p - axis_a).squaredNorm() <= radius * radius) {
        t_cap = t_a;
        n_cap = -axis;
        hit_cap = true;
      }
    }
  }

  // Cap B (at axis_b, normal axis)
  double denom_b = ray.direction.dot(axis);
  if (denom_b < -1e-9) { // Ray opposing normal
    double t_b = (axis_b - ray.origin).dot(axis) / denom_b;
    if (t_b >= min_t) {
      Eigen::Vector3d p = ray.origin + t_b * ray.direction;
      if ((p - axis_b).squaredNorm() <= radius * radius) {
        if (t_b < t_cap) {
          t_cap = t_b;
          n_cap = axis;
          hit_cap = true;
        }
      }
    }
  }

  if (!hit_body && !hit_cap) return false;

  if (hit_body && (!hit_cap || t_body < t_cap)) {
    t = t_body;
    Eigen::Vector3d p = ray.origin + t * ray.direction;
    Eigen::Vector3d proj_p = axis_a + (p - axis_a).dot(axis) * axis;
    n = (p - proj_p).normalized();
    return true;
  } else if (hit_cap) {
    t = t_cap;
    n = n_cap;
    return true;
  }

  return false;
}
