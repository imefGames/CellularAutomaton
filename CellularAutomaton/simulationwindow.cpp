#include "simulationwindow.h"

#include "gridsettings.h"

namespace CellularAutomaton
{
    SimulationWindow::SimulationWindow(const GridSettings& settings)
        : m_CellularAutomatonGrid{ settings }
        , m_InputHandler{}
        , m_Renderer{}
        , m_StepGridComputationsDuration{ 2 }
        , m_KeepRunning{ true }
        , m_IsAutomatonRunning{ false }
        , m_ForceGridComputation{ false }
    {
        using namespace std::chrono_literals;
        m_GridComputationsDuration = 100ms;
        m_MinGridComputationsDuration = 12500us;
        m_MaxGridComputationsDuration = 1600ms;

        unsigned int buttonX{ settings.GetGridX() + settings.GetGridWidth() + 3 };
        unsigned int buttonY{ settings.GetGridY() };
        m_Buttons.emplace_back(buttonX, buttonY++, "Toggle Play", ' ', [this]() { m_IsAutomatonRunning = !m_IsAutomatonRunning; });
        m_Buttons.emplace_back(buttonX, buttonY++, "Step", 's', [this]() { m_ForceGridComputation = true; });
        m_Buttons.emplace_back(buttonX, buttonY++, "Slower", '-', [this]() { if (m_GridComputationsDuration < m_MaxGridComputationsDuration) { m_GridComputationsDuration *= m_StepGridComputationsDuration; } });
        m_Buttons.emplace_back(buttonX, buttonY++, "Faster", '+', [this]() { if (m_GridComputationsDuration > m_MinGridComputationsDuration) { m_GridComputationsDuration /= m_StepGridComputationsDuration; } });
        buttonY++;
        m_Buttons.emplace_back(buttonX, buttonY++, "Randomize", 'r', [this]() { m_CellularAutomatonGrid.RandomizeGrid(); });
        m_Buttons.emplace_back(buttonX, buttonY++, "Clear", 'c', [this]() { m_CellularAutomatonGrid.ClearGrid(); });
        buttonY++;
        m_Buttons.emplace_back(buttonX, buttonY++, "Exit", 'q', [this]() { m_KeepRunning = false; });

        for (CellularAutomaton::Button& b : m_Buttons)
        {
            b.RegisterButton(m_InputHandler);
        }
        m_InputHandler.RegisterMouseClickCommand([this](unsigned int clickX, unsigned int clickY) { m_CellularAutomatonGrid.ToggleCellState(clickX, clickY); });
    }

    void SimulationWindow::RunSimulationLoop()
    {
        auto lastGridComputationTime{ std::chrono::high_resolution_clock::now() };
        while (m_KeepRunning)
        {
            m_InputHandler.HandleEvents();

            auto currentTime{ std::chrono::high_resolution_clock::now() };
            if (m_IsAutomatonRunning && std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastGridComputationTime) > m_GridComputationsDuration || m_ForceGridComputation)
            {
                m_CellularAutomatonGrid.ComputeNextGrid();
                lastGridComputationTime = std::chrono::high_resolution_clock::now();
                m_ForceGridComputation = false;
            }

            m_CellularAutomatonGrid.Render(m_Renderer);
            for (CellularAutomaton::Button& b : m_Buttons)
            {
                b.Render(m_Renderer);
            }

            m_Renderer.RenderFrame();
        }
    }
}