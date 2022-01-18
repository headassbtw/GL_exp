#version 330 core

layout(location = 3) in vec2 vertexPosition_screenspace;
layout(location = 4) in vec2 vertexUV;

uniform mat4 MVP;
out vec2 UV;

void main(){
	vec2 vertexPosition_homoneneousspace = vertexPosition_screenspace;

	vertexPosition_homoneneousspace.y -= 450;
	vertexPosition_homoneneousspace.x -= 800;

	vertexPosition_homoneneousspace.y /= 450.0;
	vertexPosition_homoneneousspace.x /= 800.0;

	vertexPosition_homoneneousspace.y *= 0.755;
	vertexPosition_homoneneousspace.x *= 1.925;
	float a = vertexPosition_homoneneousspace.x;
	
	float b = abs(a);
	float ang = atan(a/2.0f);
	float xx = sin(ang)*2;
	float zz = cos(ang)*2;
	//vertexPosition_homoneneousspace.y -= b;
	gl_Position =  MVP * vec4(xx,vertexPosition_homoneneousspace.y,-zz,1);
	
	
	UV = vertexUV;
}

