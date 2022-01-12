#pragma once
#include <GL/glew.h>
namespace Engine{
    namespace Filesystem{
        namespace Textures{
            GLuint LoadSauce();
            GLuint LoadDDS(const char* path);
        }
    }
}