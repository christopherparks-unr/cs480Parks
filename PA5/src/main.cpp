#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>

using namespace std;

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>

#include "graphics.h"
#include "global.h"
#include "engine.h"

/*
std::vector< aiScene* > scene_vector;
std::vector< std::vector<std::string> > object_vector;
std::string cur_name;
std::string cur_obj_path;
std::string cur_parent;
float cur_scale;
float cur_rot_spd;
float cur_orbit_rad;
float cur_orbit_spd;
*/


std::string replaceOccurrences(std::string& s, const std::string& toReplace, const std::string& replaceWith)
{
    while(s.find(toReplace) != std::string::npos)
    {
      std::size_t pos = s.find(toReplace);
      s = s.replace(pos, toReplace.length(), replaceWith);
    }
    return s;
}


int main(int argc, char **argv)
{
  //Define default path strings for vertex and fragment shaders
  std::string v = "../assets/shaders/vertex_pa1.txt";
  std::string f = "../assets/shaders/fragment_pa1.txt";
  std::string o = "cube.obj";
  //Parse through command line arguments and search for alternate paths to shaders
  int cur_arg = 2;
  if(argc > 1)
  {
    while(cur_arg <= argc)
    {
      if(cur_arg + 1 <= argc && strcmp(argv[cur_arg - 1], "-v") == 0)
      {
        v = argv[cur_arg];
        cur_arg += 2;
      }
      else if(cur_arg + 1 <= argc && strcmp(argv[cur_arg - 1], "-f") == 0)
      {
        f = argv[cur_arg];
        cur_arg += 2;
      }
      else
      {
        cur_arg++;
      }
      
    }
  }
  std::cout << "Vertex Shader: " << v << std::endl;
  std::cout << "Fragment Shader: " << f << std::endl;
  std::cout << "Object: " << o << std::endl;


  //String that contains the directory currently being read from file
  std::string line;
  //Input stream class for reading from file
  std::ifstream dir_list;
  //Open the file, path determined by function call in graphics.cpp
  Assimp::Importer importer;
  dir_list.open("../config.xml");
  int obj_num = 0;



  while(getline(dir_list,line))
  {
    line = replaceOccurrences(line, " ", "");
    line = replaceOccurrences(line, " ", "");
    if(line.find("[") != std::string::npos)
    {
      if(obj_num > 0)
      {
        std::vector<std::string> tempVec = {cur_name, cur_obj_path, cur_parent, cur_scale, cur_rot_spd, cur_orbit_rad, cur_orbit_spd};
        scene_vector.push_back(importer.ReadFile(cur_obj_path, aiProcessPreset_TargetRealtime_Fast));
        object_vector.push_back(tempVec);
        obj_num++;
      }
      line = replaceOccurrences(line, "[", "");
      cur_name = replaceOccurrences(line, "]", "");
      cur_obj_path = "../assets/objects/cube.obj";
      cur_parent = "nullptr";
      cur_scale = "1.0";
      cur_rot_spd = "1.0";
      cur_orbit_rad = "0.0";
      cur_orbit_spd = "1.0";
    }
    else if(line.find("obj_path=") != std::string::npos)
    {
      cur_obj_path = replaceOccurrences(line, "obj_path=", "");
    }
    else if(line.find("parent=") != std::string::npos)
    {
      cur_parent = replaceOccurrences(line, "parent=", "");
    }
    else if(line.find("scale=") != std::string::npos)
    {
      cur_scale = replaceOccurrences(line, "scale=", "");
    }
    else if(line.find("rot_spd=") != std::string::npos)
    {
      cur_rot_spd = replaceOccurrences(line, "rot_spd=", "");
    }
    else if(line.find("orbit_rad=") != std::string::npos)
    {
      cur_orbit_rad = replaceOccurrences(line, "orbit_rad=", "");
    }
    else if(line.find("orbit_spd=") != std::string::npos)
    {
      cur_orbit_spd = replaceOccurrences(line, "orbit_spd=", "");
    }
    
  }
  dir_list.close();


  // Start an engine and run it then cleanup after
  Engine *engine = new Engine("Chris Parks, Grant Thompson, PA5", 800, 600);
  if(!engine->Initialize(v, f))
  {
    printf("The engine failed to start.\n");
    delete engine;
    engine = NULL;
    return 1;
  }
  engine->Run();
  delete engine;
  engine = NULL;
  return 0;
}
