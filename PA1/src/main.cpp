#include <iostream>
#include <fstream>

#include "engine.h"
#include <cstring>
using namespace std;


int main(int argc, char **argv)
{
  // Start an engine and run it then cleanup after
  int cur_arg = 2;
  std::string v = "../assets/shaders/vertex_pa01.txt"; //Set default vertex shader
  std::string f = "../assets/shaders/fragment_pa01.txt"; //Set default fragment shader
  if(argc > 1)
  {
    while(cur_arg <= argc)
    {
      //Test for '-v /path' argument: Vertex shader path definition
      if(strcmp(argv[cur_arg - 1],"-v") == 0 && argc >= cur_arg + 1)
      {
        v = argv[cur_arg];
        cur_arg += 2;
      }
      //Testfor '-f /path' argument: Fragment shader path definition
      else if(strcmp(argv[cur_arg - 1],"-f") == 0 && argc >= cur_arg + 1)
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
  Engine *engine = new Engine("Tutorial Window Name", 800, 600);
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
