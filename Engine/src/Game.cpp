#include <content/filesystem.hpp>

#include "rendering/Camera.hpp"
#include "rendering/Shader.hpp"
#include "object/GameObject.hpp"
#include <ui/hud.hpp>
#include <content/textures.hpp>
#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <controls.hpp>
#include <Game.hpp>
#include <string>
#include <vector>
#include <algorithm>
using namespace glm;

GLuint vertexbuffer;
GLuint uvbuffer;
GLuint normalbuffer;

GLuint VertexArrayID;
GLuint MatrixID;
GLuint ViewMatrixID;
GLuint ModelMatrixID;
GLuint TextureID;
GLuint LightID;

GLuint VertPos;

glm::vec3 lightPos = glm::vec3(4,4,4);
std::vector<Engine::GameObject> ShaderSorted;

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
GLuint lastshader = -122;
void SetShader(GLuint programID){
	if(lastshader != programID)
	{
		glUseProgram(programID);
		MatrixID = glGetUniformLocation(programID, "MVP");
		ViewMatrixID = glGetUniformLocation(programID, "V");
		ModelMatrixID = glGetUniformLocation(programID, "M");
		VertPos = glGetUniformLocation(programID, "Position");
		TextureID  = glGetUniformLocation(programID, "textureSampler");
		LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	}
	lastshader = programID;
}

void Engine::Game::ResizeCallback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
	printf("Resized to: %dx%d\n",width,height);
}

bool HigherLayer(Engine::GameObject g, Engine::GameObject gg){
	return g.layer > gg.layer;
}

bool shaderishigher(Engine::GameObject g, Engine::GameObject gg){
	return g.mesh.Shader > gg.mesh.Shader;
}


