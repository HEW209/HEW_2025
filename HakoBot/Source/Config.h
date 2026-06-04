// プロジェクト設定
#pragma once

/// タイトル
constexpr const char* GameTitle = "ハコボット";

#ifdef _DEBUG
/// ウィンドウの幅
constexpr int ScreenWidth = 1280;

/// ウィンドウの高さ
constexpr int ScreenHeight = 720;

#else
/// ウィンドウの幅
constexpr int ScreenWidth = 1920;

/// ウィンドウの高さ
constexpr int ScreenHeight = 1080;

#endif // _DEBUG

/// 動作FPS
constexpr double FPS = 60.0;