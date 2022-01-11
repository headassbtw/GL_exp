#include "content/filesystem.hpp"
#include "rendering/Camera.hpp"
#include "rendering/Shader.hpp"
#include <content/textures.hpp>
#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <controls.hpp>
#include <Game.hpp>
#include <vector>
using namespace glm;

GLuint vertexbuffer;
GLuint uvbuffer;
GLuint normalbuffer;

GLuint programID;
GLuint Texture;

GLuint VertexArrayID;
GLuint MatrixID;
GLuint ViewMatrixID;
GLuint ModelMatrixID;
GLuint TextureID;
GLuint LightID;
glm::vec3 lightPos = glm::vec3(4,4,4);


void BindBuffers(std::vector<glm::vec3> vertices,
                 std::vector<glm::vec2> uvs,
                 std::vector<glm::vec3> normals){
    glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
    
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_DYNAMIC_DRAW);

    
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_DYNAMIC_DRAW);

    
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_DYNAMIC_DRAW);
}

void SetShader(GLuint programID){
    glUseProgram(programID);
    MatrixID = glGetUniformLocation(programID, "MVP");
	ViewMatrixID = glGetUniformLocation(programID, "V");
	ModelMatrixID = glGetUniformLocation(programID, "M");
    TextureID  = glGetUniformLocation(programID, "myTextureSampler");
    LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
}

Engine::Game::Game(const char* title, int width, int height){
    if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1600, 900, title, NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1600/2, 900/2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles which normal is not towards the camera
	//glEnable(GL_CULL_FACE);

	
	

	programID = Engine::Render::Shaders::GetShaders( "content/shaders/vert.glsl", "content/shaders/frag_lit.glsl" );
    Texture = Engine::Filesystem::Textures::LoadDDS("content/textures/skybox.dds");

    
	bool res = Engine::Filesystem::OBJ("content/models/skybox.obj", _vertexBuffer, _uvBuffer, _normalBuffer);
    if(res) BindBuffers(_vertexBuffer, _uvBuffer, _normalBuffer);
	

	SetShader(programID);
}


void Engine::Game::ProcessMeshes(){
    std::vector<glm::vec3> tmp_v;
    std::vector<glm::vec2> tmp_u;
    std::vector<glm::vec3> tmp_n;
    int aaa = 0;
    for(int i = 0; i < Game::Objects.size();i++){
        auto a = Objects[i];
        Objects[i].mesh.vbuffer_start = tmp_v.size();
        for(int j = 0; j < a.mesh.VertexBuffer.size();j++){
            tmp_v.push_back(a.mesh.VertexBuffer[j]);
        }
        Objects[i].mesh.vbuffer_end = tmp_v.size();
        aaa = tmp_v.size();
        for(int j = 0; j < a.mesh.UVBuffer.size();j++){
            tmp_u.push_back(a.mesh.UVBuffer[j]);
        }
        for(int j = 0; j < a.mesh.NormalBuffer.size();j++){
            tmp_n.push_back(a.mesh.NormalBuffer[j]);
        }
        
    }
    _vertexBuffer.resize(tmp_v.size()); _vertexBuffer.clear();
    _uvBuffer.resize(tmp_u.size()); _uvBuffer.clear();
    _normalBuffer.resize(tmp_n.size()); _normalBuffer.clear();
    for(int j = 0; j < tmp_v.size();j++){
        _vertexBuffer.push_back(tmp_v[j]);
    }
    for(int j = 0; j < tmp_u.size();j++){
        _uvBuffer.push_back(tmp_u[j]);
    }
    for(int j = 0; j < tmp_n.size();j++){
        _normalBuffer.push_back(tmp_n[j]);
    }
}



int Engine::Game::Render()
{
    ProcessMeshes();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		glm::mat4 ProjectionMatrix = Game::ProjectionMatrix;
		glm::mat4 ViewMatrix = Game::ViewMatrix;
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;


		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
        glBufferData(GL_ARRAY_BUFFER, _vertexBuffer.size() * sizeof(glm::vec3), &_vertexBuffer[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
        glBufferData(GL_ARRAY_BUFFER, _uvBuffer.size() * sizeof(glm::vec2), &_uvBuffer[0], GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
        glBufferData(GL_ARRAY_BUFFER, _normalBuffer.size() * sizeof(glm::vec3), &_normalBuffer[0], GL_DYNAMIC_DRAW);

		
        for(int i = 0; i < Objects.size();i++){
            SetShader(Objects[i].mesh.Shader);

            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		    glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

            glActiveTexture(GL_TEXTURE0);
		    glBindTexture(GL_TEXTURE_2D, Objects[i].mesh.Texture);
		
		    glUniform1i(TextureID, 0);
            glDrawArrays(GL_TRIANGLES, Objects[i].mesh.vbuffer_start, Objects[i].mesh.vbuffer_end - Objects[i].mesh.vbuffer_start);
        }

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glfwSwapBuffers(window);
		glfwPollEvents();
}

void Terminate(){
// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	//glDeleteProgram(programID);
	//glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}
