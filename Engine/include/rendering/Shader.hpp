#pragma once
#include <GL/glew.h>
namespace Engine{
    namespace Render{
        namespace Shaders{
            GLuint CreateFrag(const char *path);
            GLuint CreateVert(const char *path);
            GLuint GetShaders(const char* vert, const char* frag);
        }
    }
}