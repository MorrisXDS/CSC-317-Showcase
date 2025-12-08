#ifndef MATERIAL_H
#define MATERIAL_H
#include <Eigen/Core>

// Blinn-Phong Approximate Shading Material Parameters
struct Material
{
  // Ambient, Diffuse, Specular, Mirror Color
  Eigen::Vector3d ka,kd,ks,km;
  // Phong exponent
  double phong_exponent;

  void operator=(const Material& other)
  {
    this->ka = other.ka;
    this->kd = other.kd;
    this->ks = other.ks;
    this->km = other.km;
    this->phong_exponent = other.phong_exponent;
  }

  bool operator==(const Material& other) const
  {
    if (this->ka != other.ka )
      return false;
    if (this->kd != other.kd )
      return false;
    if (this->ks != other.ks )
      return false;
    if (this->km != other.km )
      return false;
    if (this->phong_exponent != other.phong_exponent )
      return false;

    return true;
  }
};
#endif
