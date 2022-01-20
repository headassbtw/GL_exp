#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D textureSampler;

void main(){
	//color = vec4(1.0,1.0,1.0,0.5);
	vec4 texCol = texture( textureSampler, UV );
	color = texCol;
}