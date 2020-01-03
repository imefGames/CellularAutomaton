#pragma once

#include <vector>

namespace CellularAutomaton
{
    class Renderer;

    class Grid
    {
    public:
        Grid(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

        void ComputeNextGrid();
        void RandomizeGrid();
        void ClearGrid();
        void Render(Renderer& renderer);
        void OnMouseClick(unsigned int x, unsigned int y);

        bool IsCellAlive(unsigned int x, unsigned int y) const;
        char GetCellState(unsigned int x, unsigned int y) const;
        void SetCellState(unsigned int x, unsigned int y, char newState);

    private:
        unsigned int GetNeighborCount(unsigned int x, unsigned int y) const;

        std::vector<char> m_Cells;
        unsigned int m_GridX;
        unsigned int m_GridY;
        unsigned int m_GridWidth;
        unsigned int m_GridHeight;
    };
}