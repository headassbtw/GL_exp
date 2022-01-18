#include "content/filesystem.hpp"
#include <rendering/Shader.hpp>
#include <string>
#include <map>
std::map<const char*, GLuint> FragStore;
std::map<const char*, GLuint> VertStore;
std::map<const char*, GLuint> ProgStore;
GLint log_length, success;
GLuint Engine::Render::Shaders::CompileFrag(const char *path){
    GLuint rtnShader;
    std::string ShaderSource = Engine::Filesystem::Text(path);
    const char* ShaderSourcePointer = ShaderSource.c_str();

    rtnShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(rtnShader, 1, &ShaderSourcePointer, NULL);
    glCompileShader(rtnShader);
    glGetShaderiv(rtnShader, GL_COMPILE_STATUS, &success);
    glGetShaderiv(rtnShader, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > 0) {
        printf("fragment shader compile error:\n");
        std::vector<char> FragError(log_length+1);
		glGetShaderInfoLog(rtnShader, log_length, NULL, &FragError[0]);
		printf("%s\n", &FragError[0]);
    }
    if (!success) {
        exit(EXIT_FAILURE);
    }
    printf("Compiled fragment shader \"%s\"\n",path);
    return rtnShader;
}
GLuint Engine::Render::Shaders::CompileVert(const char *path){
    GLuint rtnShader;
    std::string ShaderSource = Engine::Filesystem::Text(path);
    const char* ShaderSourcePointer = ShaderSource.c_str();
    rtnShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(rtnShader, 1, &ShaderSourcePointer, NULL);
    glCompileShader(rtnShader);
    glGetShaderiv(rtnShader, GL_COMPILE_STATUS, &success);
    glGetShaderiv(rtnShader, GL_INFO_LOG_LENGTH, &log_length);
    if ( log_length > 0 ){
        printf("vertex shader compile error:\n");
		std::vector<char> VertexShaderErrorMessage(log_length+1);
		glGetShaderInfoLog(rtnShader, log_length, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}
    if (!success) {
        exit(EXIT_FAILURE);
    }
    printf("Compiled vertex shader \"%s\"\n",path);
    return rtnShader;
}

GLuint Engine::Render::Shaders::GetFrag(const char *path){
    if(!FragStore.contains(path)){
        FragStore[path] = CompileFrag(path);
    }
    return FragStore[path];
}

GLuint Engine::Render::Shaders::GetVert(const char *path){
    if(!VertStore.contains(path)){
        VertStore[path] = CompileVert(path);
    }
    return VertStore[path];
}



GLuint Engine::Render::Shaders::CompileShaders(GLuint vert, GLuint frag) {
    GLuint  program;
    
    program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);
    
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > 0) {
        printf("shader link error for program %d:\n", program);
        std::vector<char> ShaderLinkError(log_length);
		glGetShaderInfoLog(program, log_length, NULL, &ShaderLinkError[0]);
		printf("%s\n", &ShaderLinkError[0]);
        
    }
    if (!success) {
        exit(EXIT_FAILURE);
    }
    //glDeleteShader(vert);
    //glDeleteShader(frag);
    return program;
}

GLuint Engine::Render::Shaders::GetShaders(const char* vert,const char* frag){
    if(!ProgStore.contains(frag)){
        ProgStore[frag] = CompileShaders(GetVert(vert), GetFrag(frag));
    }
    return ProgStore[frag];
}
