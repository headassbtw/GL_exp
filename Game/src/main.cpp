#include <GL/glew.h>
#include "Game.hpp"
#include "content/filesystem.hpp"
#include "content/textures.hpp"
#include "object/Mesh.hpp"
#include "object/Transform.hpp"
#include "rendering/Shader.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <iostream>
#include <test.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <object/GameObject.hpp>
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



void computeMatricesFromInputs(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(game.window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(game.window, 1600/2, 900/2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(1600/2 - xpos );
	verticalAngle   += mouseSpeed * float( 900/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey( game.window, GLFW_KEY_W ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( game.window, GLFW_KEY_S ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( game.window, GLFW_KEY_D ) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( game.window, GLFW_KEY_A ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	game.ProjectionMatrix = glm::perspective(glm::radians(FoV), 16.0f / 9.0f, 0.1f, 100.0f);
	// Camera matrix
    game.Objects[0].transform.Position = position;
    game.Objects[0].mesh.ApplyTransform(game.Objects[0].transform);
	game.ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	lastTime = currentTime;
}
int main(){
    Engine::Test::Hello();
    auto ass = Engine::GameObject();
    ass.mesh = Engine::Mesh("content/models/skybox.obj");
    ass.transform.Scale = glm::vec3(10);
    ass.mesh.ApplyTransform(ass.transform);
    ass.mesh.Shader = Engine::Render::Shaders::GetShaders( "content/shaders/vert.glsl", "content/shaders/frag_unlit.glsl" );
    ass.mesh.Texture = Engine::Filesystem::Textures::LoadDDS("content/textures/skybox.dds");

    auto ass2 = Engine::GameObject();
    ass2.mesh = Engine::Mesh("content/models/cube.obj");
    ass2.transform.Position = glm::vec3(0,-3,0);
    ass2.mesh.ApplyTransform(ass2.transform);
    ass2.mesh.Shader = Engine::Render::Shaders::GetShaders( "content/shaders/vert.glsl", "content/shaders/frag_lit.glsl" );
    ass2.mesh.Texture = Engine::Filesystem::Textures::LoadDDS("content/textures/sauce.dds");

    auto ass3 = Engine::GameObject();
    ass3.mesh = Engine::Mesh("content/models/cube.obj");
    ass3.transform.Position = glm::vec3(0,3,0);
    ass3.mesh.ApplyTransform(ass3.transform);
    ass3.mesh.Shader = Engine::Render::Shaders::GetShaders( "content/shaders/vert.glsl", "content/shaders/frag_lit.glsl" );
    ass3.mesh.Texture = Engine::Filesystem::Textures::LoadDDS("content/textures/spher.dds");
    

    game.Objects.push_back(ass);
    game.Objects.push_back(ass2);
    game.Objects.push_back(ass3);

    do{
        computeMatricesFromInputs();
        game.Render();
    }
    while( glfwGetKey(game.window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(game.window) == 0 );

    
    std::cout << "hi from game" << std::endl;
    return 0;
}

