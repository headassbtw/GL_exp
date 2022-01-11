#pragma once
#include "rendering/Camera.hpp"
#include <glm/fwd.hpp>
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <object/GameObject.hpp>
namespace Engine{
    
    struct Game{
        GLFWwindow* window;
        int Width;
        int Height;
        Render::Camera Camera;
        private:
        std::vector<glm::vec3> _vertexBuffer;
        std::vector<glm::vec2> _uvBuffer;
        std::vector<glm::vec3> _normalBuffer;
        public:
        Game(const char* title, int width, int height);
        glm::mat4 ViewMatrix;  
        glm::mat4 ProjectionMatrix;
        std::vector<GameObject> Objects;
        void ProcessMeshes();
        int Render();
    };
}