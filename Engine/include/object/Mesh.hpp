#pragma once
#include "object/Transform.hpp"
#include <GL/gl.h>
#include <vector>
#include <glm/glm.hpp>
namespace Engine{
    struct Mesh{
        std::vector<glm::vec3> VertexBuffer;
        std::vector<glm::vec2> UVBuffer;
        std::vector<glm::vec3> NormalBuffer;
        int vbuffer_start;
        int vbuffer_end;
        GLuint Texture;
        GLuint Shader;
        //private members are the OG mesh without transforms
        private:
        std::vector<glm::vec3> p_VertexBuffer;
        std::vector<glm::vec2> p_UVBuffer;
        std::vector<glm::vec3> p_NormalBuffer;
        public:
        void ApplyTransform(Engine::Transform transform);
        Mesh();
        Mesh(const char* modelpath);
        ~Mesh();

    };
}