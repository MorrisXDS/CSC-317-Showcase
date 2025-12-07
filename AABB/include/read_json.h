#ifndef READ_JSON_H
#define READ_JSON_H

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
// Forward declaration
struct Object;
struct Light;

// Read a scene description from a .json file
//
// Input:
//   filename  path to .json file
// Output:
//   camera  camera looking at the scene
//   objects  list of shared pointers to objects
//   lights  list of shared pointers to lights
inline bool read_json(
  const std::string & filename, 
  Camera & camera,
  std::vector<std::shared_ptr<Object> > & objects,
  std::vector<std::shared_ptr<Light> > & lights);

// Implementation

#include <../../include/json.hpp>
#include "../../include/readSTL.h"
#include "../../include/dirname.h"
#include "../../include/Object.h"
#include "../../include/Sphere.h"
#include "../../include/Plane.h"
#include "../AABB/include/Triangle.h"
#include "../AABB/include/TriangleSoup.h"
#include "../../include/Light.h"
#include "../../include/PointLight.h"
#include "../../include/DirectionalLight.h"
#include "Material.h"
#include <Eigen/Geometry>
#include <fstream>
#include <iostream>
#include <cassert>

inline bool read_json(
  const std::string & filename, 
  Camera & camera,
  std::vector<std::shared_ptr<Object> > & objects,
  std::vector<std::shared_ptr<Light> > & lights)
{
  // Heavily borrowing from
  // https://github.com/yig/graphics101-raycasting/blob/master/parser.cpp
  using json = nlohmann::json;

  std::ifstream infile( filename );
  if( !infile ) return false;
  json j;
  infile >> j;


  // parse a vector
  auto parse_Vector3d = [](const json & j) -> Eigen::Vector3d
  {
    return Eigen::Vector3d(j[0],j[1],j[2]);
  };
  // parse camera
  auto parse_camera = 
    [&parse_Vector3d](const json & j, Camera & camera)
  {
    assert(j["type"] == "perspective" && "Only handling perspective cameras");
    camera.d = j["focal_length"].get<double>();
    camera.e =  parse_Vector3d(j["eye"]);
    camera.v =  parse_Vector3d(j["up"]).normalized();
    camera.w = -parse_Vector3d(j["look"]).normalized();
    camera.u = camera.v.cross(camera.w);
    camera.height = j["height"].get<double>();
    camera.width = j["width"].get<double>();
  };
  parse_camera(j["camera"],camera);

  // Parse materials
  std::unordered_map<std::string,std::shared_ptr<Material> > materials;
  auto parse_materials = [&parse_Vector3d](
    const json & j,
    std::unordered_map<std::string,std::shared_ptr<Material> > & materials)
  {
    materials.clear();
    for(const json & jmat : j)
    {
      std::string name = jmat["name"];
      // std::cout << name << std::endl;
      std::shared_ptr<Material> material(new Material());
      material->ka = parse_Vector3d(jmat["ka"]);
      material->kd = parse_Vector3d(jmat["kd"]);
      material->ks = parse_Vector3d(jmat["ks"]);
      material->km = parse_Vector3d(jmat["km"]);
      material->phong_exponent = jmat["phong_exponent"];
      materials[name] = material;
    }
  };
  parse_materials(j["materials"],materials);

  auto parse_lights = [&parse_Vector3d](
    const json & j,
    std::vector<std::shared_ptr<Light> > & lights)
  {
    lights.clear();
    for(const json & jlight : j)
    {
      if(jlight["type"] == "directional")
      {
        std::shared_ptr<DirectionalLight> light(new DirectionalLight());
        light->d = parse_Vector3d(jlight["direction"]).normalized();
        light->I = parse_Vector3d(jlight["color"]);
        lights.push_back(light);
      }else if(jlight["type"] == "point")
      {
        std::shared_ptr<PointLight> light(new PointLight());
        light->p = parse_Vector3d(jlight["position"]);
        light->I = parse_Vector3d(jlight["color"]);
        lights.push_back(light);
      }
    }
  };
  parse_lights(j["lights"],lights);

  auto parse_objects = [&parse_Vector3d,&filename,&materials](
    const json & j,
    std::vector<std::shared_ptr<Object> > & objects)
  {
    objects.clear();
    int size = -1;
    for(const json & jobj : j)
    {
      if(jobj["type"] == "sphere")
      {
        // std::shared_ptr<Sphere> sphere(new Sphere());
        // sphere->center = parse_Vector3d(jobj["center"]);
        // sphere->radius = jobj["radius"].get<double>();
        // objects.push_back(sphere);
        std::cerr << "type "<< jobj["type"] << "not supported !" << std::endl;
      }else if(jobj["type"] == "plane")
      {
        // std::shared_ptr<Plane> plane(new Plane());
        // plane->point = parse_Vector3d(jobj["point"]);
        // plane->normal = parse_Vector3d(jobj["normal"]).normalized();
        std::cerr << "type "<< jobj["type"] << "not supported !" << std::endl;
        // objects.push_back(plane);
      }else if(jobj["type"] == "triangle")
      {
        std::shared_ptr<Triangle> tri(new Triangle());
        tri->corners = std::make_tuple(
          parse_Vector3d(jobj["corners"][0]),
          parse_Vector3d(jobj["corners"][1]),
          parse_Vector3d(jobj["corners"][2]));
        objects.push_back(tri);
      }else if(jobj["type"] == "soup")
      {
        std::vector<std::vector<double> > V;
        std::vector<std::vector<double> > F;
        std::vector<std::vector<int> > N;
        {
#if defined(WIN32) || defined(_WIN32)
#define PATH_SEPARATOR std::string("\\")
#else
#define PATH_SEPARATOR std::string("/")
#endif
          const std::string stl_path = jobj["stl"];
          igl::readSTL(
              igl::dirname(filename)+
              PATH_SEPARATOR +
              stl_path,
              V,F,N);
        }
        
        int start_position = objects.size();
        int end_position = start_position + F.size();
        for(int f = 0;f<F.size();f++)
        {
          std::shared_ptr<Triangle> tri(new Triangle());
          tri->corners = std::make_tuple(
            Eigen::Vector3d( V[F[f][0]][0], V[F[f][0]][1], V[F[f][0]][2]),
            Eigen::Vector3d( V[F[f][1]][0], V[F[f][1]][1], V[F[f][1]][2]),
            Eigen::Vector3d( V[F[f][2]][0], V[F[f][2]][1], V[F[f][2]][2])
          );
          objects.push_back(tri);
        }

        if (materials.count(jobj["material"]))
        {
          for(int idx = start_position;idx < end_position;idx++)
          {
            objects[idx]->material = materials[jobj["material"]];
          }
        }

      }
      //objects.back()->material = default_material;
      if(jobj.count("material"))
      {
        if(materials.count(jobj["material"]) && jobj["type"] != "soup")
        {
          objects.back()->material = materials[jobj["material"]];
        }
      }
    }
  };
  parse_objects(j["objects"],objects);

  return true;
}

#endif 
