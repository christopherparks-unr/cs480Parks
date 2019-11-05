#include <iostream>
#include <cstring>
#include "engine.h"


int load(std::string v1, std::string f1, std::string v2, std::string f2)
{
  bool endProg = true;
  // Start an engine and run it then cleanup after
  while(endProg)
  {
    Engine *engine = new Engine("Chris Parks & Grant Thompson, PA9", 800, 600);
    if(!engine->Initialize(v1, f1, v2, f2))
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
  std::string v1 = "../assets/shaders/vertex_pa9_p.txt";
  std::string f1 = "../assets/shaders/fragment_pa9_p.txt";
  std::string v2 = "../assets/shaders/vertex_pa9_p.txt";
  std::string f2 = "../assets/shaders/fragment_pa9_p.txt";

  return load(v1, f1, v2, f2);
}
