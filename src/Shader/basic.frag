#version 330

in vec3 fragNormal;
in vec2 fragmentUV;
in vec3 fragPos;
in vec3 pointLightPos2;
in vec3 fragCameraPos;

layout(location = 0) out vec4 fragColor;

uniform sampler2D color_texture;


void main() {
	vec3 viewDir = normalize(fragCameraPos - fragPos);
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 lightDir = normalize(pointLightPos2 - fragPos);
	vec3 reflectDir = reflect(-lightDir, fragNormal);

	float specIntensity = 0.5f;
	float ambientIntensity = 0.1f;
	float diff = max(dot(normalize(fragNormal), lightDir), 0.0f);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32) * specIntensity;

	vec3 ambientColor = lightColor * ambientIntensity;
	vec3 diffColor = lightColor * diff;
	vec3 specColor = lightColor * spec + specIntensity;
	vec3 textureColor = texture(color_texture, fragmentUV).rgb * (ambientColor + diffColor + specColor);
	fragColor = vec4(textureColor, 1.0);
	
	//fragColor = vec4(fragPos, 1.0f);
}