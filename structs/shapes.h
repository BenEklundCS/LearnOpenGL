//
// Created by ben on 11/6/24.
//

#ifndef LEARNOPENGL_SHAPES_H
#define LEARNOPENGL_SHAPES_H

struct TriangleVertexArray {
    float vertices[9];
};

struct Triangle {
    unsigned int shaderProgram;
    unsigned int VAO;
};

#endif //LEARNOPENGL_SHAPES_H