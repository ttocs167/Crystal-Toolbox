#pragma once
#include "ImGuiApp.h"
#include "Crystal.h"

class CrystalToolbox : public ImGuiApp{
    //public methods
public:
    // constructor and destructor
    CrystalToolbox();
    ~CrystalToolbox() = default;
    void Update() override;
    void ImageWindow();
    void Cleanup();
    void LoadImageOnce();

    // private variables
private:
    Crystal m_Crystal;

    // private methods
private:
    void UpdateCrystal();
};
