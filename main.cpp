#include "imgui_impl_glfw.h"
#include <cstdlib>
#include "CrystalToolbox.h"
#include <iostream>

int main(){

    CrystalToolbox myapp;

    try
    {
        myapp.Run();
        myapp.Cleanup();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;

}