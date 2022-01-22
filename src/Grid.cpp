#include <iostream>
#include <stdexcept>
#include "Grid.hpp"
#include "Square.hpp"

Grid::Grid(unsigned int colCount, unsigned int rowCount, unsigned int winWidth, unsigned int winHeight)
{
    createGrid(colCount, rowCount, winWidth, winHeight);
}

Grid::~Grid()
{

}

void Grid::createGrid(unsigned int colCount, unsigned int rowCount, unsigned int w, unsigned int h)
{
    data.clear();
    colC = colCount;
    rowC = rowCount;

    float xPosOffset = w / colC;
    float yPosOffset = h / rowC;
    
    //TODO:: OPTIMIZE
    //IF COLCOUNT && ROWCOUNT ARE SAME AS OLD ONES,
    //ONLY UPDATE EXISTING SQUARE OBJECTS POSITIONS
    for(unsigned int i = 0; i < rowCount; i++)
    {
        std::map<unsigned int, Square> row;
        for(unsigned int j = 0; j < colCount; j++)
        {
            Square sq;
            //Range is 1.92
            float startX = -1;
            float startY = -1;
            float tileWidth = (float)w / (float)colCount;
            float tileHeight = (float)h / (float)rowCount;
            float tileX = tileWidth * (j + 1);
            float tileY = tileHeight * (i + 1);
            float tileXNormMin = tileWidth / (float)w;
            float vMin = -0.975f;// - tileXNormMin; 
            float vMax = 0.975f;// - tileXNormMin; 

            float tileXnorm = (tileX - tileWidth) / ((tileWidth * colCount) - (tileWidth));
            float tileXfit = (tileXnorm * (vMax - vMin)) + vMin;
            
            float tileYnorm = (tileY - tileHeight) / ((tileHeight * rowCount) - (tileHeight));
            float tileYfit = (tileYnorm * (vMax - vMin)) + vMin;
           
            sq.position = glm::vec3(tileXfit, tileYfit, 0.0f);
            row.insert({i * rowCount + j, sq});
        }
        data.push_back(row);
    }

    generateNeighbours();
}

void Grid::generateNeighbours()
{
    int rowIdx = 0;
    std::vector<std::map<unsigned int, Square>>::iterator row;
    for(row = data.begin(); row != data.end(); row++)
    {
        int colIdx = 0;
        std::map<unsigned int, Square>::iterator col;
        for(col = row->begin(); col != row->end(); col++)
        {
            //West
            if(colIdx - 1 >= 0)
            {
                (*col).second.addNeighbour("W", &(data.at(rowIdx).at(rowIdx * data.size() + colIdx - 1)));
            }
            //East
            if(colIdx + 1 < data.at(0).size())
            {
                (*col).second.addNeighbour("E", &(data.at(rowIdx).at(rowIdx * data.size() + colIdx + 1)));
            }
            //North
            if(rowIdx - 1 >= 0)
            {
                (*col).second.addNeighbour("N", &(data.at(rowIdx-1).at((rowIdx - 1) * data.size() + colIdx)));
            }
            //South
            if(rowIdx + 1 < data.size())
            {
                (*col).second.addNeighbour("S", &(data.at(rowIdx+1).at((rowIdx + 1) * data.size() + colIdx)));
            }
            //NW
            if(colIdx - 1 >= 0 && rowIdx - 1 >= 0)
            {
                (*col).second.addNeighbour("NW", &(data.at(rowIdx-1).at((rowIdx - 1) * data.size() + colIdx - 1)));
            }
            //NE
            if(colIdx - 1 >= 0 && rowIdx + 1 < data.size())
            {
                (*col).second.addNeighbour("NE", &(data.at(rowIdx+1).at((rowIdx + 1) * data.size() + colIdx - 1)));
            }
            //SW
            if(colIdx + 1 < data.at(0).size() && rowIdx - 1 >= 0)
            {
                (*col).second.addNeighbour("SW", &(data.at(rowIdx-1).at((rowIdx - 1) * data.size() + colIdx + 1)));
            }
            //SE
            if(colIdx + 1 < data.at(0).size() && rowIdx + 1 < data.size())
            {
                (*col).second.addNeighbour("SE", &(data.at(rowIdx+1).at((rowIdx+1) * data.size() + colIdx + 1)));
            }
            colIdx++;
        }
        rowIdx++;
    }
}

void Grid::updateGridDim(unsigned int w, unsigned int h)
{
    createGrid(colC, rowC, w, h);
}

Square* Grid::getElemAtIndex(unsigned int row, unsigned int col)
{
    std::map<unsigned int, Square>::iterator item = data.at(row).find((unsigned int)(row * data.size() + col));
    if(item != data.at(row).end())
        return &(*item).second;
    throw std::runtime_error("No element found from grid");
}
