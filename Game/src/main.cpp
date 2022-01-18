#include <GL/glew.h>
#include <GL/gl.h>
#include <ui/hud.hpp>
#include <Game.hpp>
#include <content/filesystem.hpp>
#include <content/textures.hpp>
#include <object/Mesh.hpp>
#include <object/Transform.hpp>
#include <rendering/Shader.hpp>
#include <scripting/squirrel.hpp>
#include <GLFW/glfw3.h>
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

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;

float verticalMax = glm::radians(89.99f);
float verticalMin = glm::radians(-89.99f);


bool capturing = true;


void computeMatricesFromInputs(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	
	std::this_thread::sleep_for(std::chrono::nanoseconds(3)); //lmao
	

	// Get mouse position
	double xpos, ypos;
	if(capturing){
		glfwGetCursorPos(game.window, &xpos, &ypos);
		
		glfwSetCursorPos(game.window, game.Width/2, game.Height/2);
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

	
	if (glfwGetKey( game.window, GLFW_KEY_W ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	
	if (glfwGetKey( game.window, GLFW_KEY_S ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	
	if (glfwGetKey( game.window, GLFW_KEY_D ) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	
	if (glfwGetKey( game.window, GLFW_KEY_A ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}

	if (glfwGetKey( game.window, GLFW_KEY_GRAVE_ACCENT ) == GLFW_PRESS){
		glfwSetInputMode(game.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		capturing = false;
		
	}
	else{
		
		glfwSetInputMode(game.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		capturing = true;
	}
    //game.Objects[0].transform.Position = position;
    //game.Objects[1].transform.Position = position + (direction * glm::vec3(1.5f));
	game.Camera.Position = position;
	game.Camera.Rotation = direction;
    
    //game.Objects[1].mesh.FlagForUpdate();

	//i've moved this to the engine, but i'm keeping it here just in case

	/*game.ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );
*/
	lastTime = currentTime;
}


void input(){
	
	do{
        computeMatricesFromInputs();
    }
    while( glfwGetKey(game.window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(game.window) == 0 );
}
void meshupdate(){
	
	do{
		//game.UpdateMeshes();
    }
    while( glfwGetKey(game.window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(game.window) == 0 );
}

int main(){

	if(glfwRawMouseMotionSupported()){
		printf("Raw mouse supported\n");
		glfwSetInputMode(game.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetInputMode(game.window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}

/*
	auto shad = Engine::Render::Shaders::GetShaders( "content/shaders/vert.glsl", "content/shaders/frag_unlit.glsl" );

	#pragma region skybox
    auto skybox = Engine::GameObject();
    skybox.mesh = Engine::Mesh("content/models/skybox.obj");
	skybox.mesh.FlagForUpdate();
    skybox.mesh.Shader = shad;
    skybox.mesh.Texture = Engine::Filesystem::Textures::LoadDDS("content/textures/skybox.dds");
	game.Objects.push_back(skybox);
	#pragma endregion


	auto coob = Engine::GameObject();
    coob.mesh = Engine::Mesh("content/models/axis.obj");
    coob.mesh.FlagForUpdate();
    coob.mesh.Shader = shad;
    coob.mesh.Texture = Engine::Filesystem::Textures::LoadDDS("content/textures/color.dds");
    game.Objects.push_back(coob);*/


	Engine::Scripting::Run(game);
	printf("game has %zu objects\n",game.Objects.size());
    game.Camera.FarClip = 3000.0f;
	game.ProcessMeshes();
	//Engine::Scripting::GiveGame(game);

	std::thread input_thread(input);
	game.Objects[0].mesh.FlagForUpdate();
	std::thread mesh_update_thread(meshupdate);
	


	
	HUD::Init("content/textures/font.dds");

	
    do{
        
		
        game.Render();
    }
    while( glfwGetKey(game.window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(game.window) == 0 );
	input_thread.join();
	mesh_update_thread.join();
    Engine::Scripting::Terminate();
    std::cout << "hi from game" << std::endl;
    return 0;
}

