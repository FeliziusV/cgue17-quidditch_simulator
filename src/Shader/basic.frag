#version 330

in vec3 fragNormal;
in vec2 fragmentUV;
in vec3 fragPos;
in vec3 pointLightPos2;

layout(location = 0) out vec4 fragColor;

uniform sampler2D color_texture;


void main() {
	vec3 ambient = texture(color_texture, fragmentUV).rgb * vec3(0.1f, 0.1f, 0.1f);
	vec3 lightDir = normalize(pointLightPos2 - fragPos);
	float diff = max(dot(normalize(fragNormal), lightDir), 0.0f);
	vec3 textureColor = texture(color_texture, fragmentUV).rgb * diff + ambient;
	//fragColor = vec4(normalize(pointLightPos2), 1.0);
	fragColor = vec4(textureColor, 1.0);
}