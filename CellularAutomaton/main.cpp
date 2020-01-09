#include "button.h"
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

    unsigned int buttonX{ gridSettings.GetGridX() + gridSettings.GetGridWidth() + 3 };
    unsigned int buttonY{ gridSettings.GetGridY() };
    std::vector<CellularAutomaton::Button> buttons;
    buttons.emplace_back(buttonX, buttonY++, "Toggle Play", ' ', [&isAutomatonRunning]() { isAutomatonRunning = !isAutomatonRunning; });
    buttons.emplace_back(buttonX, buttonY++, "Step", 's', [&forceGridComputation]() { forceGridComputation = true; });
    buttons.emplace_back(buttonX, buttonY++, "Slower", '-', [&gridComputationsPerSecond, maxGridComputationsPerSecond, stepGridComputationsPerSecond]() { if (gridComputationsPerSecond < maxGridComputationsPerSecond) { gridComputationsPerSecond *= stepGridComputationsPerSecond; } });
    buttons.emplace_back(buttonX, buttonY++, "Faster", '+', [&gridComputationsPerSecond, minGridComputationsPerSecond, stepGridComputationsPerSecond]() { if (gridComputationsPerSecond > minGridComputationsPerSecond) { gridComputationsPerSecond /= stepGridComputationsPerSecond; } });
    buttonY++;
    buttons.emplace_back(buttonX, buttonY++, "Randomize", 'r', [&cellularAutomatonGrid]() { cellularAutomatonGrid.RandomizeGrid(); });
    buttons.emplace_back(buttonX, buttonY++, "Clear", 'c', [&cellularAutomatonGrid]() { cellularAutomatonGrid.ClearGrid(); });
    buttonY++;
    buttons.emplace_back(buttonX, buttonY++, "Exit", 'q', [&keepRunning]() { keepRunning = false; });
    
    for (CellularAutomaton::Button& b : buttons)
    {
        b.RegisterButton(inputHandler);
    }
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
        for (CellularAutomaton::Button& b : buttons)
        {
            b.Render(renderer);
        }

        renderer.RenderFrame();
    }

    return 0;
}