// main.cpp
#include <Windows.h>
#include <iostream>
#pragma comment(lib, "winmm.lib")


#include "Effekseer/Effekseer.h"
#include "Effekseer/EffekseerRendererDX11.h"
#include "Config.h"
#include "StageSelectScene.h"
#include <DXGameFrame.h>
#include "GameScene.h"

// 最初のシーン
#include "GameScene.h"
#include "InputManager.h"
#include "InputSystem.h"
#include "TitleScene.h"

#include "SoundMaster.h"
#include "SoundManager.h"

static LARGE_INTEGER s_timeFreq;

void InitTimer() {
	QueryPerformanceFrequency(&s_timeFreq);
	timeBeginPeriod(1);
}

double GetTimeSec() {
	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);
	return static_cast<double>(current.QuadPart) / static_cast<double>(s_timeFreq.QuadPart);
}

constexpr double MIN_FRAME_TIME = (FPS > 0.0) ? (1.0 / FPS) : 0.0;

// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	//----------------------------
	//		ウィンドウの作成
	//----------------------------
	WNDCLASSEX wcex;	// ウィンドウクラス
	HWND hWnd;			// ウィンドウハンドル
	MSG message = { 0 };		// メッセージ

	// ウィンドウクラス情報を設定
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.hInstance = hInstance;									// インスタンスハンドル
	wcex.lpszClassName = "Class Name";							// ウィンドウクラス名
	wcex.lpfnWndProc = WndProc;									// ウィンドウプロシージャへのポインタ
	wcex.style = CS_HREDRAW | CS_VREDRAW;						// ウィンドウの挙動
	wcex.cbSize = sizeof(WNDCLASSEX);							// 構造体のサイズ
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);				// アプリアイコン
	wcex.hIconSm = wcex.hIcon;									// アプリアイコン(小)
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);					// カーソルアイコン
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// 背景の色

	// ウィンドウクラス情報の登録
	if (!RegisterClassEx(&wcex))
	{
		Debug::ErrorMessage("ウィンドウクラスの登録に失敗しました");
		return 0;
	}

	// ウィンドウスタイル設定
	DWORD windowStyle;
	windowStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	// ウィンドウのサイズを計算
	RECT windowRect;		// ウィンドウサイズ
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = ScreenWidth;
	windowRect.bottom = ScreenHeight;
	AdjustWindowRect(&windowRect, windowStyle, false);

	// ウィンドウの作成
	hWnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,				// ウィンドウの見た目(Exからの追加スタイル)
		wcex.lpszClassName,					// ウィンドウクラス名
		GameTitle,							// ウィンドウのタイトル
		windowStyle,						// ウィンドウの見た目(スタイル)
		CW_USEDEFAULT, CW_USEDEFAULT,		// ウィンドウの位置
		windowRect.right - windowRect.left,	// ウィンドウのサイズ
		windowRect.bottom - windowRect.top,	// ウィンドウのサイズ
		HWND_DESKTOP,						// 親ウィンドウ
		NULL,								// 子ウィンドウ又はメニュー
		hInstance, NULL
	);

	// エラーチェック
	if (hWnd == NULL) {
		Debug::ErrorMessage("ウィンドウの生成に失敗しました");
		return 0;
	}

	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// COMの初期化
	HRESULT result;
	result = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(result)) return false;


	//-------------------------------------
	//		ライブラリ設定の読み込み
	//-------------------------------------
	if (!ConfigManager::Instance().Load("DXGameFrameConfig.json"))
		return 0;

	//-----------------------------------------
	//		ゲームフレームワークの初期化
	//-----------------------------------------
	// Direct3Dクラスの作成
	RECT clientRc;
	GetClientRect(hWnd, &clientRc);
	int clientW = clientRc.right - clientRc.left;
	int clientH = clientRc.bottom - clientRc.top;

	bool fullScreen = true;
#ifdef _DEBUG
	fullScreen = false;
#endif // _DEBUG


	if (FAILED(Direct3D::Instance().Init(hWnd, clientW, clientH, fullScreen)))
	{
		Debug::ErrorMessage("Direct3Dの初期化に失敗しました");
		return 0;
	}

	ImGuiManager::Instance().Init(
		hWnd, Direct3D::Instance().GetDevice(), Direct3D::Instance().GetContext());

	InputManager::Init();
	// XAudio2初期化
	SoundMaster::Instance().Init();
	// サウンドマネージャー
	SoundManager::Load();

	EffectManager::Instance().Init();

	// シーンの作成
	SceneManager::Init(std::make_unique<TitleScene>());


	//-------------------------
	//		その他の準備
	//-------------------------
	srand(time(0));
	InitTimer();

	double execLastTime;				//最終実行時間
	double crrentTime;				//現在時間
	execLastTime = crrentTime = GetTimeSec();


	//----------------------------
	//		メッセージループ
	//----------------------------
	while (message.message != WM_QUIT)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			// FPS固定
			crrentTime = GetTimeSec();
			double deltaTime = crrentTime - execLastTime;
			if (MIN_FRAME_TIME > 0.0 && deltaTime < MIN_FRAME_TIME)
			{
				continue;
			}
			execLastTime = crrentTime;

			// ゲームの処理
			SceneManager::Execute(static_cast<float>(deltaTime));
			InputManager::Update();
		}
	}

	// リソースの解放
	SceneManager::Uninit();
	ImGuiManager::Instance().Uninit();
	Direct3D::Instance().Uninit();
	SoundManager::StopAll();
	SoundMaster::Instance().Uninit();
	CoUninitialize();
	timeEndPeriod(1);
	return 0;
}


// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_CLOSE:
		//if (IDNO == MessageBox(hWnd, "終了しますか？", "終了確認", MB_YESNO))
		//{
		//	return 0;
		//}
		break;

	case WM_DESTROY:
		// スレッドの終了をシステムに伝える
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}