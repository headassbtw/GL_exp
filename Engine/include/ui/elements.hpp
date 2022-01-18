#pragma once
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/fwd.hpp>
namespace Engine {
    namespace UI{
        namespace Elements{
            struct Sprite{
                int vert_idx;
                int vert_count;
                GLuint TextureID;
                glm::vec2 Position;
                glm::vec2 Size;
                Sprite(const char* texture, float posX, float posY, float sizeX, float sizeY);
            };
            struct Text{
                int vert_idx;
                int vert_count;
                GLuint TextureID;
                const char* text;
                glm::vec2 Position;
                float Size;
                Text(const char* location, const char* contents, float posX, float posY, float size);
            };
            
        }   
    }
}