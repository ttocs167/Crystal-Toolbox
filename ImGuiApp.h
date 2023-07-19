#pragma once

#include <imgui.h>
#include "GLFW/glfw3.h"
#include "Crystal.h"
#include <imgui_impl_vulkan.h>

// A struct to manage data related to one image in vulkan
struct MyTextureData
{
    VkDescriptorSet DS;         // Descriptor set: this is what you'll pass to Image()
    int             Width;
    int             Height;
    int             Channels;

    // Need to keep track of these to properly cleanup
    VkImageView     ImageView;
    VkImage         Image;
    VkDeviceMemory  ImageMemory;
    VkSampler       Sampler;
    VkBuffer        UploadBuffer;
    VkDeviceMemory  UploadBufferMemory;

    MyTextureData() { memset(this, 0, sizeof(*this)); }
};

bool LoadTextureFromFile(const char* filename, MyTextureData* tex_data);
bool LoadTextureFromCrystal(Crystal* crystal, MyTextureData* tex_data);
bool UpdateCrystalImage(Crystal* crystal, MyTextureData* tex_data);

void RemoveTexture(MyTextureData* tex_data);

class ImGuiApp
{
// public variables
public:


// public methods
public:
    // constructor and destructor
    ImGuiApp();
    ~ImGuiApp()= default;
    void Init(GLFWwindow *window);
    void NewFrame(GLFWwindow *window);
    virtual void Update();
    static void Render();
    void Shutdown(GLFWwindow *window);
    void Run();

// private variables
private:
    GLFWwindow* m_Window;
};
