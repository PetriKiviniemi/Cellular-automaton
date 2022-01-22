#define GLFW_INCLUDE_NONE

#include <iostream>
#include <Windows.h>
#include <ctime>
#include <random>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer.hpp"

unsigned int WINDOW_WIDTH = 1024; 
unsigned int WINDOW_HEIGHT = 1024;

//TODO::
//- IMPLEMENT A GAME CLASS
//- OPTIMIZE GRID RENDERING WHEN IMAGE RESIZES
//- LOCK ASPECT RATIO OF WINDOW
//- CLI MENU FOR SETTINGS

Grid* grid = new Grid(50, 50, WINDOW_WIDTH, WINDOW_HEIGHT);


void framebuffer_size_callback(GLFWwindow*, int width, int height);

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
    glViewport(0, 0, width, width);
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
    grid->updateGridDim(width, height);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Main window", NULL, NULL);
    if(!window)
    {
        std::cout << "Failed to create a window" << std::endl;
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to load GLAD" << std::endl;
        glfwTerminate();
        return 0;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
   

    Renderer* renderer = Renderer::getInstance();

    //TODO:: CREATE GAME CLASS
    std::thread mainLoop([]() {
        while(true)
        {
            //Scan the whole grid and perform necessary actions for each cell
            std::vector<std::map<unsigned int, Square>>::iterator row;
            for(row = grid->data.begin(); row != grid->data.end(); row++)
            {
                std::map<unsigned int, Square>::iterator col;
                for(col = row->begin(); col != row->end(); col++)
                {
                    unsigned int activeNeighbours = 0;
                    std::map<std::string, Square*>::iterator neighbour;
                    for(neighbour = (*col).second.neighbours.begin();
                        neighbour != (*col).second.neighbours.end();
                        neighbour++)
                    {
                        if((*neighbour).second != nullptr && (*neighbour).second->active == 1)
                        {
                            activeNeighbours++;
                        }
                    }

                    //Game logic (Highlife ruleset)
                    if((*col).second.active == 1)
                    {
                        if(activeNeighbours < 2 || activeNeighbours > 3)
                            (*col).second.activate(false);
                    }
                    else
                    {
                        if(activeNeighbours == 3 || activeNeighbours == 6)
                            (*col).second.activate(true);
                    }
                } 
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(200));       
        }
    });
    mainLoop.detach();

    std::thread inputLoop([]() {
        int keydown = 0;
        while(true)
        {
            if(GetAsyncKeyState(VK_UP) & 0x8000 && keydown == 0)
            {
                keydown = 1;
                
                //Select randomly 3 elements from grid, activate them and 2 of their random neighbours
                static std::mt19937 gen(static_cast<unsigned int>(time(nullptr)));
                std::uniform_int_distribution<> rowdistr(0, grid->data.size()-1);
                std::uniform_int_distribution<> coldistr(0, grid->data.at(0).size()-1);

                for(int i = 0; i < 3; i++)
                {
                    int row,col;
                    row = rowdistr(gen);
                    col = coldistr(gen);
                    std::cout << "Activating element at index: " << row << "," << col << std::endl; 
                    Square* sq = grid->getElemAtIndex(row, col);
                    sq->activate(true);
                    

                    std::vector<std::string> availableNeighbours;
                    std::map<std::string, Square*>::iterator it;
                    for(it = sq->neighbours.begin(); it != sq->neighbours.end(); it++)
                    {
                        if((*it).second != nullptr)
                            availableNeighbours.push_back((*it).first);
                    } 
                    
                    std::uniform_int_distribution<> keyDistr(0, availableNeighbours.size()-1);
                    for(int i = 0; i < 2; i++)
                    {
                        sq->getNeighbour(availableNeighbours[keyDistr(gen)])->activate(true);
                    }
                }

                std::cout << "Game started" << std::endl;
            }
            else if(GetAsyncKeyState(VK_UP) == 0)
                keydown = 0;
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); 
        }
    });
    inputLoop.detach();

    
    //Main loop
    while(!glfwWindowShouldClose(window))
    {
        renderer->drawGrid(WINDOW_WIDTH, WINDOW_HEIGHT, grid);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
