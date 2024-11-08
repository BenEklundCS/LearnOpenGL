//
// Created by luke4 on 11/8/2024.
//

#ifndef SHADERS_H
#define SHADERS_H

//
// Created by ben on 11/6/24.
//

// vertex shader
inline auto vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

// fragment shader
inline auto orangeFragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

inline auto yellowFragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    FragColor = vec4(1.0f, 0.8f, 0.0f, 1.0f);\n"
        "}\0";

#endif //SHADERS_H
