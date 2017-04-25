#version 330

in vec3 fragNormal;
in vec2 fragmentUV;


layout(location = 0) out vec4 fragColor;

uniform sampler2D color_texture;

void main() {
	vec3 textureColor = texture(color_texture, fragmentUV).rgb;
	fragColor = vec4(textureColor, 1.0);
}