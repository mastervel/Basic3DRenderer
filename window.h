#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "camera.h"

void framebuffer_size_callback(GLFWwindow* WindowPtr, int width, int height);
void mouse_callback(GLFWwindow* WindowPtr, double xpos, double ypos);
void scroll_callback(GLFWwindow* WindowPtr, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 1.7f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

class Window 
{
private:
	int Width;
	int Height;
	const char* Title;
	GLFWwindow* WindowPtr;
public:
	Window(int width = 800, int height = 600, const char* title = "OpenGL Window")
		: Width(width), Height(height), Title(title), WindowPtr(nullptr)
	{
		if (!initialize())
		{
			std::cerr << "Failed to initialize GLFW or create window!" << std::endl;
		}
	}
	~Window()
	{
		if (WindowPtr)
		{
			glfwDestroyWindow(WindowPtr);
		}
		glfwTerminate();
	}

    // Get the GLFW window pointer
    GLFWwindow* GetWindow() const { return WindowPtr; }

    // Check if the window should close
    bool ShouldClose() const { return glfwWindowShouldClose(WindowPtr); }

    // Swap buffers and poll events
    void Update() const
    {
        glfwSwapBuffers(WindowPtr);
        glfwPollEvents();
    }

private:
    // Initialize GLFW and create the window
    bool initialize()
    {
        // Initialize GLFW
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW!" << std::endl;
            return false;
        }

        // Set GLFW window hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        #ifdef __APPLE__
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

        // Create the GLFW window
        WindowPtr = glfwCreateWindow(Width, Height, Title, nullptr, nullptr);
        if (!WindowPtr)
        {
            std::cerr << "Failed to create GLFW window!" << std::endl;
            glfwTerminate();
            return false;
        }

        // Make the OpenGL context current
        glfwMakeContextCurrent(WindowPtr);
        glfwSetFramebufferSizeCallback(WindowPtr, framebuffer_size_callback);
        glfwSetCursorPosCallback(WindowPtr, mouse_callback);
        glfwSetScrollCallback(WindowPtr, scroll_callback);

        // tell GLFW to capture our mouse
        glfwSetInputMode(WindowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Load OpenGL function pointers using GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "Failed to initialize GLAD!" << std::endl;
            return false;
        }

        // Set viewport
        glViewport(0, 0, Width, Height);
        return true;
    }
};

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* WindowPtr, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* WindowPtr, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* WindowPtr, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

#endif