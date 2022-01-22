#ifndef GRID_H
#define GRID_H

#include <vector>
#include <map>
#include "Square.hpp"

class Grid
{
    private:
        unsigned int colC;
        unsigned int rowC;
    public:
        std::vector<std::map<unsigned int, Square>> data; 
        Grid(unsigned int colCount, unsigned int rowCount, unsigned int winWidth, unsigned int winHeight);
        ~Grid();
        
        void updateIndex(unsigned int x, unsigned int y);
        void createGrid(unsigned int r, unsigned int c, unsigned int w, unsigned int h);
        void updateGridDim(unsigned int w, unsigned int h);
        Square* getElemAtIndex(unsigned int row, unsigned int col);
        void generateNeighbours();
        const unsigned int getRowCount() const { return rowC; }
        const unsigned int getColCount() const { return colC; }
};

#endif
