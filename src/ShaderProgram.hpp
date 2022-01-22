#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram
{
    private:
        unsigned int program;
        unsigned int vertexShader;
        unsigned int fragmentShader;
        void createVertexShader(const char* fname);
        void createFragmentShader(const char* fname);
        void checkCompileErrors(unsigned int shader);
    public:
        ShaderProgram();
        ~ShaderProgram();
        ShaderProgram(const char* vfname, const char* ffname);
        
        void activate();
        void setMat4(std::string uniform_name, glm::mat4 mat);
};

#endif
