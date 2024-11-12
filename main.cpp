#include "external/glad/include/glad/glad.h"
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
void drawTriangle(Triangle triangle);
void updateTriangle(Triangle triangle);

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
            // positions         // colors
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
            0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
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

void drawTriangle(const Triangle triangle) {
    triangle.shaderProgram.use();
    glBindVertexArray(triangle.VAO); // use the triangle vertex array object
    glDrawArrays(GL_TRIANGLES, 0, 3); // draw
    glBindVertexArray(0); // unbind triangle
}

float x_speed = 0.01f;
float y_speed = 0.005f;

void updateTriangle(const Triangle triangle) {
    int posOffsetLocation = glGetUniformLocation(triangle.shaderProgram.getId(), "posOffset");
    float current_offset[3];
    glGetUniformfv(triangle.shaderProgram.getId(), posOffsetLocation, current_offset);
    if (current_offset[0] > 1.0f || current_offset[0] <= -1.0f) {
        x_speed *= -1;
    }
    if (current_offset[1] > 1.0f || current_offset[1] <= -1.0f) {
        y_speed *= -1;
    }
    glUseProgram(triangle.shaderProgram.getId());
    glUniform3f(posOffsetLocation, current_offset[0] + x_speed, current_offset[1] + y_speed, current_offset[2]);
}

Triangle getDrawableTriangle(const float* vertices, size_t vertexSize) {
    unsigned int VBO; // vertex buffer object creation
    glGenBuffers(1, &VBO); // generate buffers with a unique ID
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);

    Shader myShader("shaders/vertex_shader.vs", "shaders/fragment_shader.fs");

    float texCoords[] = {
            0.0f, 0.0f,  // lower-left corner
            1.0f, 0.0f,  // lower-right corner
            0.5f, 1.0f   // top-center corner
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // 1. Bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. Copy vertices array in a buffer for OpenGL
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);
    // 3. Set vertex attribute pointers
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    return Triangle{myShader, VBO};
}






