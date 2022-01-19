#pragma once
#include "rendering/Camera.hpp"
#include <glm/fwd.hpp>
#include <vector>
#include <deque>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <object/GameObject.hpp>
namespace Engine{
    
    class Game{
        public: 
        GLFWwindow* window;
        float Width;
        float Height;
        float InputThreadMs;
        float RenderThreadMs;
        float ScriptThreadMs;
        bool RenderSkybox = true;
        bool RenderAxisHelper = true;
        Render::Camera Camera;
        std::deque<std::string> Errors;
        void ResizeCallback(GLFWwindow* window, int width, int height);
        private:
        std::vector<glm::vec3> _vertexBuffer;
        std::vector<glm::vec2> _uvBuffer;
        std::vector<glm::vec3> _normalBuffer;
        
        public:
        GameObject Find(const char* name);
        Game(); //NEVER USE THIS
        Game(const char* title, int width, int height);
        glm::mat4 ViewMatrix;  
        glm::mat4 ProjectionMatrix;
        std::vector<GameObject> Objects;
        void ProcessMeshes();
        void UpdateMeshes();
        void DrawObject(int object,glm::mat4& modelmatrix, glm::mat4& mvp);
        int Render();
    };
}