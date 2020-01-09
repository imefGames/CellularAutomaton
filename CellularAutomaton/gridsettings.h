#pragma once

#include <vector>

namespace CellularAutomaton
{
    class Grid;
    enum class EColor : unsigned char;

    using CellID = unsigned int;

    class CellTransitionDescriptor
    {
    public:
        CellTransitionDescriptor(CellID searchedCellID, unsigned int searchedCellCount, CellID destinationCellID);

        CellID GetDestinationCellID() const { return m_DestinationCellID; }
        bool CanTransition(const Grid& grid, unsigned int x, unsigned int y) const;

    private:
        CellID m_DestinationCellID;
        CellID m_SearchedCellID;
        unsigned int m_SearchedCellCount;
    };

    class CellDescriptor
    {
    public:
        CellDescriptor(CellID defaultTransitionCellID, char cellDisplay, EColor cellColor);

        inline char GetCellDisplay() const { return m_CellDisplay; }
        inline EColor GetCellColor() const { return m_CellColor; }
        inline std::vector<CellTransitionDescriptor>& GetTransitions() { return m_Transitions; }

        inline void SetDefaultTransitionCellID(CellID defaultTransitionCellID) { m_DefaultTransitionCellID = defaultTransitionCellID; }

        CellID ComputeNextCellID(const Grid& grid, unsigned int x, unsigned int y) const;

    private:
        std::vector<CellTransitionDescriptor> m_Transitions;
        CellID m_DefaultTransitionCellID;
        char m_CellDisplay;
        EColor m_CellColor;
    };

    class GridSettings
    {
    public:
        GridSettings();

        const CellDescriptor& FindCellDescriptor(CellID cellID) const;

        inline std::vector<CellDescriptor>& GetCellDescriptors() { return m_CellDescriptors; }
        inline unsigned int GetGridX() const { return m_GridX; }
        inline unsigned int GetGridY() const { return m_GridY; }
        inline unsigned int GetGridWidth() const { return m_GridWidth; }
        inline unsigned int GetGridHeight() const { return m_GridHeight; }

        inline void SetGridX(unsigned int x) { m_GridX = x; }
        inline void SetGridY(unsigned int y) { m_GridY = y; }
        inline void SetGridWidth(unsigned int width) { m_GridWidth = width; }
        inline void SetGridHeight(unsigned int height) { m_GridHeight = height; }

    private:

        std::vector<CellDescriptor> m_CellDescriptors;

        unsigned int m_GridX;
        unsigned int m_GridY;
        unsigned int m_GridWidth;
        unsigned int m_GridHeight;
    };
}