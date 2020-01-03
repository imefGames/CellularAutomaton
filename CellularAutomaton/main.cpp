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
    CellularAutomaton::Grid cellularAutomatonGrid{ 1, 2, 50, 20 };
    CellularAutomaton::Renderer renderer;
    CellularAutomaton::InputHandler inputHandler;

    inputHandler.RegisterButtonPressCommand('q', [&keepRunning]() { keepRunning = false; });
    inputHandler.RegisterButtonPressCommand('r', [&cellularAutomatonGrid]() { cellularAutomatonGrid.RandomizeGrid(); });
    inputHandler.RegisterButtonPressCommand('c', [&cellularAutomatonGrid]() { cellularAutomatonGrid.ClearGrid(); });
    inputHandler.RegisterButtonPressCommand('s', [&isAutomatonRunning, &cellularAutomatonGrid]() { if (!isAutomatonRunning) { cellularAutomatonGrid.ComputeNextGrid(); } });
    inputHandler.RegisterButtonPressCommand(' ', [&isAutomatonRunning]() { isAutomatonRunning = !isAutomatonRunning; });
    inputHandler.RegisterMouseClickCommand([&cellularAutomatonGrid](unsigned int clickX, unsigned int clickY) { cellularAutomatonGrid.ToggleCellState(clickX, clickY); });

    while (keepRunning)
    {
        inputHandler.HandleEvents();

        if (isAutomatonRunning)
        {
            cellularAutomatonGrid.ComputeNextGrid();
        }

        cellularAutomatonGrid.Render(renderer);
        renderer.RenderFrame();
        std::this_thread::sleep_for(100ms);
    }

    return 0;
}