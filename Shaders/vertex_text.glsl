#version 330

layout (location = 0) in vec4 vertex;
out vec2 texc;

uniform mat4 projection;


void main() {
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    texc = vertex.zw;
}