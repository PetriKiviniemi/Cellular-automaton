#define GLFW_INCLUDE_NONE

#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.hpp"

ShaderProgram::ShaderProgram()
{

}

ShaderProgram::ShaderProgram(const char* vfname, const char* ffname)
{
    /* TODO:: COMPILE AND LINKER ERROR CHECKS VIA GL_INFOLOG */
    createVertexShader(vfname);
    createFragmentShader(ffname);
    program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    //Check linking errors
    int success;
    char ilog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(program, 512, NULL, ilog);
        std::cout << "Failed to link shader program\n" << ilog << std::endl;
    }
    
}

ShaderProgram::~ShaderProgram()
{

}

void ShaderProgram::activate()
{
    if(vertexShader && fragmentShader && program)
    {
        glUseProgram(program);
    }
}

void ShaderProgram::setMat4(std::string uniform_name, glm::mat4 mat)
{
    const char* n = uniform_name.c_str();
    glUniformMatrix4fv(glGetUniformLocation(program, n), 1, GL_FALSE, glm::value_ptr(mat)); 
}

void ShaderProgram::checkCompileErrors(unsigned int shader)
{
    int success;
    char ilog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, ilog);
        std::cout << "Failed to compile shader:\n" << ilog << std::endl;
    }
}

void ShaderProgram::createFragmentShader(const char* fname)
{
    std::ifstream is;
    is.exceptions(std::ios::failbit | std::ios::badbit);
    is.open(fname);
    
    std::string sourceCode((std::istreambuf_iterator<char>(is)),
                            std::istreambuf_iterator<char>());
    if(!sourceCode.empty())
    {
        std::cout << "Fragment Shader file read" << std::endl;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* sourceCodeCstr = sourceCode.c_str();
        glShaderSource(fragmentShader, 1, &sourceCodeCstr, NULL);
        glCompileShader(fragmentShader);
        checkCompileErrors(fragmentShader); 
    }
}

void ShaderProgram::createVertexShader(const char* fname)
{
    std::ifstream is;
    is.exceptions(std::ios::failbit | std::ios::badbit);
    is.open(fname);
    
    std::string sourceCode((std::istreambuf_iterator<char>(is)),
                            std::istreambuf_iterator<char>());
    if(!sourceCode.empty())
    {
        std::cout << "Vertex Shader file read" << std::endl;
   
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* sourceCodeCstr = sourceCode.c_str();
        glShaderSource(vertexShader, 1, &sourceCodeCstr, NULL);
        glCompileShader(vertexShader);
        checkCompileErrors(vertexShader); 
    }
}
