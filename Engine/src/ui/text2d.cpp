#include "content/textures.hpp"
#include "rendering/Shader.hpp"
#include <cstring>
#include <ui/text2d.hpp>
#include <cstdio>
#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
using namespace glm;

GLuint Text2DTextureID;
GLuint Text2DVertexBufferID;
GLuint Text2DUVBufferID;
GLuint Text2DShaderID;
GLuint Text2DUniformID;

GLuint MVP_ID;


const float px_x = 800.0f;
const float px_y = 450.0f;

void Engine::UI::Text2D::Init(const char * texturePath){
    Text2DTextureID = Engine::Filesystem::Textures::LoadDDS(texturePath);
    glGenBuffers(1, &Text2DVertexBufferID);
	glGenBuffers(1, &Text2DUVBufferID);
    Text2DShaderID = Engine::Render::Shaders::GetShaders("content/shaders/vert_text.glsl", "content/shaders/frag_text.glsl");
    Text2DUniformID = glGetUniformLocation( Text2DShaderID, "textureSampler" );
    MVP_ID = glGetUniformLocation( Text2DShaderID, "MVP" );
}


glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(23.2f), 16.0f / 9.0f, 0.0001f, 1000.0f);
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	glm::mat4 ViewMatrix = glm::lookAt(
						glm::vec3(0,0,2.4),
						glm::vec3(0,0,-1),
						glm::vec3(0,1,0)
					);
	glm::mat4 MVP_Normal = ProjectionMatrix * ViewMatrix * ModelMatrix;

void Engine::UI::Text2D::Square(float x, float y, float w, float h){

	std::vector<glm::vec2> t_vertices;
	std::vector<glm::vec2> t_UVs;
	
	glm::vec2 vertex_up_left    = glm::vec2( x  , y  );
	glm::vec2 vertex_up_right   = glm::vec2( x+w, y  );
	glm::vec2 vertex_down_right = glm::vec2( x+w, y+h);
	glm::vec2 vertex_down_left  = glm::vec2( x  , y+h);

	t_vertices.push_back(vertex_up_left   );
	t_vertices.push_back(vertex_down_left );
	t_vertices.push_back(vertex_up_right  );

	t_vertices.push_back(vertex_down_right);
	t_vertices.push_back(vertex_up_right);
	t_vertices.push_back(vertex_down_left);

	glm::vec2 uv_up_left    = glm::vec2( 0,1);
	glm::vec2 uv_up_right   = glm::vec2( 1,1);
	glm::vec2 uv_down_right = glm::vec2( 1,0);
	glm::vec2 uv_down_left  = glm::vec2( 0,0);
	t_UVs.push_back(uv_up_left   );
	t_UVs.push_back(uv_down_left );
	t_UVs.push_back(uv_up_right  );

	t_UVs.push_back(uv_down_right);
	t_UVs.push_back(uv_up_right);
	t_UVs.push_back(uv_down_left);

	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, t_vertices.size() * sizeof(glm::vec2), &t_vertices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, t_UVs.size() * sizeof(glm::vec2), &t_UVs[0], GL_DYNAMIC_DRAW);


	
	glDisable(GL_CULL_FACE);
	glUseProgram(Text2DShaderID);
	
	glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, &MVP_Normal[0][0]);





	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Text2DTextureID);
	glUniform1i(Text2DUniformID, 0);

	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClear(GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, t_vertices.size() );

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
}

void Engine::UI::Text2D::Print(const char * text, int x, int y, int size){
    unsigned int length = strlen(text);
	// Fill buffers
	std::vector<glm::vec2> t_vertices;
	std::vector<glm::vec2> t_UVs;
	for ( unsigned int i=0 ; i<length ; i++ ){
		
		glm::vec2 vertex_up_left    = glm::vec2( x+i*size     , y+size );
		glm::vec2 vertex_up_right   = glm::vec2( x+i*size+size, y+size );
		glm::vec2 vertex_down_right = glm::vec2( x+i*size+size, y      );
		glm::vec2 vertex_down_left  = glm::vec2( x+i*size     , y      );

		t_vertices.push_back(vertex_up_left   );
		t_vertices.push_back(vertex_down_left );
		t_vertices.push_back(vertex_up_right  );

		t_vertices.push_back(vertex_down_right);
		t_vertices.push_back(vertex_up_right);
		t_vertices.push_back(vertex_down_left);

		char character = text[i];
		float uv_x = (character%16)/16.0f;
		float uv_y = (character/16)/16.0f;

		glm::vec2 uv_up_left    = glm::vec2( uv_x           , uv_y );
		glm::vec2 uv_up_right   = glm::vec2( uv_x+1.0f/16.0f, uv_y );
		glm::vec2 uv_down_right = glm::vec2( uv_x+1.0f/16.0f, (uv_y + 1.0f/16.0f) );
		glm::vec2 uv_down_left  = glm::vec2( uv_x           , (uv_y + 1.0f/16.0f) );
		t_UVs.push_back(uv_up_left   );
		t_UVs.push_back(uv_down_left );
		t_UVs.push_back(uv_up_right  );

		t_UVs.push_back(uv_down_right);
		t_UVs.push_back(uv_up_right);
		t_UVs.push_back(uv_down_left);
	}
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, t_vertices.size() * sizeof(glm::vec2), &t_vertices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, t_UVs.size() * sizeof(glm::vec2), &t_UVs[0], GL_DYNAMIC_DRAW);
	glDisable(GL_CULL_FACE);
	glUseProgram(Text2DShaderID);
	
	glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, &MVP_Normal[0][0]);





	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Text2DTextureID);
	glUniform1i(Text2DUniformID, 0);

	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, t_vertices.size() );

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
}
void Engine::UI::Text2D::Cleanup(){
    glDeleteBuffers(1, &Text2DVertexBufferID);
	glDeleteBuffers(1, &Text2DUVBufferID);

	glDeleteTextures(1, &Text2DTextureID);

	glDeleteProgram(Text2DShaderID);
}