cmake_minimum_required(VERSION 3.20)
project(imgui_sfml_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================
# SFML (УСТАНОВЛЕННЫЙ)
# =========================
set(SFML_DIR "C:/SFML/lib/cmake/SFML")
find_package(SFML 2.6 REQUIRED COMPONENTS graphics window system)

include(FetchContent)

# =========================
# IMGUI
# =========================
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.90.9
)

FetchContent_MakeAvailable(imgui)

# =========================
# CRITICAL FIX
# =========================
set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "" FORCE)
set(IMGUI_DIR ${imgui_SOURCE_DIR})

# =========================
# IMGUI-SFML
# =========================
FetchContent_Declare(
    imgui-sfml
    GIT_REPOSITORY https://github.com/eliasdaler/imgui-sfml.git
    GIT_TAG v2.6
)

FetchContent_MakeAvailable(imgui-sfml)

# =========================
# APP
# =========================
add_executable(app 
    app/main.cpp 
    src/Particle.cpp 
    src/FixedCharge.cpp 
    src/Physics.cpp 
    src/Accelerator.cpp
)

# =========================
# DLL COPY
# =========================
add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "C:/SFML/bin"
    $<TARGET_FILE_DIR:app>
)

target_include_directories(app PRIVATE 
    ${imgui_SOURCE_DIR}
    ${imgui-sfml_SOURCE_DIR}
)

target_link_libraries(app PRIVATE 
    sfml-graphics 
    sfml-window 
    sfml-system 
    ImGui-SFML::ImGui-SFML
)