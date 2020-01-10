#pragma once

#include <string>

namespace CellularAutomaton
{
    class GridSettings;

    namespace GridSettingsCreator
    {
        GridSettings CreateGridSettings(const std::string& filePath);
    }
}