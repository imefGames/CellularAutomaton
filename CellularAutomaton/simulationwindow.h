#pragma once

#include "button.h"
#include "grid.h"
#include "inputhandler.h"
#include "renderer.h"

#include <chrono>
#include <vector>

namespace CellularAutomaton
{
    class GridSettings;

    class SimulationWindow
    {
    public:
        SimulationWindow(const GridSettings& settings);

        void RunSimulationLoop();

    private:
        Grid m_CellularAutomatonGrid;
        InputHandler m_InputHandler;
        Renderer m_Renderer;
        std::vector<Button> m_Buttons;
        std::chrono::microseconds m_GridComputationsDuration;
        std::chrono::microseconds m_MinGridComputationsDuration;
        std::chrono::microseconds m_MaxGridComputationsDuration;
        int m_StepGridComputationsDuration;
        bool m_KeepRunning;
        bool m_IsAutomatonRunning;
        bool m_ForceGridComputation;
    };
}