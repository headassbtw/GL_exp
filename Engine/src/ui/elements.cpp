#include "content/textures.hpp"
#include <ui/elements.hpp>
#include <vector>
using namespace Engine;
using namespace Engine::UI;
using namespace Engine::UI::Elements;

Engine::UI::Elements::Sprite::Sprite(const char* texture, float posX, float posY, float sizeX, float sizeY){
    TextureID = Engine::Filesystem::Textures::LoadDDS(texture);
    Position.x = posX;
    Position.y = posY;
    Size.x = sizeX;
    Size.y = sizeY;
}
Engine::UI::Elements::Text::Text(const char* location, const char* contents, float posX, float posY, float size){
    TextureID = Engine::Filesystem::Textures::LoadDDS(location);
    text = contents;
    Position.x = posX;
    Position.y = posY;
    Size = size;
}

