#version 330

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 color;

uniform mat4 projMatr;

smooth out vec4 _color;

void main() {
    gl_Position = projMatr * pos;
    _color = color;
}
