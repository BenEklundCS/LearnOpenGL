#include "external/glad/include/glad/glad.h"
#include "shaders/shaders.h"
#include "structs/shapes.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>

// main application process functions
GLFWwindow* createWindow();
void processInput(GLFWwindow* window);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

// shapes
Triangle getDrawableTriangle(const float* vertices, size_t vertexSize, const char * fragmentShaderSource);
void drawTriangle(Triangle triangle);

// Shaders
unsigned int compileVertexShader(const char * vertexShaderSource);
unsigned int compileFragmentShader(const char * fragmentShaderSource);
unsigned int compileShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);

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
    constexpr float v1[] = {
            -0.1f, -0.5f, 0.0f,
            -0.1f, 0.8f, 0.0f,
            0.2f, 0.8f, 0.0f,
    };

    constexpr float v2[] = {
        -0.1f, -0.5f, 0.0f,
        0.2f, -0.5f, 0.0f,
        0.2f, 0.8f, 0.0f
    };
    // End F base

    // Top arm of the F
    constexpr float v3[] = {
        0.5f, 0.8f, 0.0f,
        0.5f, 0.6f, 0.0f,
        0.2f, 0.8f, 0.0f
    };


    constexpr float v4[] = {
        0.5f, 0.6f, 0.0f,
        0.2f, 0.6f, 0.0f,
        0.2f, 0.8f, 0.0f
    };
    // End top arm

    // Bottom arm of the F
    constexpr float v5[] = {
        0.5f, 0.4f, 0.0f,
        0.5f, 0.2f, 0.0f,
        0.2f, 0.4f, 0.0f
    };


    constexpr float v6[] = {
        0.5f, 0.2f, 0.0f,
        0.2f, 0.2f, 0.0f,
        0.2f, 0.4f, 0.0f
    };
    // End bottom arm

    const Triangle drawableTriangle1 = getDrawableTriangle(v1, sizeof(v1), orangeFragmentShaderSource);
    const Triangle drawableTriangle2 = getDrawableTriangle(v2, sizeof(v2), yellowFragmentShaderSource);
    const Triangle drawableTriangle3 = getDrawableTriangle(v3, sizeof(v3), orangeFragmentShaderSource);
    const Triangle drawableTriangle4 = getDrawableTriangle(v4, sizeof(v4), yellowFragmentShaderSource);
    const Triangle drawableTriangle5 = getDrawableTriangle(v5, sizeof(v5), orangeFragmentShaderSource);
    const Triangle drawableTriangle6 = getDrawableTriangle(v6, sizeof(v6), yellowFragmentShaderSource);

    // Render loop
    while(!glfwWindowShouldClose(window)) {
        // Process input
        processInput(window);
        // Render commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw triangle one
        drawTriangle(drawableTriangle1);
        // Draw triangle two
        drawTriangle(drawableTriangle2);
        // Draw triangle three
        drawTriangle(drawableTriangle3);
        // Draw triangle four
        drawTriangle(drawableTriangle4);
        // Draw triangle 5
        drawTriangle(drawableTriangle5);
        // Draw triangle 6
        drawTriangle(drawableTriangle6);

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
    // Create the window object
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);

    // Make sure the window was created successfully
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window, exiting..." << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Register window resize callback function
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    return window;
}

void processInput(GLFWwindow* window) {
    // Close window on ESCAPE
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { // check if user presses escape
        std::cout << "ESC" << std::endl;
        glfwSetWindowShouldClose(window, true); // Set window should close to true
    }
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

#pragma endregion

// ****** //
// SHAPES //
// ****** //

void drawTriangle(const Triangle triangle) {
    glUseProgram(triangle.shaderProgram); // use the triangle shader program
    glBindVertexArray(triangle.VAO); // use the triangle vertex array object
    glDrawArrays(GL_TRIANGLES, 0, 3); // draw
    glBindVertexArray(0); // unbind triangle
}

Triangle getDrawableTriangle(const float* vertices, size_t vertexSize, const char * fragmentShaderSource) {
    unsigned int VBO; // vertex buffer object creation
    glGenBuffers(1, &VBO); // generate buffers with a unique ID
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);

    // Load vertex shader
    const unsigned int vertexShader = compileVertexShader(vertexShaderSource);
    // Load fragment shader
    const unsigned int fragmentShader = compileFragmentShader(fragmentShaderSource);
    // Load shader program
    const unsigned int shaderProgram = compileShaderProgram(vertexShader, fragmentShader);

    //glUseProgram(shaderProgram);
    // Now that the program is being used, the vertex/fragment shaders are no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // 1. Bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. Copy vertices array in a buffer for OpenGL
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);
    // 3. Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    return Triangle{shaderProgram, VBO};
}

#pragma region shaders and programs

unsigned int compileShaderProgram(const unsigned int vertexShader, const unsigned int fragmentShader) {
    // Link compiled shaders to a shader program object
    const unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM:COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shaderProgram;
}

unsigned int compileVertexShader(const char * vertexShaderSource) {
    // Get shader
    const unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Check to see if shader compiled
    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX:COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return vertexShader;
}

unsigned int compileFragmentShader(const char * fragmentShaderSource) {
    // Load fragment shader
    const unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Check to see if shader compiled
    int success;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT:COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return fragmentShader;
}

#pragma endregion






