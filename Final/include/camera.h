#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
bool Reposition(glm::vec3 eye, glm::vec3 focus, int w, int h);
bool Reposition(glm::vec3 where, float theta, float phi, int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
  
    glm::vec3 eye_store;
  
  private:
    glm::mat4 projection;
    glm::mat4 view;
};

#endif /* CAMERA_H */
