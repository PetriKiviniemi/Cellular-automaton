#define GLFW_INCLUDE_NONE

#include <iostream>
#include <vector>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer.hpp"
#include "ShaderProgram.hpp"
#include "Grid.hpp"
#include "Square.hpp"

Renderer* Renderer::_instance{nullptr};

Renderer::Renderer()
{
    init();
}

Renderer::~Renderer()
{

}

Renderer* Renderer::getInstance()
{
    if(_instance == nullptr)
    {
        _instance = new Renderer();
    }
    return _instance;
}

void Renderer::init()
{
    sp = new ShaderProgram("../shaders/VertexShader.vshader", "../shaders/FragmentShader.fshader");

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao); 
}

void Renderer::updateScreenDimensions(unsigned int w, unsigned int h)
{
    //Pass
    return;
}

void Renderer::addData(std::vector<float> vertices, std::vector<unsigned int> indices)
{
    //Bind buffers we want to use
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //Unbind buffers, data is bound to vao now
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::drawGrid(unsigned int wWidth, unsigned int wHeight, Grid* grid)
{
    float w2h_ratio = wWidth/wHeight;
    float h2w_ratio = wWidth/wHeight;

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    sp->activate();

    glm::mat4 projection = glm::mat4(1.0f);
    //projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f); 
    sp->setMat4("projection", projection);

    glm::mat4 view = glm::mat4(1.0f);
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    sp->setMat4("view", view);
    
    //Draw objects in the object list
    if(!grid)
        return;


    std::vector<std::map<unsigned int, Square>>::iterator it; 
    for(it = grid->data.begin(); it != grid->data.end(); it++)
    {
        std::map<unsigned int, Square>::iterator tiles;
        for(tiles = it->begin(); tiles != it->end(); tiles++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, (*tiles).second.position);

            float tileWidth = (float)wWidth/(float)grid->getColCount();
            float tileHeight = (float)wHeight/(float)grid->getRowCount(); 
            float tileWidthNorm = (tileWidth)/(wWidth);
            float tileWidthFit = (tileWidthNorm * 1.8); 
            float tileHeightNorm = (tileHeight)/(wHeight);
            float tileHeightFit = (tileHeightNorm * 1.8);

            model = glm::scale(model, glm::vec3(tileWidthFit, tileHeightFit, 0.1f));

            sp->setMat4("model", model);

            addData((*tiles).second.data,(*tiles).second.vIndices); 
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    } 
}
