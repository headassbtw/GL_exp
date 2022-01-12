#include <GL/glew.h>
#include <Game.hpp>
#include <content/filesystem.hpp>
#include <content/textures.hpp>
#include <object/Mesh.hpp>
#include <object/Transform.hpp>
#include <rendering/Shader.hpp>
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

float verticalMax = glm::radians(90.0f);
float verticalMin = glm::radians(-90.0f);

void computeMatricesFromInputs(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	
	std::this_thread::sleep_for(std::chrono::nanoseconds(3)); //lmao
	

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(game.window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(game.window, game.Width/2, game.Height/2);

	
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


	if (glfwGetKey( game.window, GLFW_KEY_Z ) == GLFW_PRESS){
		game.Objects[1].transform.Position.y += 0.05f;
		game.Objects[1].mesh.FlagForUpdate();
    	//game.Objects[1].mesh.ApplyTransform(game.Objects[1].transform);
	}

	game.ProjectionMatrix = glm::perspective(glm::radians(game.Camera.FOV), 16.0f / 9.0f, game.Camera.NearClip, game.Camera.FarClip);
	
    game.Objects[0].transform.Position = position;
    game.Objects[0].mesh.FlagForUpdate();
	game.ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	lastTime = currentTime;
}


void constupdate(){
	
	do{
        computeMatricesFromInputs();
    }
    while( glfwGetKey(game.window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(game.window) == 0 );
}
void constupdate2(){
	
	do{
		game.UpdateMeshes();
    }
    while( glfwGetKey(game.window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(game.window) == 0 );
}

int main(){
	#pragma region skybox
    Engine::Test::Hello();
    auto skybox = Engine::GameObject();
    skybox.mesh = Engine::Mesh("content/models/skybox.obj");
    skybox.transform.Scale = glm::vec3(1000);
    skybox.mesh.FlagForUpdate();
    skybox.mesh.Shader = Engine::Render::Shaders::GetShaders( "content/shaders/vert.glsl", "content/shaders/frag_unlit.glsl" );
    skybox.mesh.Texture = Engine::Filesystem::Textures::LoadDDS("content/textures/skybox.dds");
	game.Objects.push_back(skybox);
	#pragma endregion

	auto shad = Engine::Render::Shaders::GetShaders( "content/shaders/vert.glsl", "content/shaders/frag_lit.glsl" );

	auto ass = Engine::GameObject();
    ass.mesh = Engine::Mesh("content/models/cube.obj");
    ass.transform.Position = glm::vec3(0);
    ass.mesh.FlagForUpdate();
    ass.mesh.Shader = shad;
    ass.mesh.Texture = Engine::Filesystem::Textures::LoadDDS("content/textures/no.dds");


    
    game.Objects.push_back(ass);
    game.Camera.FarClip = 3000.0f;
	game.ProcessMeshes();



	std::thread update_thread(constupdate);
	std::thread update_2thread(constupdate2);
	
    do{
        
		//computeMatricesFromInputs();
        game.Render();
    }
    while( glfwGetKey(game.window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(game.window) == 0 );
	update_thread.join();
	update_2thread.join();
    
    std::cout << "hi from game" << std::endl;
    return 0;
}

