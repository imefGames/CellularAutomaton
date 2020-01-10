#include "gridsettings.h"
#include "gridsettingscreator.h"
#include "simulationwindow.h"
#include <string>

static const std::string K_CONFIG_FILE_PATH = "CellularAutomatonSettings.txt";

int main()
{
    CellularAutomaton::GridSettings gridSettings{ CellularAutomaton::GridSettingsCreator::CreateGridSettings(K_CONFIG_FILE_PATH) };
    CellularAutomaton::SimulationWindow simulationWindow{ gridSettings };
    simulationWindow.RunSimulationLoop();

    return 0;
}