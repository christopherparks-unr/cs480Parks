#include "camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

bool Camera::Initialize(int w, int h)
{
  //--Init the view and projection matrices

  view = glm::lookAt( glm::vec3(2.5, 6.0, 0.0), //Eye Position
                      glm::vec3(2.5, 0.0, -1.75), //Focus point
                      glm::vec3(0.0, 0.0, -1.0)); //Positive Y is up

  projection = glm::ortho(-5.0,11.0,5.0,-5.0,4.0,10.0);

  return true;
}

bool Camera::Reposition(float eye_x, float eye_y, float eye_z, float focus_x, float focus_y, float focus_z, int w, int h)
{

  view = glm::lookAt( glm::vec3(eye_x, eye_y, eye_z), //Eye Position
                      glm::vec3(focus_x, focus_y, focus_z), //Focus point
                      glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 2000.0f); //Distance to the far plane, 
  return true;
}

bool Camera::Reposition(float theta, float radius, int w, int h)
{
  float x = sin(theta)*radius;
  float z = cos(theta)*radius;

  view = glm::lookAt( glm::vec3(x+8.0, 8.0, z+8.0), //Eye Position
                      glm::vec3(x, 0.0, z), //Focus point
                      glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 2000.0f); //Distance to the far plane, 

  return true;
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view;
}


