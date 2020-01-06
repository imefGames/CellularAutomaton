#include "grid.h"
#include "inputhandler.h"
#include "renderer.h"

#include <chrono>
#include <thread>

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

    CellularAutomaton::Grid cellularAutomatonGrid{ 1, 2, 50, 20 };
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
        if (isAutomatonRunning && (forceGridComputation || std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastGridComputationTime) > gridComputationsPerSecond))
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