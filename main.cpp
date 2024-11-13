#include "external/glad/include/glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image/stb_image.h"
#include "shaders.h"
#include "structs/shapes.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <cmath>

// main application process functions
GLFWwindow* createWindow();
void processInput(GLFWwindow* window);

// shapes
Triangle getDrawableTriangle(const float* vertices, size_t vertexSize);
void drawTriangle(const Triangle &triangle);
void updateTriangle(Triangle triangle);

bool MOVE_ENABLED = false;

int main() {
    // Initialize the glfw window library
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Run OpenGL with the Core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Load the window
    GLFWwindow* window = createWindow();
    glfwMakeContextCurrent(window);

    // Load GLAD
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to load GLAD, exiting..." << std::endl;
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, 800, 600);

    // Base of the F
    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
   };

    // End bottom arm

    const Triangle drawableTriangle1 = getDrawableTriangle(vertices, sizeof(vertices));

    // Render loop
    while(!glfwWindowShouldClose(window)) {
        // Process input
        processInput(window);
        // Render commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Update triangle
        updateTriangle(drawableTriangle1);
        // Draw triangle one
        drawTriangle(drawableTriangle1);

        // Check/call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // Clean up GLFW
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

#pragma region main application functions

GLFWwindow* createWindow() {
    const int width = 800;
    const int height = 600;
    // Create the window object
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);

    // Make sure the window was created successfully
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window, exiting..." << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    auto resizeWindowCallback = [](GLFWwindow* window, int width, int height) -> void {
        glViewport(0, 0, width, height);
    };

    // Register window resize callback function
    glfwSetFramebufferSizeCallback(window, resizeWindowCallback);

    return window;
}

void processInput(GLFWwindow* window) {
    // Close window on ESCAPE
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { // check if user presses escape
        std::cout << "ESC" << std::endl;
        glfwSetWindowShouldClose(window, true); // Set window should close to true
    }
}

#pragma endregion

// ****** //
// SHAPES //
// ****** //

void drawTriangle(const Triangle &triangle) {
    triangle.shaderProgram.use();
    glBindTexture(GL_TEXTURE_2D, triangle.texture);
    glBindVertexArray(triangle.VAO); // bind the VAO containing the VBO and EBO
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // draw elements based on EBO
    glBindVertexArray(0); // unbind
}


float x_speed = 0.01f;
float y_speed = 0.005f;

void updateTriangle(const Triangle triangle) {
    int posOffsetLocation = glGetUniformLocation(triangle.shaderProgram.getId(), "posOffset");
    float current_offset[3];
    glGetUniformfv(triangle.shaderProgram.getId(), posOffsetLocation, current_offset);
    if (MOVE_ENABLED) {
        if (current_offset[0] > 1.0f || current_offset[0] <= -1.0f) {
            x_speed *= -1;
        }
        if (current_offset[1] > 1.0f || current_offset[1] <= -1.0f) {
            y_speed *= -1;
        }
        glUniform3f(posOffsetLocation, current_offset[0] + x_speed, current_offset[1] + y_speed, current_offset[2]);
        glUseProgram(triangle.shaderProgram.getId());
    }
}

Triangle getDrawableTriangle(const float* vertices, size_t vertexSize) {
    unsigned int VBO, EBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);

    // Define the indices for two triangles forming a square
    unsigned int indices[] = {
        0, 1, 3,  // first triangle (top-right, bottom-right, top-left)
        1, 2, 3   // second triangle (bottom-right, bottom-left, top-left)
    };

    // Create and bind EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    Shader myShader("../shaders/vertex_shader.vs", "../shaders/fragment_shader.fs");

    // Set up the texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    unsigned char *data = stbi_load("../textures/wall.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Create the Vertex Array Object (VAO) and bind it
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Bind VBO, EBO and set attribute pointers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    return Triangle{myShader, texture, VBO, VAO, EBO};  // Updated Triangle to include EBO
}







