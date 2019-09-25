#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <vector>

using namespace std;

#include "graphics.h"
#include "global.h"
#include "engine.h"


std::vector<std::string> object_reference_names;
std::vector<std::vector< std::vector<float> > > object_list;

//Function that finds index of string o in list of potential objects
int getObjectIndex(std::string o)
{
  for(unsigned int a = 0; a < object_reference_names.size(); a++)
  {
    if(object_reference_names[a].compare(o) == 0)
    {
      return a;
    }
  }
  return -1;
}

std::vector<Vertex> getVertexVector(int index)
{
  std::vector<Vertex> output;
  std::vector<float> thisVec = object_list[index][0];
  unsigned int i = 0;
  while(i < thisVec.size())
  {
    Vertex thisVert = {{thisVec[i], thisVec[i+1], thisVec[i+2]}, {(float)(rand() % 100) / 100, (float)(rand() % 100) / 100, (float)(rand() % 100) / 100}};
    output.push_back(thisVert);
    i += 3;
  }
  std::cout << "Initialized Vertex Vector" << std::endl;
  return output;
}

std::vector<int> getIndexVector(int index)
{
  std::vector<int> output;
  std::vector<float> thisVec = object_list[index][1];
  unsigned int i = 0;
  while(i < thisVec.size())
  {
    output.push_back((int)floor(thisVec[i]) - 1);
    i++;
  }
  std::cout << "Initialized Index Vector" << std::endl;
  return output;
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
      else if(cur_arg + 1 <= argc && strcmp(argv[cur_arg - 1], "-o") == 0)
      {
        o = argv[cur_arg];
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


  //Read list of objects to read from /assets/objects/object_list.txt
  //Vector of directories
  std::vector<std::string> object_dir_array;
  //std::vector<std::string> object_reference_names;
  //String that contains the directory currently being read from file
  std::string line;
  //Input stream class for reading from file
  std::ifstream dir_list;
  //Open the file, path determined by function call in graphics.cpp
  dir_list.open("../object_list.txt");

  while(getline(dir_list,line))
  {
    object_dir_array.push_back(line);
    object_reference_names.push_back(line.substr(line.find_last_of('/') + 1, string::npos));
    std::cout << "Found " + object_reference_names.back() << std::endl;
  }
  dir_list.close();

  //Open every object directory within the vector, parse the contents of the .obj file, and bring info into a vector of objects
  //std::vector< std::vector< std::vector<float> > > object_list;
  for(unsigned int a = 0; a < object_dir_array.size(); a++)
  {
    std::vector< std::vector<float> > some_object;
  
    for(int b = 0; b < 2; b++)
    {
      std::vector<float> some_list; //Vertices and indices
      some_object.push_back(some_list);
    }
    object_list.push_back(some_object);
  }
  int ind = 0;
  for(std::string n : object_dir_array)
  {
    dir_list.open(n);

    while(getline(dir_list,line))
    {
      if(line.compare(0, line.find_first_of(' '),"v") == 0)
      {
        //std::cout << "Vertex found!" << std::endl;
        
        line = line.substr(line.find_first_of(' ') + 1, string::npos);
        
        object_list[ind][0].push_back(stof(line.substr(0, line.find_first_of(' '))));
        line = line.substr(line.find_first_of(' ') + 1, string::npos);
        
        object_list[ind][0].push_back(stof(line.substr(0, line.find_first_of(' '))));
        line = line.substr(line.find_first_of(' ') + 1, string::npos);
        
        object_list[ind][0].push_back(stof(line.substr(0, line.find_first_of(' '))));
      }
      else if(line.compare(0, line.find_first_of(' '),"f") == 0)
      {
        //std::cout << "Index found!" << std::endl;
        line = line.substr(line.find_first_of(' ') + 1, string::npos);
        
        object_list[ind][1].push_back(stof(line.substr(0, line.find_first_of(' '))));
        line = line.substr(line.find_first_of(' ') + 1, string::npos);
        
        object_list[ind][1].push_back(stof(line.substr(0, line.find_first_of(' '))));
        line = line.substr(line.find_first_of(' ') + 1, string::npos);
        
        object_list[ind][1].push_back(stof(line.substr(0, line.find_first_of(' '))));
      }
    }
    //std::cout << n << '\n';
    dir_list.close();
    std::cout << "Initialized " << object_reference_names[ind] << std::endl;
    ind++;
  }




  // Start an engine and run it then cleanup after
  Engine *engine = new Engine("Chris Parks, PA4", 800, 600);
  if(!engine->Initialize(v, f, o))
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
