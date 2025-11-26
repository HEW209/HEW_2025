// ImGuiManager.cpp
#include "ImguiManager.h"

ImGuiManager::ImGuiManager()
    : m_initialized(false)
{
}

ImGuiManager& ImGuiManager::Instance()
{
    static ImGuiManager instance;
    return instance;
}

void ImGuiManager::Init(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context)
{
    if (m_initialized)
        return;

    ImGui_ImplWin32_EnableDpiAwareness();

    // ImGuiコンテキスト作成
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Win32 + DX11 backend 初期化
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(device, context);

    ImGui::StyleColorsDark();

    m_initialized = true;
}

void ImGuiManager::Uninit()
{
    if (!m_initialized)
        return;

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    m_initialized = false;
}

void ImGuiManager::BeginFrame()
{
    if (!m_initialized)
        return;

    // 毎フレームの更新処理
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void ImGuiManager::Draw()
{
    if (!m_initialized)
        return;

    // この時点でUI構築完了
    ImGui::Render();

    // ImGuiの描画命令をDX11に送る
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}