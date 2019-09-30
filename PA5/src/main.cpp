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
#include "graphics_headers.h"
#include "engine.h"

std::string replaceOccurrences(std::string& s, const std::string& toReplace, const std::string& replaceWith)
{
    while(s.find(toReplace) != std::string::npos)
    {
      std::size_t pos = s.find(toReplace);
      s = s.replace(pos, toReplace.length(), replaceWith);
    }
    return s;
}

Scene scene_from_assimp(std::string filename)
{
	Assimp::Importer importer;
	const aiScene *readScene = importer.ReadFile(filename,aiProcess_Triangulate);

	Scene make;
	make.meshes.clear();

	std::string otherstring(filename);
	make.supername = otherstring;
	
	Mesh submake;
	Vertex read = {{0.0f, 0.0f, 0.0f},{0.0f, 0.0f, 0.0f}};

	for (int iter = 0; iter < (int) readScene->mNumMeshes; iter++)
	{
		std::string newstring(readScene->mMeshes[iter]->mName.C_Str());
		submake.name = newstring;

		submake.vertex_data.clear();

		for (int verts = 0; verts < (int) (readScene->mMeshes[iter])->mNumVertices; verts++)
		{
			read.vertex[0] = (readScene->mMeshes[iter])->mVertices[verts].x;
			read.vertex[1] = (readScene->mMeshes[iter])->mVertices[verts].y;
			read.vertex[2] = (readScene->mMeshes[iter])->mVertices[verts].z;
			read.color[0] = 1.0;
			read.color[1] = 1.0;
			read.color[2] = 1.0;

			submake.vertex_data.push_back(read);
		}

		submake.indice_data.clear();

		for (int faces = 0; faces < (int) (readScene->mMeshes[iter])->mNumFaces; faces++)
		{
			//ASSUMES triangle conversion worked

			submake.indice_data.push_back( (readScene->mMeshes[iter])->mFaces[faces].mIndices[0] );
			submake.indice_data.push_back( (readScene->mMeshes[iter])->mFaces[faces].mIndices[1] );
			submake.indice_data.push_back( (readScene->mMeshes[iter])->mFaces[faces].mIndices[2] );
		}

		make.meshes.push_back(submake);
	}

	return make;
}


int main(int argc, char **argv)
{
  //Define default path strings for vertex and fragment shaders
  std::string v = "../assets/shaders/vertex_pa1.txt";
  std::string f = "../assets/shaders/fragment_pa1.txt";
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

  //String that contains the directory currently being read from file
  std::string line;
  //Input stream class for reading from file
  std::ifstream dir_list;
  //Open the file, path determined by function call in graphics.cpp
  Assimp::Importer importer;
  dir_list.open("../config.ini");
  int obj_num = 0;

  while(getline(dir_list,line))
  {
    line = replaceOccurrences(line, " ", "");
    line = replaceOccurrences(line, " ", "");
    if(line.find("[") != std::string::npos)
    {
      if(obj_num > 0)
      {
        std::vector<std::string> tempVec = {cur_name, cur_obj_path, cur_scale, cur_rot_spd, cur_orbit_rad, cur_orbit_spd, cur_parent};
	      scene_vector.push_back(scene_from_assimp(cur_obj_path));
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