Engine::GameObject Engine::Game::Find(const char* name){
	for(int i = 0;i < Objects.size();i++){
		if(Objects[i].name.c_str() == name){
			return Objects[i];
		}
	}
}
Engine::Game::Game(){} //NEVER USE THIS
Engine::Game::Game(const char* title, int width, int height){
	Camera = Render::Camera();
	Height = height;
	Width = width;
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
	window = glfwCreateWindow( width, height, title, NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. Make sure your GPU supports OpenGL 3.3 or above.\n" );
		getchar();
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glfwPollEvents();
    glfwSetCursorPos(window, Width/2, Height/2);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	

    Engine::Filesystem::Textures::LoadSauce(); //load in the missing texture
	BindBuffers(_vertexBuffer, _uvBuffer, _normalBuffer);


	for(int i = 0; i < Objects.size();i++){
		ShaderSorted.push_back(Objects[i]);
	}

	

}


void Engine::Game::ProcessMeshes(){
    std::vector<glm::vec3> tmp_v;
    std::vector<glm::vec2> tmp_u;
    std::vector<glm::vec3> tmp_n;
    for(int i = 0; i < Game::Objects.size();i++){
        auto a = Objects[i];
        Objects[i].mesh.vbuffer_start = tmp_v.size();
        for(int j = 0; j < a.mesh.VertexBuffer.size();j++){
            tmp_v.push_back(a.mesh.VertexBuffer[j]);
        }
        Objects[i].mesh.vbuffer_end = tmp_v.size();
		Objects[i].mesh.ubuffer_start = tmp_u.size();
        for(int j = 0; j < a.mesh.UVBuffer.size();j++){
            tmp_u.push_back(a.mesh.UVBuffer[j]);
        }
		Objects[i].mesh.ubuffer_end = tmp_u.size();
		Objects[i].mesh.nbuffer_start = tmp_n.size();
        for(int j = 0; j < a.mesh.NormalBuffer.size();j++){
            tmp_n.push_back(a.mesh.NormalBuffer[j]);
        }
		Objects[i].mesh.nbuffer_end = tmp_n.size();
        
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
void Engine::Game::UpdateMeshes(){ //like ProcessMeshes but only for the meshes that need it (framerate issues go brr)
	for(int i = 0; i < Game::Objects.size();i++){
        auto a = Objects[i];
		if(a.mesh.NeedsRenderUpdate){
			//Objects[i].mesh.ApplyTransform(Objects[i].transform);
			for(int j = 0; j < a.mesh.VertexBuffer.size();j++){
				_vertexBuffer[a.mesh.vbuffer_start + j] = (a.mesh.VertexBuffer[j]);
			}
			for(int j = 0; j < a.mesh.UVBuffer.size();j++){
				_uvBuffer[a.mesh.ubuffer_start + j] = (a.mesh.UVBuffer[j]);
			}
			for(int j = 0; j < a.mesh.NormalBuffer.size();j++){
				_normalBuffer[a.mesh.nbuffer_start + j] = (a.mesh.NormalBuffer[j]);
			}
			Objects[i].mesh.NeedsRenderUpdate = false;
		}
        
        
    }
}

void Engine::Game::DrawObject(int object,glm::mat4& modelmatrix, glm::mat4& mvp){
	SetShader(Objects[object].mesh.Shader);
	if(Objects[object].mesh.CullBackface){
		glEnable(GL_CULL_FACE);
	}
	else{
		glDisable(GL_CULL_FACE);
	}
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelmatrix[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

	auto pos = Objects[object].transform.Position;
	glUniform3f(VertPos, pos.x, pos.y, pos.z);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Objects[object].mesh.Texture);

	glUniform1i(TextureID, 0);
	glDrawArrays(GL_TRIANGLES, Objects[object].mesh.vbuffer_start, Objects[object].mesh.vbuffer_end - Objects[object].mesh.vbuffer_start);
}

bool wireframe = false;

int Engine::Game::Render()
{
	struct {
        bool operator()(Engine::GameObject a, Engine::GameObject b) const { return a.layer < b.layer; }
    } customLess;


	std::sort(Objects.begin(), Objects.end(), customLess);
	glfwPollEvents();
	glPolygonMode(GL_FRONT_AND_BACK, wireframe ?GL_LINE : GL_FILL);
	glClear(GL_DEPTH_BUFFER_BIT | wireframe ? GL_COLOR_BUFFER_BIT : GL_DEPTH_BUFFER_BIT);
		

		

		ProjectionMatrix = glm::perspective(glm::radians(Camera.FOV), 16.0f / 9.0f, Camera.NearClip, Camera.FarClip);
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		ViewMatrix = glm::lookAt(
							Camera.Position,           // Camera is here
							Camera.Position+Camera.Rotation, // and looks here : at the same position, plus "direction"
							glm::vec3(0,1,0)                  // Head is up (set to 0,-1,0 to look upside-down)
						);
		glm::mat4 Follow = glm::lookAt(
			glm::vec3(0),           // Camera is here
			Camera.Rotation, // and looks here : at the same position, plus "direction"
			glm::vec3(0,1,0)                  // Head is up (set to 0,-1,0 to look upside-down)
		);
		glm::mat4 InverseFollow = glm::lookAt(
			glm::vec3(-2) * Camera.Rotation,           // Camera is here
			glm::vec3(0), // and looks here : at the same position, plus "direction"
			glm::vec3(0,1,0)                  // Head is up (set to 0,-1,0 to look upside-down)
		);
		
		glm::mat4 MVP_Normal = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glm::mat4 MVP_Follow = ProjectionMatrix * Follow * ModelMatrix;
		glm::mat4 MVP_InverseFollow = ProjectionMatrix * InverseFollow * ModelMatrix;
		
		


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

		int layer = -200;
        for(int i = 0; i < Objects.size();i++){
			glm::mat4 view;
			switch (Objects[i].follow) {
				case Engine::Enums::None:
					view = MVP_Normal;
				break;
				case Engine::Enums::Camera:
					view = MVP_Follow;
				break;
				case Engine::Enums::InverseCamera:
					view = MVP_InverseFollow;
				break;
				default:
					view = MVP_Normal;
				break;
			}
			if(Objects[i].layer > layer){
				glClear(GL_DEPTH_BUFFER_BIT);
				layer = Objects[i].layer;
			}

            DrawObject(i, ModelMatrix, view);
        }
		

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		std::string POS;
		POS += "Pos: (";
		POS += std::to_string(Camera.Position.x);
		POS += ",";
		POS += std::to_string(Camera.Position.y);
		POS += ",";
		POS += std::to_string(Camera.Position.z);
		POS += ")";
		std::string ROT;
		ROT += "Rot: (";
		ROT += std::to_string(Camera.Rotation.x);
		ROT += ",";
		ROT += std::to_string(Camera.Rotation.y);
		ROT += ",";
		ROT += std::to_string(Camera.Rotation.z);
		ROT += ")";
		std::string objcount;
		objcount += std::to_string(Objects.size());
		objcount += " Objects";

		
		HUD::PushSprite("content/textures/suce.dds",0, 0, 400, 400);
		HUD::PushSprite("content/textures/suce.dds",400, 0, 200, 200);
		HUD::PushSprite("content/textures/sace.dds",600, 0, 100, 100);
		HUD::PushSprite("content/textures/suce.dds",600, 100, 50, 50);
		HUD::PushText("what the FUCK is up", 50, 50, 30);
		HUD::PushText(POS.c_str(), 0, 900, 20);
		HUD::PushText(ROT.c_str(), 0, 880, 20);
		HUD::PushText(objcount.c_str(), 0, 860, 20);
		HUD::Render();

		glfwSwapBuffers(window);
		
}

void Terminate(){
	HUD::Cleanup();
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	//glDeleteProgram(programID);
	//glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}
