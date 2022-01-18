#pragma once
namespace Engine::UI::Text2D{
    void Init(const char * texturePath);
    void Print(const char * text, int x, int y, int size);
    void Cleanup();
}