// main.cpp
#include <Windows.h>
#include <iostream>
#pragma comment(lib, "winmm.lib")

#include "Config.h"
#include <DXGameFrame.h>
#include "TestScene.h"

// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//----------------------------
	//		ウィンドウの作成
	//----------------------------
	WNDCLASSEX wcex;	// ウィンドウクラス
	HWND hWnd;			// ウィンドウハンドル
	MSG message;		// メッセージ

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


	//-------------------------------------
	//		ライブラリ設定の読み込み
	//-------------------------------------
	if (!ConfigManager::Instance().Load("DXGameFrameConfig.json"))
		return 0;

	//-----------------------------------------
	//		ゲームフレームワークの初期化
	//-----------------------------------------
	// Direct3Dクラスの作成
	if (FAILED(Direct3D::Instance().Init(hWnd, ScreenWidth, ScreenHeight)))
	{
		Debug::ErrorMessage("Direct3Dの初期化に失敗しました");
		return 0;
	}

	ImGuiManager::Instance().Init(
		hWnd, Direct3D::Instance().GetDevice(), Direct3D::Instance().GetContext());

	// シーンの作成
	SceneManager::Init(std::make_unique<TestScene>());


	//-------------------------
	//		その他の準備
	//-------------------------
	srand(time(0));
	timeBeginPeriod(1);				//分解能 (１ミリ秒)

	int nExecLastTime;				//最終実行時間
	int nCrrentTime;				//現在時間
	nExecLastTime = nCrrentTime = timeGetTime();


	//----------------------------
	//		メッセージループ
	//----------------------------
	while (1)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&message, NULL, 0, 0))
			{
				break;
			}
			else
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}
		else
		{
			// FPS固定
			nCrrentTime = timeGetTime();
			if (nCrrentTime - nExecLastTime < 1000.0f / FPS)
			{
				continue;
			}
			nExecLastTime = nCrrentTime;

			// ゲームの処理
			SceneManager::Execute();
		}
	}

	// リソースの解放
	SceneManager::Uninit();
	ImGuiManager::Instance().Uninit();
	Direct3D::Instance().Uninit();
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
		if (IDNO == MessageBox(hWnd, "終了しますか？", "終了確認", MB_YESNO))
		{
			return 0;
		}
		break;

	case WM_DESTROY:
		// スレッドの終了をシステムに伝える
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}