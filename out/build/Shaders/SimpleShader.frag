#version 330

uniform vec4 rgba;

out vec4 colour;

void main() {
	colour = vec4(rgba.x, rgba.y, rgba.z, rgba.w);
}
