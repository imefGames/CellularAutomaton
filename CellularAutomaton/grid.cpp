#include "grid.h"

#include "renderer.h"
#include <random>
#include <ctime>

namespace CellularAutomaton
{
    Grid::Grid(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
        : m_GridX{ x }
        , m_GridY{ y }
        , m_GridWidth{ width }
        , m_GridHeight{ height }
    {
        m_Cells.resize(m_GridWidth * m_GridHeight);
    }

    void Grid::ComputeNextGrid()
    {
        std::vector<char> nextCells;
        nextCells.resize(m_GridWidth * m_GridHeight);

        for (unsigned int j = 0; j < m_GridHeight; ++j)
        {
            for (unsigned int i = 0; i < m_GridWidth; ++i)
            {
                bool isCellAlive{ IsCellAlive(i, j) };
                unsigned int neighborCount{ GetNeighborCount(i, j) };

                bool nextState{ isCellAlive && neighborCount == 2 || neighborCount == 3 };
                nextCells[i + j * m_GridWidth] = (nextState ? '*' : ' ');
            }
        }

        m_Cells = nextCells;
    }

    void Grid::RandomizeGrid()
    {
        std::mt19937_64 randomDevice{ static_cast<unsigned int>(time(nullptr)) };
        std::uniform_int_distribution<int> dist{ 0, 1 };

        for (char& c : m_Cells)
        {
            c = dist(randomDevice) ? '*' : ' ';
        }
    }

    void Grid::ClearGrid()
    {
        for (char& c : m_Cells)
        {
            c = ' ';
        }
    }

    void Grid::Render(Renderer& renderer)
    {
        renderer.DrawRectangle(m_GridX, m_GridY, m_GridWidth + 2, m_GridHeight + 2);
        for (unsigned int j = 0; j < m_GridHeight; ++j)
        {
            for (unsigned int i = 0; i < m_GridWidth; ++i)
            {
                renderer.DrawCharacter(i + m_GridX + 1, j + m_GridY + 1, GetCellState(i, j));
            }
        }
    }

    void Grid::OnMouseClick(unsigned int x, unsigned int y)
    {
        unsigned int gridX{ x - m_GridX - 1 };
        unsigned int gridY{ y - m_GridY - 1 };
        if (gridX < m_GridWidth && gridY < m_GridHeight)
        {
            char clickedCell{ GetCellState(gridX, gridY) };
            SetCellState(gridX, gridY, clickedCell == '*' ? ' ' : '*');
        }
    }

    bool Grid::IsCellAlive(unsigned int x, unsigned int y) const
    {
        return (GetCellState(x, y) == '*');
    }

    char Grid::GetCellState(unsigned int x, unsigned int y) const
    {
        char cellState{ ' ' };
        if (x < m_GridWidth && y < m_GridHeight)
        {
            cellState = m_Cells[x + y * m_GridWidth];
        }
        return cellState;
    }

    void Grid::SetCellState(unsigned int x, unsigned int y, char newState)
    {
        if (x < m_GridWidth && y < m_GridHeight)
        {
            m_Cells[x + y * m_GridWidth] = newState;
        }
    }

    unsigned int Grid::GetNeighborCount(unsigned int x, unsigned int y) const
    {
        unsigned int neighborCount{ 0 };
        if (IsCellAlive(x - 1, y - 1)) { ++neighborCount; }
        if (IsCellAlive(x - 1, y    )) { ++neighborCount; }
        if (IsCellAlive(x - 1, y + 1)) { ++neighborCount; }
        if (IsCellAlive(x    , y - 1)) { ++neighborCount; }
        if (IsCellAlive(x    , y + 1)) { ++neighborCount; }
        if (IsCellAlive(x + 1, y - 1)) { ++neighborCount; }
        if (IsCellAlive(x + 1, y    )) { ++neighborCount; }
        if (IsCellAlive(x + 1, y + 1)) { ++neighborCount; }
        return neighborCount;
    }
}