#ifndef CLIMATE_CHANGE_VISUALISATION_CAMERA_H_
#define CLIMATE_CHANGE_VISUALISATION_CAMERA_H_

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <vector>

class Camera {
 public:
  explicit Camera();

  void OnMouseMove(int x, int y);

  void OnKeyboardDown(unsigned char key);

  void OnKeyboardUp(unsigned char key);

  void UpdatePosition();

  void SetDelta(float delta);

  glm::mat4 GetMatrix();

 private:
  int last_x_;
  int last_y_;
  bool is_first_mouse_movement_;
  float yaw_;
  float pitch_;
  float delta_;
  std::vector<bool> is_key_pressed_;
  glm::vec3 camera_pos_;
  glm::vec3 camera_front_;
  glm::vec3 camera_up_;
};

#endif
