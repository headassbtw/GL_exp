#include <GL/glew.h>
#include <GL/gl.h>
#include "content/textures.hpp"
#include "rendering/Shader.hpp"
#include "ui/elements.hpp"
#include <cstring>
#include <ui/hud.hpp>
#include <cstdio>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
using namespace glm;
using namespace Engine;
using namespace Engine::UI;
GLuint Text2DTextureID;
GLuint Text2DVertexBufferID;
GLuint Text2DUVBufferID;
GLuint Text2DShaderID;
GLuint Text2DUniformID;

GLuint MVP_ID;

std::vector<glm::vec2> t_vertices;
std::vector<glm::vec2> t_UVs;

std::vector<Sprite> Sprites;
std::vector<Text> Texts;


const float px_x = 800.0f;
const float px_y = 450.0f;


void HUD::Add(Sprite sprite){
	Sprites.push_back(sprite);
}
void HUD::Add(Text text){
	Texts.push_back(text);
}


void HUD::Init(const char * texturePath){
    Text2DTextureID = Engine::Filesystem::Textures::LoadDDS(texturePath);
    glGenBuffers(1, &Text2DVertexBufferID);
	glGenBuffers(1, &Text2DUVBufferID);
    Text2DShaderID = Engine::Render::Shaders::GetShaders("content/shaders/vert_text.glsl", "content/shaders/frag_text.glsl");
    Text2DUniformID = glGetUniformLocation( Text2DShaderID, "textureSampler" );
    MVP_ID = glGetUniformLocation( Text2DShaderID, "MVP" );
}


glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(23.0f), 16.0f / 9.0f, 0.0001f, 1000.0f);
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	glm::mat4 ViewMatrix = glm::lookAt(
						glm::vec3(0,0,2.4),
						glm::vec3(0,0,-1),
						glm::vec3(0,1,0)
					);
	glm::mat4 MVP_Normal = ProjectionMatrix * ViewMatrix * ModelMatrix;

void HUD::Square(float x, float y, float w, float h){
	//i'm just gonna actually explain it here
	int caulk = floor(w/10); //chunk count, divides by 10, so it'd make for 5 chunks on a 50x sprite
	int bawls = w/caulk; //chunk width, divides width by the amount of chunks, to get the length of each chunk
	for(int i = 0; i < caulk; i++){ //iterates by chunk count
		glm::vec2 vertex_up_left    = glm::vec2( x+(bawls*i)  , y  );
		glm::vec2 vertex_up_right   = glm::vec2( x+(bawls*i)+(bawls), y  );
		glm::vec2 vertex_down_right = glm::vec2( x+(bawls*i)+(bawls), y+h);
		glm::vec2 vertex_down_left  = glm::vec2( x+(bawls*i)  , y+h);

		t_vertices.push_back(vertex_up_left   );
		t_vertices.push_back(vertex_down_left );
		t_vertices.push_back(vertex_up_right  );

		t_vertices.push_back(vertex_down_right);
		t_vertices.push_back(vertex_up_right);
		t_vertices.push_back(vertex_down_left);

		glm::vec2 uv_up_left    = glm::vec2( (bawls*i)/(w),1);
		glm::vec2 uv_up_right   = glm::vec2( ((bawls*i)/(w)) + (bawls / w),1);
		glm::vec2 uv_down_right = glm::vec2( ((bawls*i)/(w)) + (bawls / w),0);
		glm::vec2 uv_down_left  = glm::vec2( (bawls*i)/(w),0);
		t_UVs.push_back(uv_up_left   );
		t_UVs.push_back(uv_down_left );
		t_UVs.push_back(uv_up_right  );

		t_UVs.push_back(uv_down_right);
		t_UVs.push_back(uv_up_right);
		t_UVs.push_back(uv_down_left);
	}

	
}

void HUD::Print(const char * text, int x, int y, int size){
    unsigned int length = strlen(text);
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
	
}
void HUD::PushSprite(const char* sprite, float x, float y, float w, float h){
	Sprite a = Sprite(sprite,x,y,w,h);
	Sprites.push_back(a);
}

void HUD::PushText(const char * text, int x, int y, int size){
	Text a = Text("content/textures/font.dds", text, x,y,size);
	Texts.push_back(a);
}


void HUD::Render(){
	for(int i = 0; i < Sprites.size();i++){
		auto* spr = &Sprites[i];
		spr->vert_idx = t_vertices.size();
		Square(spr->Position.x, spr->Position.y, spr->Size.x, spr->Size.y);
		spr->vert_count = t_vertices.size() - spr->vert_idx;
	}
	for(int i = 0; i < Texts.size();i++){
		auto* spr = &Texts[i];
		spr->vert_idx = t_vertices.size();
		Print(spr->text, spr->Position.x, spr->Position.y, spr->Size);
		spr->vert_count = t_vertices.size() - spr->vert_idx;
	}




	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, t_vertices.size() * sizeof(glm::vec2), &t_vertices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, t_UVs.size() * sizeof(glm::vec2), &t_UVs[0], GL_DYNAMIC_DRAW);
	glDisable(GL_CULL_FACE);
	glUseProgram(Text2DShaderID);
	
	glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, &MVP_Normal[0][0]);

	

	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	


	for(int i = 0; i < Sprites.size();i++){
		glClear(GL_DEPTH_BUFFER_BIT);
		auto* spr = &Sprites[i];
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, spr->TextureID);
		glUniform1i(Text2DUniformID, 0);
		glDrawArrays(GL_TRIANGLES, spr->vert_idx, spr->vert_count );
	}
	for(int i = 0; i < Texts.size();i++){
		glClear(GL_DEPTH_BUFFER_BIT);
		auto* spr = &Texts[i];
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, spr->TextureID);
		glUniform1i(Text2DUniformID, 0);
		glDrawArrays(GL_TRIANGLES, spr->vert_idx, spr->vert_count );
	}
	

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	t_vertices.clear();
	t_UVs.clear();
	Sprites.clear();
	Texts.clear();
}
void HUD::Cleanup(){
    glDeleteBuffers(1, &Text2DVertexBufferID);
	glDeleteBuffers(1, &Text2DUVBufferID);

	glDeleteTextures(1, &Text2DTextureID);

	glDeleteProgram(Text2DShaderID);
}