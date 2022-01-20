#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_scancode.h>
#include <cstdio>
#include <ui/hud.hpp>
#include <Game.hpp>
#include <content/filesystem.hpp>
#include <content/textures.hpp>
#include <object/Mesh.hpp>
#include <object/Transform.hpp>
#include <rendering/Shader.hpp>
#include <scripting/squirrel.hpp>
#include <glm/common.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <test.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <object/GameObject.hpp>
#include <scripting/squirrel.hpp>
#include <thread>
#include <chrono>
Engine::Game game = Engine::Game("Hello!",1600,900);



// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 0, 0 ); 
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 90.0f;

float speed = 0.0005f;
float mouseSpeed = 0.005f;

float verticalMax = glm::radians(89.99f);
float verticalMin = glm::radians(-89.99f);


bool capturing = true;


void computeMatricesFromInputs(){
	
	int xpos, ypos;
	xpos = 800;
	ypos = 450;
	if(capturing){
		SDL_ShowCursor(SDL_DISABLE);
		SDL_GetMouseState(&xpos, &ypos);
		SDL_WarpMouseInWindow(game.window, game.Width/2, game.Height/2);
	}
	else{
		SDL_ShowCursor(SDL_ENABLE);
	}

	
	horizontalAngle += mouseSpeed * float( game.Width/2 - xpos );
	verticalAngle   += mouseSpeed * float( game.Height/2 - ypos );

	verticalAngle = glm::clamp(verticalAngle, verticalMin,verticalMax);

	
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	
	
	glm::vec3 up = glm::cross( right, direction );

	SDL_Event event;
        while (SDL_PollEvent(&event))
        {      
            switch(event.type){
				
                case SDL_QUIT:
                    game.running = false;
                break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                        case SDL_KeyCode::SDLK_ESCAPE:
							game.running = false;
                        break;
						case SDL_KeyCode::SDLK_BACKQUOTE:
							capturing = !capturing;
                        break;
						case SDL_KeyCode::SDLK_z:
							game.wireframe = !game.wireframe;
                        break;
                    }
                break;
            }
        }
	auto state = const_cast <Uint8*> (SDL_GetKeyboardState(NULL));

    if(state[SDLK_SPACE]){
		printf("W\n");
	}
	float mspeed = speed;
	if(state[SDL_SCANCODE_LSHIFT]){
		mspeed = speed * 3;
	}
    if(state[SDL_SCANCODE_W])
    {
		position += direction * game.RenderThreadMs * mspeed;
    }
    if(state[SDL_SCANCODE_A])
    {
		position -= right * game.RenderThreadMs * mspeed;
    }
    if(state[SDL_SCANCODE_S])
    {
		position -= direction * game.RenderThreadMs * mspeed;
    }
    if(state[SDL_SCANCODE_D])
    {
		position += right * game.RenderThreadMs * mspeed;
    }

	



	game.Camera.Position = position;
	game.Camera.Rotation = direction;
}


void input(){
	
	do{
		auto start = std::chrono::high_resolution_clock::now();
        computeMatricesFromInputs();
		auto finish = std::chrono::high_resolution_clock::now();
		auto ms = std::chrono::duration_cast<std::chrono::microseconds>(finish-start);
		game.InputThreadMs = (float)ms.count()/1000;
    }
    while(game.running);
}
void meshupdate(){
	
	do{
		if(game.Errors.size() >= 1)
			game.Errors.pop_back();
		std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    while(game.running);
}
void squirrelupdate(){
	
	auto start = std::chrono::high_resolution_clock::now();
	Engine::Scripting::Run("update.nut");
	auto finish = std::chrono::high_resolution_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::microseconds>(finish-start);
	game.ScriptThreadMs = (float)ms.count()/1000;
}

int main(){


	HUD::Init("content/textures/font.dds");
	Engine::Scripting::Run(game);
	
	printf("game has %zu objects\n",game.Objects.size());
    game.Camera.FarClip = 3000.0f;
	game.ProcessMeshes();
	//Engine::Scripting::GiveGame(game);

	std::thread input_thread(input);
	game.Objects[0].mesh.FlagForUpdate();
	std::thread mesh_update_thread(meshupdate);
	


	

	
    do{
		//meshupdate();
		squirrelupdate();
		auto start = std::chrono::high_resolution_clock::now();
        game.Render(); 
		auto finish = std::chrono::high_resolution_clock::now();
		auto ms = std::chrono::duration_cast<std::chrono::microseconds>(finish-start);
		game.RenderThreadMs = (float)ms.count()/1000;
    }
    while(game.running);
	input_thread.join();
	mesh_update_thread.join();
	game.Terminate();
    Engine::Scripting::Terminate();
    return 0;
}

