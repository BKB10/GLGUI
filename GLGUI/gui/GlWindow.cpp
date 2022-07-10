#include "GlWindow.h"
#include <iostream>
#include "../../GLAbs/render/Renderer.h"
#include "../../GLAbs/utils/Utils.h"
//#include "Text.h"

int GlWindow::startWindow() {
    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    glfwWindow = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);
    if (!glfwWindow)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(glfwWindow);

    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW!" << std::endl;
        return -1;
    }

    //while (glGetError() != GL_NO_ERROR);

    glfwSetDropCallback(glfwWindow, dropCallback);

    std::cout << "GL Version: " << glGetString(GL_VERSION) << std::endl;

    //textManager.initialize();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(glfwWindow))
    {
        /* Render here */

        tick();

        render();

        /* Swap front and back buffers */
        glfwSwapBuffers(glfwWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

void GlWindow::initialize() {
}

void GlWindow::tick() {
    for (unsigned int i = 0; i < components.size(); i++) {
        components[i]->tick();
    }
}

void GlWindow::render() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int windowWidth;
    int windowHeight;
    glfwGetWindowSize(glfwWindow, &windowWidth, &windowHeight);
    width = windowWidth;
    height = windowHeight;

    glViewport(0, 0, windowWidth, windowHeight);

    for (unsigned int i = 0; i < components.size(); i++) {
        components[i]->render();
    }
}