#pragma once

#include "gridsettings.h"
#include <vector>

namespace CellularAutomaton
{
    class Renderer;

    class Grid
    {
    public:
        Grid(const GridSettings& settings);

        void ComputeNextGrid();
        void RandomizeGrid();
        void ClearGrid();
        void Render(Renderer& renderer);
        void ToggleCellState(unsigned int x, unsigned int y);

        unsigned int GetNeighborCount(unsigned int x, unsigned int y, CellID m_SearchedCellID) const;
        CellID GetCellState(unsigned int x, unsigned int y) const;
        void SetCellState(unsigned int x, unsigned int y, CellID newState);

    private:
        GridSettings m_Settings;
        std::vector<CellID> m_Cells;
        unsigned int m_GridX;
        unsigned int m_GridY;
        unsigned int m_GridWidth;
        unsigned int m_GridHeight;
    };
}