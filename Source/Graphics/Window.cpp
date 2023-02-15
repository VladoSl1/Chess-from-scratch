#include "Window.h"

#include <iostream>
#include <sstream>



namespace graphics
{
    Window::Window(size_t width, size_t height, const char* name, bool VSync)
        : Width(width), Height(height), window(nullptr), window_name(name)
    {
        /* Initialize the library */
        if (!glfwInit())
        {
            std::cout << "Failed to initialize GLFW library\n";
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(width, height, name, NULL, NULL);

        if (!window)
        {
            std::cout << "Failed to create GLFW window\n" << std::endl;
            glfwTerminate();
            return;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        /* Cap frame rate to display max frame rate */
        if (!VSync)
            glfwSwapInterval(0);

        /* Resize window when user it resizing it */
        glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

        if (glewInit() != GLEW_OK)
        {
            std::cout << "Failed to initialize GLEW\n";
            return;
        }
    }

    Window::~Window() {
        CloseWindow();
    }
    
    void Window::CloseWindow() {
        glfwTerminate();
    }
    
    void Window::ClearScreen(math::vec4<> color) {
        GLCall(glClearColor(color.x, color.y, color.z, color.w));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }

    void Window::Update() {
        SwapBuffers();
        PollEvents();
    }

    void Window::ShowFPS()
    {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        FrameNumber++;

        if (deltaTime >= 1.0f)
        {
            double fps = double(FrameNumber) / deltaTime;

            std::stringstream ss;
            ss << window_name << " [" << fps << "FPS]";

            glfwSetWindowTitle(window, ss.str().c_str());

            FrameNumber = 0;
            lastTime = currentTime;
        }
    }

    /* Resize window */
    void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
        GLCall(glViewport(0, 0, width, height));
    }

    void Window::SwapBuffers() {
        glfwSwapBuffers(window);
    }
     
    void Window::PollEvents() {
        glfwPollEvents();
    }
}