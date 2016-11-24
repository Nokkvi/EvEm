#ifndef Application_H
#define Application_H

#include "Definitions.h"
#include "Processor.h"
#include "Memory.h"
#include "key.h"
#include <string>

#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Application
{
    private:
        GLFWwindow* window;

        GPU* g;
        Memory* m;
        Processor* p;

        std::string title;
        int width, height;
    public:
        static key* k;

        Application(std::string title, int width, int height);
        ~Application();

        bool Init();
        void Loop();
    private:
        void Frame();
};

#endif
