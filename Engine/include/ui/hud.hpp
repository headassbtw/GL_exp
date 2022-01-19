#pragma once
#include <ui/elements.hpp>
using namespace Engine;
using namespace Engine::UI;
using namespace Engine::UI::Elements;
namespace Engine::UI::HUD{
    void Add(Text text);
    void Add(Sprite sprite);
    void Init(const char * texturePath);
    void Square(float x, float y, float w, float h);
    void Print(const char * text, int x, int y, int size);
    void PushSprite(const char* sprite,float x, float y, float w, float h);
    void PushText(const char * text, int x, int y, int size);
    void Render();
    void Clear();
    void Cleanup();
    int CountElements();
}