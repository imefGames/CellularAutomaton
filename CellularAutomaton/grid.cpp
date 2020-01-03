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
        std::vector<EGridCellState> nextCells;
        nextCells.resize(m_GridWidth * m_GridHeight);

        for (unsigned int j = 0; j < m_GridHeight; ++j)
        {
            for (unsigned int i = 0; i < m_GridWidth; ++i)
            {
                bool isCellAlive{ IsCellAlive(i, j) };
                unsigned int neighborCount{ GetNeighborCount(i, j) };

                EGridCellState nextState{ (isCellAlive && neighborCount == 2 || neighborCount == 3) ? EGridCellState::Alive : EGridCellState::Dead };
                nextCells[i + j * m_GridWidth] = nextState;
            }
        }

        m_Cells = nextCells;
    }

    void Grid::RandomizeGrid()
    {
        std::mt19937_64 randomDevice{ static_cast<unsigned int>(time(nullptr)) };
        std::uniform_int_distribution<int> dist{ 0, 1 };

        for (EGridCellState& c : m_Cells)
        {
            c = dist(randomDevice) ? EGridCellState::Alive : EGridCellState::Dead;
        }
    }

    void Grid::ClearGrid()
    {
        for (EGridCellState& c : m_Cells)
        {
            c = EGridCellState::Dead;
        }
    }

    void Grid::Render(Renderer& renderer)
    {
        renderer.DrawRectangle(m_GridX, m_GridY, m_GridWidth + 2, m_GridHeight + 2);
        for (unsigned int j = 0; j < m_GridHeight; ++j)
        {
            for (unsigned int i = 0; i < m_GridWidth; ++i)
            {
                EGridCellState cellState{ GetCellState(i, j) };
                renderer.DrawCharacter(i + m_GridX + 1, j + m_GridY + 1, cellState == EGridCellState::Alive ? '*' : ' ');
            }
        }
    }

    void Grid::OnMouseClick(unsigned int x, unsigned int y)
    {
        unsigned int gridX{ x - m_GridX - 1 };
        unsigned int gridY{ y - m_GridY - 1 };
        if (gridX < m_GridWidth && gridY < m_GridHeight)
        {
            EGridCellState clickedCell{ GetCellState(gridX, gridY) };
            SetCellState(gridX, gridY, clickedCell == EGridCellState::Alive ? EGridCellState::Dead : EGridCellState::Alive);
        }
    }

    bool Grid::IsCellAlive(unsigned int x, unsigned int y) const
    {
        return (GetCellState(x, y) == EGridCellState::Alive);
    }

    EGridCellState Grid::GetCellState(unsigned int x, unsigned int y) const
    {
        EGridCellState cellState{ EGridCellState::Dead };
        if (x < m_GridWidth && y < m_GridHeight)
        {
            cellState = m_Cells[x + y * m_GridWidth];
        }
        return cellState;
    }

    void Grid::SetCellState(unsigned int x, unsigned int y, EGridCellState newState)
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