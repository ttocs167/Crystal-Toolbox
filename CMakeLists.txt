cmake_minimum_required(VERSION 3.25)
project(imgui_test C CXX)

set(CMAKE_CXX_STANDARD 23)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVK_PROTOTYPES")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_PROTOTYPES")
set (GCC_COVERAGE_LINK_FLAGS    "-static -static-libgcc -static-libstdc++")
set (GCC_COVERAGE_COMPILE_FLAGS "-static -static-libgcc -static-libstdc++")

# GLFW
set(GLFW_DIR ../glfw) # Set this to point to an up-to-date GLFW repo
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)
add_subdirectory(${GLFW_DIR} binary_dir EXCLUDE_FROM_ALL)
include_directories(${GLFW_DIR}/include)

# Dear ImGui
set(IMGUI_DIR ../imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ..)

# Libraries
find_package(Vulkan REQUIRED)
#find_library(VULKAN_LIBRARY
#NAMES vulkan vulkan-1)
#set(LIBRARIES "glfw;${VULKAN_LIBRARY}")
set(LIBRARIES "glfw;Vulkan::Vulkan")


# Use vulkan headers from glfw:
include_directories(${GLFW_DIR}/deps)

file(GLOB sources *.cpp)

add_executable(imgui_test ${sources} ImGuiApp.cpp ImGuiApp.h main.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp Crystal.cpp Crystal.h CrystalToolbox.cpp CrystalToolbox.h stb_image.h)

target_link_libraries(imgui_test ${LIBRARIES})
target_compile_definitions(imgui_test PUBLIC -DImTextureID=ImU64)