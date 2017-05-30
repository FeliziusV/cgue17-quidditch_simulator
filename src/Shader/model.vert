#version 330

in vec3 position;
in vec3 normal;
in vec2 uv;


out vec3 fragNormal;
out vec2 fragmentUV;
out vec3 fragPos;
out vec3 pointLightPos2;
out vec3 fragCameraPos;

uniform mat4 model;
uniform mat4 VP;
uniform vec3 pointLightPos;
uniform vec3 cameraPos;

void main() {
	fragmentUV = uv;
	fragNormal = (model * vec4(normal, 0)).xyz;
	gl_Position = VP * model * vec4(position, 1.0f);
	pointLightPos2 = vec3(model * vec4(pointLightPos, 1.0f));
	fragPos = vec3(model * vec4(position, 1.0f));
	fragCameraPos = cameraPos;
	
}