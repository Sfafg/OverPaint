#version 450

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec4 aColor;


layout(location = 0)out vec4 color;
void main() {

    gl_PointSize = 2;
    gl_Position =  vec4(aPosition / vec2(1920,1080) * 2 - 1, 0, 1.0);
    color = aColor;
}