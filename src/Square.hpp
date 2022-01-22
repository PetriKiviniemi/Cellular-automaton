#ifndef SQUARE_H
#define SQUARE_H

#define GLFW_INCLUDE_NONE
#include <thread>
#include <chrono>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct Square
{
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f); 
    std::vector<float> data = {
       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
    };

    std::vector<unsigned int> vIndices= {
        0,1,2,
        0,2,3
    };

    unsigned int active = 0;
    std::map<std::string, Square*> neighbours = {
        {"N", nullptr},{"S", nullptr},{"W", nullptr},{"E", nullptr},
        {"NW", nullptr},{"NE", nullptr},{"SW", nullptr},{"SE", nullptr}
    };
    
    void setColor(std::vector<float> color)
    {
        std::vector<float> newData = {
           -0.5f, -0.5f, 0.0f, color[0], color[1], color[2],
            0.5f, -0.5f, 0.0f, color[0], color[1], color[2],
            0.5f, 0.5f, 0.0f, color[0], color[1], color[2],
           -0.5f, 0.5f, 0.0f, color[0], color[1], color[2]
        };
        data = newData; 
    };

    void activate(bool state)
    {
        if(active == state)
            return;
        if(state)
            setColor({1.0f, 1.0f, 1.0f});
        else
            setColor({0.0f, 0.0f, 0.0f});
        active = state;
    }

    void addNeighbour(std::string key, Square* sq)
    {
        if(neighbours.find(key) != neighbours.end())
        {
            neighbours[key] = sq;
        }
    }

    Square* getNeighbour(std::string key)
    {
        if(neighbours.find(key) == neighbours.end())
            return nullptr;
        return (neighbours.find(key)->second);
    }

    std::vector<float> getCurColor()
    {
        return {data[4], data[5], data[6]};   
    }

    //Asynchronous function for changing color overtime from current to new color
    void setAnimateColor(std::vector<float> col)
    {
        std::thread t1([&](std::vector<float> color)
        {
            while(getCurColor() != color || color[0] > 1.0f || color[1] > 1.0 || color[2] > 1.0)
            {
                std::vector<float> cc = getCurColor();
                //TODO:: implement proper scaling from one color to another
                
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            }

        }, col);
        t1.detach();
    } 
};

#endif
