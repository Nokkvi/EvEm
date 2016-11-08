#include "../include/Screen.h"

#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GLFW/glfw3.h>

Screen::Screen(int width, int height)
{
    this->width = width;
    this->height = height;
    this->pixels = new uint8_t[width * height * 4];
    this->Reset();

    this->program = glCreateProgram();
    int vs = glCreateShader(GL_VERTEX_SHADER);
    int fs = glCreateShader(GL_FRAGMENT_SHADER);

    char *ss = const_cast<char*>(vertexSource.c_str());
    char *ssa[1] = {ss};
    int size = vertexSource.length();
    glShaderSource(vs, 0, ssa, &size);

    ss = const_cast<char*>(fragmentSource.c_str());
    ssa[0] = ss;
    size = fragmentSource.length();
    glShaderSource(fs, 0, ssa, &size);

    glCompileShader(vs);
    glCompileShader(fs);

    GLint check;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &check);
    if (!check)
       throw "Vertex shader failed to compile.";

    glGetShaderiv(fs, GL_COMPILE_STATUS, &check);
    if (!check)
       throw "Fragment shader failed to compile.";

    glAttachShader(this->program, vs);
    glAttachShader(this->program, fs);
    glLinkProgram(this->program);

    glCreateBuffers(1, &this->buffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->buffer);
    glBufferData(GL_ARRAY_BUFFER, 160 * 144 * 6 * 3, nullptr, GL_STATIC_DRAW);

    GLint linked;
    glGetProgramiv(this->program, GL_LINK_STATUS, &linked);
    if (!linked)
       throw "Program failed to link.";
}

Screen::~Screen()
{
    delete[] this->pixels;
}

void Screen::SetPixels(int offset, uint8_t value)
{
    this->pixels[offset * 4] = value;
    this->pixels[offset * 4 + 1] = value;
    this->pixels[offset * 4 + 2] = value;
    this->pixels[offset * 4 + 3] = value;
}

void Screen::Render()
{
    glUseProgram(this->program);
    glBindBuffer(GL_ARRAY_BUFFER, this->buffer);

    uint8_t* data = (uint8_t*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    for (int y = 0; y < 144; y++)
    {
        for (int x = 0; x < 160; x++)
        {
            for (int j = 0; j < 4; j++)
            {
                *data = x + 1 * (j % 2);
                *(data + 1) = y + 1 * (j / 2);
                *(data + 2) = this->pixels[x + y * 160];
                data += 3;
            }
        }
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);

    glDrawArrays(GL_TRIANGLES, 0, 160 * 144 * 6);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
}

void Screen::Reset()
{
    for (int i = 0; i < width * height * 4; i++)
        this->pixels[i] = 0xFF;
}