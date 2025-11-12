#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 colorRGB;

out vec3 colorVS;
uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

void main(){
    colorVS = colorRGB;
    // gl_Position= model * vec4(vertex,1.0);
    gl_Position = projection * view * model * vec4(vertex, 1.0);

}