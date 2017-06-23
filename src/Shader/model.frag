#version 410 core

in vec2 TexCoords;
in vec3 fragNormal;
in vec3 fragPos;
in vec3 fragPointLightPos;
in vec3 fragCameraPos;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_diffuse4;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
uniform sampler2D texture_specular3;
uniform sampler2D texture_specular4;


void main()
{    
	vec3 viewDir = normalize(fragCameraPos - fragPos);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 lightDir = normalize(fragPointLightPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, fragNormal);
	
	float specIntensity = 0.5f;
	float ambientIntensity = 0.1f;
	float diff = max(dot(normalize(fragNormal), lightDir), 0.0);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1) * specIntensity;
	vec3 ambientColor = lightColor * ambientIntensity;
	vec3 diffColor = lightColor * diff;
	vec3 specColor = lightColor * spec;
	vec3 textureColor = texture(texture_diffuse1, TexCoords).rgb * (ambientColor + diffColor + specColor);
    FragColor = vec4(textureColor, 1.0);
	//FragColor = vec4(texture(texture_diffuse1, TexCoords));
}