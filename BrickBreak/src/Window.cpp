#include "Window.h"

#include <stdexcept>

Window::Window(int width, int height)
    :m_Width(width), m_Height(height)
{
    if(!glfwInit())
	    throw std::runtime_error("Failed to init glfw");

    m_Window = glfwCreateWindow(width, height, "Brick Break", nullptr, nullptr);

    if(!m_Window) 
        throw std::runtime_error("Failed to create window");
	
    glfwMakeContextCurrent(m_Window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    // gladLoadGL();

    // Buffer size info
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(m_Window, &bufferWidth, &bufferHeight);

    glViewport(0, 0, bufferWidth, bufferHeight);

    glfwSetWindowUserPointer(m_Window, this);
    glfwSetKeyCallback(m_Window, &Window::keyCallback);
    glfwSetWindowSizeCallback(m_Window, &Window::windowSizeCallback);
}

Window::~Window()
{
    glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void Window::clear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::update() 
{
	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(m_Window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Window* thisPtr = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    if (key >=0 && key < 1024)
    {
        if (action == GLFW_PRESS) thisPtr->m_Input[key] = true;
        else if (action == GLFW_RELEASE) thisPtr->m_Input[key] = false;
    }
}

void Window::windowSizeCallback(GLFWwindow* window, int w, int h)
{
    Window* thisPtr = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    thisPtr->m_Width = w;
    thisPtr->m_Height = h;
    glViewport(0, 0, w, h);
}
