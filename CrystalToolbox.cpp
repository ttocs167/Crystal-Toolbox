#include "CrystalToolbox.h"
#include "ImGuiApp.h"

MyTextureData my_texture;
bool isImageLoaded = false;

//constructor
CrystalToolbox::CrystalToolbox() : m_Crystal({500,500}){
    m_Crystal.maxCrystalSize = 1000;
}

void CrystalToolbox::UpdateCrystal(){
    // custom imgui window
    {

        static int growthSpeed = 1;
        ImGui::Begin("Custom window!", nullptr, ImGuiWindowFlags_MenuBar);
        {
            ImGui::Text("Desired crystal size");
            ImGui::SliderInt(" ", &m_Crystal.maxCrystalSize, 10, 20000);

            ImGui::SliderInt("Growth Speed", &growthSpeed, 1, 30);

            if (ImGui::Button("Toggle Growing Crystal")){
                m_Crystal.isGrowing = !m_Crystal.isGrowing;
                m_Crystal.StartTimer();
            }

            ImGui::Text("crystal size: %d", m_Crystal.currentCrystalSize);
            ImGui::Text("Time taken to grow: %.3f s", m_Crystal.timeTaken);
            ImGui::Text("Current spawn radius: %d", m_Crystal.currentSpawnRadius);
        }

        if(m_Crystal.isGrowing){
            ImGui::Text("Growing...");
            for (int i = 0; i < growthSpeed; ++i)
                m_Crystal.RandomWalk();
        }
        else{
            ImGui::Text("Not growing");
        }

        ImGui::End();
    }
}

void CrystalToolbox::Update() {
    UpdateCrystal();
    ImageWindow();
}

void CrystalToolbox::LoadImageOnce(){
    bool ret = LoadTextureFromCrystal(&m_Crystal, &my_texture);
    IM_ASSERT (ret);
    isImageLoaded = true;
}

void CrystalToolbox::ImageWindow() {
    if (!isImageLoaded)
        LoadImageOnce();

    ImGui::Begin("Crystal Image");
    {
        //ImGui::Image((void *) m_Crystal.m_CrystalImage, ImVec2(m_Crystal._width, m_Crystal._height));
        ImGui::Text("pointer = %p", my_texture.DS);
        ImGui::Text("size = %d x %d", my_texture.Width, my_texture.Height);

        //TODO update image here. Without memory leaks this time

        UpdateCrystalImage(&m_Crystal, &my_texture);

        ImGui::Image((ImTextureID)my_texture.DS, ImVec2(my_texture.Width, my_texture.Height));

    }
    ImGui::End();

}

void CrystalToolbox::Cleanup() {
    RemoveTexture(&my_texture);
}
