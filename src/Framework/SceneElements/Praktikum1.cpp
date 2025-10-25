//
// Created by Marjane Choua on 24.10.25.
//

#include "../../../src/Framework/SceneElements/Praktikum1.h"


// Dreieck mit RGB-Farben an den Ecken
Vertex vertices[] = {
    { 0.0f,  0.5f, 1.0f, 0.0f, 0.0f }, // Rot oben
    {-0.5f, -0.5f, 0.0f, 1.0f, 0.0f }, // Grün links
    { 0.5f, -0.5f, 0.0f, 0.0f, 1.0f }  // Blau rechts
};

Praktikum1::Praktikum1() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position (vec2)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Farbe (vec3)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Praktikum1::render() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}


