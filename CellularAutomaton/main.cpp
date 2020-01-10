#include "button.h"
#include "grid.h"
#include "gridsettings.h"
#include "gridsettingscreator.h"
#include "inputhandler.h"
#include "renderer.h"

#include <chrono>
#include <string>
#include <thread>

static const std::string K_CONFIG_FILE_PATH = "CellularAutomatonSettings.txt";

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

    CellularAutomaton::GridSettings gridSettings{ CellularAutomaton::GridSettingsCreator::CreateGridSettings(K_CONFIG_FILE_PATH) };
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