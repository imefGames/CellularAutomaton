#include "gridsettings.h"

#include "grid.h"
#include "renderer.h"

namespace CellularAutomaton
{
    CellTransitionDescriptor::CellTransitionDescriptor(CellID searchedCellID, unsigned int searchedCellCount, CellID destinationCellID)
        : m_SearchedCellID{ searchedCellID }
        , m_SearchedCellCount{ searchedCellCount }
        , m_DestinationCellID{ destinationCellID }
    {
    }

    bool CellTransitionDescriptor::CanTransition(const Grid& grid, unsigned int x, unsigned int y) const
    {
        return grid.GetNeighborCount(x, y, m_SearchedCellID) == m_SearchedCellCount;
    }

    CellDescriptor::CellDescriptor(CellID defaultTransitionCellID, char cellDisplay, EColor cellColor)
        : m_DefaultTransitionCellID{ defaultTransitionCellID }
        , m_CellDisplay{ cellDisplay }
        , m_CellColor{ cellColor }
    {
    }

    CellID CellDescriptor::ComputeNextCellID(const Grid& grid, unsigned int x, unsigned int y) const
    {
        CellID nextCellID{ m_DefaultTransitionCellID };
        for (const CellTransitionDescriptor& transition : m_Transitions)
        {
            if (transition.CanTransition(grid, x, y))
            {
                nextCellID = transition.GetDestinationCellID();
                break;
            }
        }
        return nextCellID;
    }

    GridSettings::GridSettings()
        : m_GridX{ 0 }
        , m_GridY{ 0 }
        , m_GridWidth{ 0 }
        , m_GridHeight{ 0 }
    {
    }

    const CellDescriptor& GridSettings::FindCellDescriptor(CellID cellID) const
    {
        static CellDescriptor nullCellDescriptor{ 0, ' ', EColor::Black };
        if (cellID < m_CellDescriptors.size())
        {
            return m_CellDescriptors[cellID];
        }
        return nullCellDescriptor;
    }
}