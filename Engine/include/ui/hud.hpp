#pragma once
#include <ui/elements.hpp>
using namespace Engine;
using namespace Engine::UI;
using namespace Engine::UI::Elements;
namespace Engine::UI::HUD{
    void Init(const char * texturePath);
    void Square(float x, float y, float w, float h);
    void Print(const char * text, int x, int y, int size);
    Sprite CreateSprite(const char* sprite,float x, float y, float w, float h);
    Text CreateText(const char * text, int x, int y, int size);
    void DestroySprite(Sprite sprite);
    void DestroyText(Text text);
    void Render();
    void Clear();
    void Cleanup();
    int CountElements();
}