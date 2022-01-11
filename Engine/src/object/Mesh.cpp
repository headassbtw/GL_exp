#include "content/filesystem.hpp"
#include "object/Transform.hpp"
#include <object/Mesh.hpp>

Engine::Mesh::Mesh(){

}
void Engine::Mesh::ApplyTransform(Engine::Transform transform){
    VertexBuffer.resize(p_VertexBuffer.size());
    for(int i = 0; i < p_VertexBuffer.size(); i++){
        glm::vec3 tmp = p_VertexBuffer[i];
        tmp *= transform.Scale;
        tmp += transform.Position;
        VertexBuffer[i] = tmp;
    }
    UVBuffer.resize(p_UVBuffer.size());
    NormalBuffer.resize(p_NormalBuffer.size());
    for(int i = 0; i < p_UVBuffer.size(); i++){
        UVBuffer[i] = p_UVBuffer[i];
    }
    for(int i = 0; i < p_NormalBuffer.size(); i++){
        glm::vec3 tmp = p_NormalBuffer[i];
        tmp *= transform.Scale;
        tmp += transform.Position;
        NormalBuffer[i] = tmp;
    }
}

Engine::Mesh::Mesh(const char* modelpath){
    Engine::Filesystem::OBJ(modelpath, p_VertexBuffer, p_UVBuffer, p_NormalBuffer);
    ApplyTransform(Engine::Transform());
}

Engine::Mesh::~Mesh(){
    
}