#include <iostream>
#include <cstring>
#include "engine.h"

int load(std::string v, std::string f)
{
  bool endProg = true;
  // Start an engine and run it then cleanup after
  while(endProg)
  {
    Engine* engine = new Engine("Chris Parks & Grant Thompson, Final", 1280, 720);
    if(!engine->Initialize(v, f))
    {
      printf("The engine failed to start.\n");
      delete engine;
      engine = NULL;
      return 1;
    }

  
    endProg = engine->Run();
    delete engine;
    engine = NULL;
  }
  return 0;
}



int main(int argc, char **argv)
{
  //Define default path strings for vertex and fragment shaders
  std::string v = "../assets/shaders/vertex_pa9_p.txt";
  std::string f = "../assets/shaders/fragment_pa9_p.txt";
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
  //std::cout << "VS: " << v << std::endl;
  //std::cout << "FS: " << f << std::endl;

  return load(v, f);
}
