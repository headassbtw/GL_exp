#include <glm/fwd.hpp>
#include <rendering/Camera.hpp>

Engine::Render::Camera::Camera(){
    FOV = 90.0f;
    NearClip = 0.001f;
    FarClip = 1000.0f;
    Position = glm::vec3(0);
    Rotation = glm::vec3(0);
}