#include "grid.h"
#include "gridsettings.h"
#include "inputhandler.h"
#include "renderer.h"

#include <chrono>
#include <thread>

void SetupConwayGridSettings(CellularAutomaton::GridSettings& gridSettings)
{
    gridSettings.SetGridX(1);
    gridSettings.SetGridY(1);
    gridSettings.SetGridWidth(50);
    gridSettings.SetGridHeight(20);

    gridSettings.GetCellDescriptors().emplace_back(0, ' ', CellularAutomaton::EColor::White);
    gridSettings.GetCellDescriptors().back().GetTransitions().emplace_back(1, 3, 1);

    gridSettings.GetCellDescriptors().emplace_back(0, 219, CellularAutomaton::EColor::White);
    gridSettings.GetCellDescriptors().back().GetTransitions().emplace_back(1, 2, 1);
    gridSettings.GetCellDescriptors().back().GetTransitions().emplace_back(1, 3, 1);
}

int main()
{
    using namespace std::chrono_literals;

    bool keepRunning{ true };
    bool isAutomatonRunning{ false };
    bool forceGridComputation{ false };

    int stepGridComputationsPerSecond{ 2 };
    std::chrono::microseconds gridComputationsPerSecond{ 100ms };
    std::chrono::microseconds minGridComputationsPerSecond{ 12500us };
    std::chrono::microseconds maxGridComputationsPerSecond{ 1600ms };

    CellularAutomaton::GridSettings gridSettings{};
    SetupConwayGridSettings(gridSettings);

    CellularAutomaton::Grid cellularAutomatonGrid{ gridSettings };
    CellularAutomaton::Renderer renderer;
    CellularAutomaton::InputHandler inputHandler;

    auto lastGridComputationTime{ std::chrono::high_resolution_clock::now() };
    inputHandler.RegisterButtonPressCommand('q', [&keepRunning]() { keepRunning = false; });
    inputHandler.RegisterButtonPressCommand('r', [&cellularAutomatonGrid]() { cellularAutomatonGrid.RandomizeGrid(); });
    inputHandler.RegisterButtonPressCommand('c', [&cellularAutomatonGrid]() { cellularAutomatonGrid.ClearGrid(); });
    inputHandler.RegisterButtonPressCommand('s', [&forceGridComputation]() { forceGridComputation = true; });
    inputHandler.RegisterButtonPressCommand(' ', [&isAutomatonRunning]() { isAutomatonRunning = !isAutomatonRunning; });
    inputHandler.RegisterButtonPressCommand('+', [&gridComputationsPerSecond, maxGridComputationsPerSecond, stepGridComputationsPerSecond]() { if (gridComputationsPerSecond < maxGridComputationsPerSecond) { gridComputationsPerSecond *= stepGridComputationsPerSecond; } });
    inputHandler.RegisterButtonPressCommand('-', [&gridComputationsPerSecond, minGridComputationsPerSecond, stepGridComputationsPerSecond]() { if (gridComputationsPerSecond > minGridComputationsPerSecond) { gridComputationsPerSecond /= stepGridComputationsPerSecond; } });
    inputHandler.RegisterMouseClickCommand([&cellularAutomatonGrid](unsigned int clickX, unsigned int clickY) { cellularAutomatonGrid.ToggleCellState(clickX, clickY); });

    while (keepRunning)
    {
        inputHandler.HandleEvents();

        auto currentTime{ std::chrono::high_resolution_clock::now() };
        if (isAutomatonRunning && std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastGridComputationTime) > gridComputationsPerSecond || forceGridComputation)
        {
            cellularAutomatonGrid.ComputeNextGrid();
            lastGridComputationTime = std::chrono::high_resolution_clock::now();
            forceGridComputation = false;
        }

        cellularAutomatonGrid.Render(renderer);
        renderer.RenderFrame();
    }

    return 0;
}