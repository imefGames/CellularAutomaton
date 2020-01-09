#include "grid.h"

#include "renderer.h"
#include <random>
#include <ctime>

namespace CellularAutomaton
{
    Grid::Grid(const GridSettings& settings)
        : m_Settings{ settings }
        , m_GridX{ settings.GetGridX() }
        , m_GridY{ settings.GetGridX() }
        , m_GridWidth{ settings.GetGridWidth() }
        , m_GridHeight{ settings.GetGridHeight() }
    {
        m_Cells.resize(m_GridWidth * m_GridHeight);
    }

    void Grid::ComputeNextGrid()
    {
        std::vector<CellID> nextCells;
        nextCells.resize(m_GridWidth * m_GridHeight);

        for (unsigned int j = 0; j < m_GridHeight; ++j)
        {
            for (unsigned int i = 0; i < m_GridWidth; ++i)
            {
                const CellDescriptor& cell{ m_Settings.FindCellDescriptor(GetCellState(i, j)) };
                nextCells[i + j * m_GridWidth] =  cell.ComputeNextCellID(*this, i, j);
            }
        }

        m_Cells = nextCells;
    }

    void Grid::RandomizeGrid()
    {
        std::mt19937_64 randomDevice{ static_cast<unsigned int>(time(nullptr)) };
        std::uniform_int_distribution<CellID> dist{ 0, static_cast<CellID>(m_Settings.GetCellDescriptors().size() - 1) };

        for (CellID& c : m_Cells)
        {
            c = dist(randomDevice);
        }
    }

    void Grid::ClearGrid()
    {
        for (CellID& c : m_Cells)
        {
            c = 0;
        }
    }

    void Grid::Render(Renderer& renderer)
    {
        renderer.DrawRectangle(m_GridX, m_GridY, m_GridWidth + 2, m_GridHeight + 2);
        for (unsigned int j = 0; j < m_GridHeight; ++j)
        {
            for (unsigned int i = 0; i < m_GridWidth; ++i)
            {
                const CellDescriptor& cell{ m_Settings.FindCellDescriptor(GetCellState(i, j)) };
                renderer.DrawCharacter(i + m_GridX + 1, j + m_GridY + 1, cell.GetCellDisplay(), cell.GetCellColor());
            }
        }
    }

    void Grid::ToggleCellState(unsigned int x, unsigned int y)
    {
        unsigned int gridX{ x - m_GridX - 1 };
        unsigned int gridY{ y - m_GridY - 1 };
        if (gridX < m_GridWidth && gridY < m_GridHeight)
        {
            CellID clickedCell{ GetCellState(gridX, gridY) };
            SetCellState(gridX, gridY, (clickedCell + 1) % m_Settings.GetCellDescriptors().size());
        }
    }

    unsigned int Grid::GetNeighborCount(unsigned int x, unsigned int y, CellID searchedCellID) const
    {
        unsigned int neighborCount{ 0 };
        if (GetCellState(x - 1, y - 1) == searchedCellID) { ++neighborCount; }
        if (GetCellState(x - 1, y    ) == searchedCellID) { ++neighborCount; }
        if (GetCellState(x - 1, y + 1) == searchedCellID) { ++neighborCount; }
        if (GetCellState(x    , y - 1) == searchedCellID) { ++neighborCount; }
        if (GetCellState(x    , y + 1) == searchedCellID) { ++neighborCount; }
        if (GetCellState(x + 1, y - 1) == searchedCellID) { ++neighborCount; }
        if (GetCellState(x + 1, y    ) == searchedCellID) { ++neighborCount; }
        if (GetCellState(x + 1, y + 1) == searchedCellID) { ++neighborCount; }
        return neighborCount;
    }

    CellID Grid::GetCellState(unsigned int x, unsigned int y) const
    {
        CellID cellState{ 0 };
        if (x < m_GridWidth && y < m_GridHeight)
        {
            cellState = m_Cells[x + y * m_GridWidth];
        }
        return cellState;
    }

    void Grid::SetCellState(unsigned int x, unsigned int y, CellID newState)
    {
        if (x < m_GridWidth && y < m_GridHeight)
        {
            m_Cells[x + y * m_GridWidth] = newState;
        }
    }
}