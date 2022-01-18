#pragma once
namespace Engine::UI::Text2D{
    void Init(const char * texturePath);
    void Square(float x, float y, float w, float h);
    void Print(const char * text, int x, int y, int size);
    void Cleanup();
}