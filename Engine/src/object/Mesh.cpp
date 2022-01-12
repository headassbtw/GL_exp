#include "content/filesystem.hpp"
#include "object/Transform.hpp"
#include <glm/fwd.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <object/Mesh.hpp>

Engine::Mesh::Mesh(){
    CullBackface = true;
}
void Engine::Mesh::FlagForUpdate(){
    NeedsRenderUpdate = true;
}
void Engine::Mesh::ApplyTransform(Engine::Transform transform){
    VertexBuffer.resize(p_VertexBuffer.size());
    glm::vec3 pos = glm::vec3(transform.Position.x,transform.Position.y,-transform.Position.z);
    for(int i = 0; i < p_VertexBuffer.size(); i++){
        glm::vec3 tmp = p_VertexBuffer[i];
        tmp *= transform.Scale;
        tmp = glm::rotateX(tmp, glm::radians(-transform.Rotation.x));
        tmp = glm::rotateY(tmp, glm::radians(-transform.Rotation.y));
        tmp = glm::rotateZ(tmp, glm::radians(-transform.Rotation.z));
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
        tmp = glm::rotateX(tmp, glm::radians(-transform.Rotation.x));
        tmp = glm::rotateY(tmp, glm::radians(-transform.Rotation.y));
        tmp = glm::rotateZ(tmp, glm::radians(-transform.Rotation.z));
        tmp += transform.Position;
        NormalBuffer[i] = tmp;
    }
    NeedsRenderUpdate = true;
}

Engine::Mesh::Mesh(const char* modelpath){
    Engine::Filesystem::OBJ(modelpath, p_VertexBuffer, p_UVBuffer, p_NormalBuffer);
    ApplyTransform(Engine::Transform());
    CullBackface = true;
}

Engine::Mesh::~Mesh(){
    
}