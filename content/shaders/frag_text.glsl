#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D textureSampler;

void main(){
	//color = vec4(1.0,1.0,1.0,1.0);
	color = texture( textureSampler, UV );
}