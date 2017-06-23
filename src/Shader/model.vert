#version 410 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;
out vec3 fragNormal;
out vec3 fragPos;
out vec3 fragPointLightPos;
out vec3 fragCameraPos;

uniform mat4 model;
uniform mat4 VP;
uniform vec3 pointLightPos;
uniform vec3 cameraPos;

void main()
{
	TexCoords = texCoords;
	fragNormal = (model * vec4(normal, 0)).xyz;
    gl_Position = VP * model * vec4(position, 1.0);
	fragPointLightPos = vec3(model * vec4(pointLightPos, 1.0));
	fragPos = vec3(model * vec4(position, 1.0f));
	fragCameraPos = cameraPos;

}

