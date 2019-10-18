#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    bool Reposition(float eye_x, float eye_y, float eye_z, float focus_x, float focus_y, float focus_z, int w, int h);
    bool Reposition(float theta, float radius, int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
  
  private:
    glm::mat4 projection;
    glm::mat4 view;
};

#endif /* CAMERA_H */
