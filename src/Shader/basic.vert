#version 330

in vec3 position;
in vec3 normal;
in vec2 uv;


out vec3 fragNormal;
out vec2 fragmentUV;
out vec3 lightDir;


uniform mat4 model;
uniform mat4 VP;
uniform vec3 pointLightPos;

void main() {
	fragmentUV = uv;
	fragNormal = (model * vec4(normal, 0)).xyz;
	gl_Position = VP * model * vec4(position, 1);
	lightDir = vec3(normalize(gl_Position - vec4(pointLightPos, 1)));
}