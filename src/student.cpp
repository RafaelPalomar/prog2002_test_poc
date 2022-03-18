#include "student.hpp"

#include <iostream>
#include <glad/glad.h>

#include "GLFW/glfw3.h"

//void ExampleGraphics::setup() {
//}
//
//void ExampleGraphics::run() {
//	while (running()) {
//		
//	}
//}
//
//void ExampleGraphics::cleanup() {
//}

void GLFWErrorCallback(int code, const char* description);

// Error function for OpenGL
void GLAPIENTRY
MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam);


GLFWwindow* window = nullptr;
GLuint vertexArrayId;
GLuint vertexBufferId;
GLuint vertexBufferId2;

bool Lab01::setup() {
    glfwSetErrorCallback(GLFWErrorCallback);
   
    // Initialization of glfw.
    if (!glfwInit())
    {
        std::cin.get();

        return false;
    }

    // Create a window
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(800, 600, "Hello Triangle", nullptr, nullptr);
    if (window == nullptr)
    {

        glfwTerminate();

        std::cin.get();

        return false;
    }

    //Set the OpenGL context
    glfwMakeContextCurrent(window);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return false;
	}

    // Enable capture of debug output.
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    // Print OpenGL data
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

    // Clear the background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Create a triangle geometry
    GLfloat triangle[3*2] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.0f, 0.5f
    };

    // Create a vertex array
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    // Create a vertex buffer
    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

    // Populate the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, nullptr);
    glEnableVertexAttribArray(0);

    GLfloat triangle2[4*2] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.0f, 0.5f,
		0.0f, 0.5f
    };


    // Create a vertex buffer
    glGenBuffers(1, &vertexBufferId2);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId2);

    // Populate the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, nullptr);
    glEnableVertexAttribArray(0);

        // Vertex shader code
	const std::string vertexShaderSrc = R"(
#version 430 core

layout(location = 0) in vec4 position;

void main()
{
gl_Position = position;
}
)";
	// Fragment shader code
	const std::string fragmentShaderSrc = R"(
#version 430 core

out vec4 color;
void main()
{
color = vec4(1);
}
)";


    // Compile the vertex shader
    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vss = vertexShaderSrc.c_str();
    glShaderSource(vertexShader, 1, &vss, nullptr);
    glCompileShader(vertexShader);

    // Compile the fragment shader
    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fss = fragmentShaderSrc.c_str();
    glShaderSource(fragmentShader, 1, &fss, nullptr);
    glCompileShader(fragmentShader);


    // Create a shader program
    auto shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);

    return true;
}

bool Lab01::run() {

    if (glfwWindowShouldClose(window)) {
        return false;
    }

	// Main loop
	glfwPollEvents();

	glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId2);
	glDrawArrays(GL_TRIANGLES, 0, 4);

	glfwSwapBuffers(window);

	// Exit the loop if escape is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        return false;
    }


    return true;

}
bool Lab01::cleanup() {
    glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &vertexBufferId);
    glDeleteVertexArrays(1, &vertexArrayId);

    glfwTerminate();
    return true;
}


void GLFWErrorCallback(int code, const char* description) {
    std::cerr << "Error " << "0x" << std::hex << code << ':' << description << "\n";
}

// Error function for OpenGL
void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam ) {
    std::cerr << "GL CALLBACK:" << ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ) <<
        "type = 0x" << type <<
        ", severity = 0x" << severity <<
        ", message =" << message << "\n";
}
