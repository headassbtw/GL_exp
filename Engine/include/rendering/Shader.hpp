#pragma once
#include <GL/glew.h>
namespace Engine{
    namespace Render{
        namespace Shaders{
            GLuint CompileFrag(const char *path);
            GLuint CompileVert(const char *path);
            GLuint GetFrag(const char *path);
            GLuint GetVert(const char *path);
            GLuint CompileShaders(GLuint vert, GLuint frag);
            GLuint GetShaders(const char* vert,const char* frag);
        }
    }
}