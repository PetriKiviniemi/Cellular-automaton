#ifndef RENDERER_H
#define RENDERER_H
#include "ShaderProgram.hpp"
#include "Grid.hpp"
#include <glm/glm.hpp>
#include "Square.hpp"

class Renderer
{
    private:
        static Renderer * _instance;
        unsigned int vbo;
        unsigned int vao;
        unsigned int ebo;
        ShaderProgram* sp;
    protected:
        Renderer();
    public:
        ~Renderer();
        Renderer(Renderer &other) = delete;
        void operator=(const Renderer &) = delete;
        static Renderer *getInstance();

        std::vector<Square> objectsToRender;
        
        void init();
        void updateScreenDimensions(unsigned int w, unsigned int h);
        void addData(std::vector<float> vertices, std::vector<unsigned int> indices);
        void drawGrid(unsigned int wWidth, unsigned int wHeight, Grid* grid);

};


#endif
