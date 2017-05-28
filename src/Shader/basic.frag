#version 330

in vec3 fragNormal;
in vec2 fragmentUV;
in vec3 lightDir;

layout(location = 0) out vec4 fragColor;

uniform sampler2D color_texture;

void main() {
	vec3 textureColor = texture(color_texture, fragmentUV).rgb * max(dot(normalize(fragNormal), lightDir), 0.0);
	fragColor = vec4(textureColor, 1.0);
}