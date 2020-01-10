#include "gridsettingscreator.h"

#include "gridsettings.h"
#include "renderer.h"

namespace CellularAutomaton
{
    namespace GridSettingsCreator
    {
        GridSettings CreateGridSettings(const std::string& filePath)
        {
            GridSettings createdSettings;

            createdSettings.SetGridX(1);
            createdSettings.SetGridY(1);
            createdSettings.SetGridWidth(50);
            createdSettings.SetGridHeight(20);

            createdSettings.GetCellDescriptors().emplace_back(0, ' ', CellularAutomaton::EColor::White);
            createdSettings.GetCellDescriptors().back().GetTransitions().emplace_back(1, 3, 1);

            createdSettings.GetCellDescriptors().emplace_back(0, 219, CellularAutomaton::EColor::White);
            createdSettings.GetCellDescriptors().back().GetTransitions().emplace_back(1, 2, 1);
            createdSettings.GetCellDescriptors().back().GetTransitions().emplace_back(1, 3, 1);

            return createdSettings;
        }
    }
}