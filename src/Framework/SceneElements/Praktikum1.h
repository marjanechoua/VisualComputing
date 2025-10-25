//
// Created by Marjane Choua on 24.10.25.
//

#ifndef OPENGL_PRAKTIKUM_PRAKTIKUM1_H
#define OPENGL_PRAKTIKUM_PRAKTIKUM1_H

#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct Vertex {
    float x, y;
    float r, g, b;
};

class Praktikum1 {
public:
    Praktikum1();
    void render();
private:
    GLuint VAO, VBO;
};



#endif //OPENGL_PRAKTIKUM_PRAKTIKUM1_H