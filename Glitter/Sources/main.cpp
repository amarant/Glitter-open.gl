// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>


void assertShaderCompile(GLuint shader) {
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    assert(status == GL_TRUE);
}

void printShaderInfoLog(GLuint shader) {
    char buffer[512];
    glGetShaderInfoLog(shader, 512, NULL, buffer);
    std::cout << buffer << std::endl;
}

void printFileOpen(std::ifstream &fileStream) {
        if ( ! fileStream.is_open() ) {
            std::cout <<" Failed to open" << std::endl;
        }
        else {
            std::cout <<"Opened OK" << std::endl;
        }
}

void errorCallback(int error, const char * description) {
    std::cout << error << " " << description << std::endl;
}

int main(int argc, char * argv[]) {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    glfwSetErrorCallback(&errorCallback);
    
    float vertices[] = {
        0.0f, 0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f,
    };
    
    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    std::ifstream vertexShaderFile(PROJECT_SOURCE_DIR "/Glitter/Shaders/main.vert");
    std::string vertexShaderStr((std::istreambuf_iterator<char>(vertexShaderFile)),
                                std::istreambuf_iterator<char>());
    auto vertexSource = vertexShaderStr.c_str();
    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    assertShaderCompile(vertexShader);

    std::ifstream fragmentShaderFile(PROJECT_SOURCE_DIR "/Glitter/Shaders/main.frag");
    std::string fragmentShaderStr((std::istreambuf_iterator<char>(fragmentShaderFile)),
                                std::istreambuf_iterator<char>());
    auto fragmentSource = fragmentShaderStr.c_str();
    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    assertShaderCompile(fragmentShader);
    
    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
        std::cout << glGetError() << std::endl;
    glUseProgram(shaderProgram);
    std::cout << glGetError() << std::endl;
        std::cout << glGetError() << std::endl;
    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    std::cout << glGetError() << std::endl;
    
    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);
        
        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw a triangle from the 3 vertices
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   glfwTerminate();
    
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
    
    glDeleteBuffers(1, &vbo);
    
    glDeleteVertexArrays(1, &vao);
    
    return EXIT_SUCCESS;
}
