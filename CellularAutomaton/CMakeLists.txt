cmake_minimum_required (VERSION 3.8)

add_executable (CellularAutomaton "main.cpp"
                                  "grid.h" "grid.cpp"
                                  "renderer.h" "renderer.cpp"
                                  "inputhandler.h" "inputhandler.cpp"
                                  "gridsettings.h" "gridsettings.cpp"
                                  "button.h" "button.cpp"
                                  )

add_custom_command(TARGET CellularAutomaton POST_BUILD 
    COMMAND "${CMAKE_COMMAND}" -E copy 
    ${CMAKE_CURRENT_SOURCE_DIR}/CellularAutomatonSettings.txt
    ${CMAKE_CURRENT_BINARY_DIR}/CellularAutomatonSettings.txt
    COMMENT "Copying to output directory")