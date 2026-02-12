cmake_minimum_required(VERSION 3.21)

project(DualShockPCController LANGUAGES CXX)

# Check for Linux and fail if detected
if(UNIX)
    message(FATAL_ERROR "Linux is not supported at the moment. Only Windows is supported.")
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Windows target platform
set(CMAKE_SYSTEM_NAME Windows)

# Qt configuration
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6 packages
find_package(Qt6 COMPONENTS 
    Core 
    Gui 
    Widgets 
    REQUIRED
)

# Find Boost
find_package(Boost REQUIRED COMPONENTS serialization)

# Define the executable
add_executable(${PROJECT_NAME}
    # Main entry point
    DualShockPCController/main.cpp
    
    # Source files
    DualShockPCController/AddCustomCommandDialog.cpp
    DualShockPCController/AddCustomCommandDialog.h
    DualShockPCController/ButtonLayoutSettings.cpp
    DualShockPCController/ButtonLayoutSettings.h
    DualShockPCController/CustomButtonSequence.cpp
    DualShockPCController/CustomButtonSequence.h
    DualShockPCController/CustomButtonConfiguration.cpp
    DualShockPCController/CustomButtonConfiguration.h
    DualShockPCController/DefaultConfigButtonHandler.cpp
    DualShockPCController/DefaultConfigButtonHandler.h
    DualShockPCController/DualShockController.cpp
    DualShockPCController/DualShockController.h
    DualShockPCController/GyroMouseControlSettings.cpp
    DualShockPCController/GyroMouseControlSettings.h
    DualShockPCController/LightBarColorSettings.cpp
    DualShockPCController/LightBarColorSettings.h
    DualShockPCController/MainWindow.cpp
    DualShockPCController/MainWindow.h
    DualShockPCController/MouseSensitivitySettings.cpp
    DualShockPCController/MouseSensitivitySettings.h
    DualShockPCController/OSHelper.cpp
    DualShockPCController/OSHelper.h
    DualShockPCController/RumbleSensitivtySettings.cpp
    DualShockPCController/RumbleSensitivtySettings.h
    DualShockPCController/JoyShockLibrary.h
    
    # UI files
    DualShockPCController/AddCustomCommandDialog.ui
    DualShockPCController/ButtonLayoutSettings.ui
    DualShockPCController/GyroMouseControlSettings.ui
    DualShockPCController/LightBarColorSettings.ui
    DualShockPCController/MainWindow.ui
    DualShockPCController/SensitivitySettings.ui
    
    # Resource files
    DualShockPCController/MainWindow.qrc
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/DualShockPCController
    ${CMAKE_CURRENT_SOURCE_DIR}/JSL
    ${Boost_INCLUDE_DIRS}
)

# Link directories
target_link_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/JSL
)

# Create an imported shared library target for JoyShockLibrary
add_library(JoyShockLibrary SHARED IMPORTED)
set_target_properties(JoyShockLibrary PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/JSL/JoyShockLibrary.dll"
    IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/JSL/JoyShockLibrary.lib"
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Boost::serialization
    JoyShockLibrary
)

# Post-build: Copy JoyShockLibrary.dll to output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/JSL/JoyShockLibrary.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/
)

# Platform-specific settings for Windows
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE ON
    )
endif()

# Optional: Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Set DualShockPCController as the default startup project in Visual Studio
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Find the Qt6 installation plugins directory
get_target_property(QT_CORE_LOCATION Qt6::Core IMPORTED_LOCATION)
get_filename_component(QT_BIN_DIR "${QT_CORE_LOCATION}" DIRECTORY)
get_filename_component(QT_INSTALL_DIR "${QT_BIN_DIR}" DIRECTORY)
set(QT_PLUGINS_DIR "${QT_INSTALL_DIR}/plugins")

# Create qt.conf file in the build output directory to tell Qt where plugins are
file(GENERATE 
    OUTPUT "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/qt.conf"
    CONTENT "[Paths]\nPlugins=./plugins\n"
)

# Post-build: Copy Qt6 DLLs and platform plugins to output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt6::Core> $<TARGET_FILE_DIR:${PROJECT_NAME}>/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt6::Gui> $<TARGET_FILE_DIR:${PROJECT_NAME}>/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt6::Widgets> $<TARGET_FILE_DIR:${PROJECT_NAME}>/
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${QT_PLUGINS_DIR}/platforms/qwindows.dll" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${QT_PLUGINS_DIR}/platforms/qwindowsd.dll" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/qt.conf" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/"
)

