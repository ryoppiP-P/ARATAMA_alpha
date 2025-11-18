//======================================================================================
//	ゲームウィンドウ [main.h]
//	Author : yumito yokozuka
//	Date   : 2025/07/02
//--------------------------------------------------------------------------------------
// 
//======================================================================================
#include <SDKDDkVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <algorithm>
#include "system_timer.h"
#include "direct3d.h"
#include "shader.h"
#include "texture.h"
#include "sprite.h"
#include "game.h"
#include "fade.h"
#include "debug_ostream.h"
#include "debug_text.h"
#include "key_logger.h"
#include "mouse.h"
#include <sstream>
#include "Audio.h"

#include "scene.h"


//--------------------------------------------------------------------------------------
// ウィンドウ情報
//--------------------------------------------------------------------------------------
static constexpr char WINDOW_CLASS[] = "GameWindou";	//メインウィンドウクラス名
static constexpr char TITLE[] = "荒魂";	//タイトルバーテキスト

//--------------------------------------------------------------------------------------
// ウィンドウプロシージャ　プロトタイプ宣言
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//--------------------------------------------------------------------------------------
// メイン
//--------------------------------------------------------------------------------------
int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE /*hPrevInstance*/, _In_ LPSTR /*lpCmdLine*/, _In_ int nCmdShow)
{
	(void)CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	//DPIスケーリング
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	//ウィンドウクラスの登録
	WNDCLASSEX wcex{};	//{}で初期化

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = WndProc;	// ウィンドウプロシージャの登録
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);	//ウィンドウのアイコンの登録
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);	//ウィンドウのカーソルの登録
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	//ウィンドウの背景色
	wcex.lpszMenuName = nullptr; //メニューは作らない
	wcex.lpszClassName = WINDOW_CLASS;	//情報を引き出すときの名前(識別子)
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);	//スモールアイコンの登録

	RegisterClassEx(&wcex);	//登録

	//クライアント領域のサイズを持った矩形（左からleft, top, right, bottom）
	RECT window_rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//ウィンドウスタイル
	//DWORD window_style = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
	DWORD window_style = WS_POPUP;// フルスクリーンウィンドウ

	//指定したクライアント領域を確保するために新たな矩形座標を計算
	AdjustWindowRect(&window_rect, window_style, FALSE);

	//ウィンドウの幅と高さを算出
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	//プライマリモニターの画面解像度取得
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	//デスクトップの真ん中にウィンドウが生成されるように座標を計算
	//※ただし万が一、デスクトップよりウィンドウが大きい場合は左上を計算
	int window_x = std::max((desktop_width - window_width) / 2, 0);
	int window_y = std::max((desktop_height - window_height) / 2, 0);

	//メインウィンドウの作成
	HWND hWnd = CreateWindow(
		WINDOW_CLASS,	//ウィンドウクラスの識別子
		TITLE,	//ウィンドウのタイトル
		window_style,	//ウィンドウのスタイル
		window_x,	//x座標	//ウィンドウの表示されるときの初期座標	//CW_USEDEFAULT:ランダム座標
		window_y,	//y座標
		window_width,	//幅	//ウィンドウの幅と高さ	////CW_USEDEFAULT:デフォルトサイズ
		window_height,	//高さ
		nullptr,	//親のウィンドウ
		nullptr,	//メニュー
		hInstance,
		nullptr
	);

	SystemTimer_Initialize();
	InitAudio();
	KeyLogger_Initialize();
	Mouse_Initialize(hWnd);

	//各種初期化
	if (!Direct3D_Initialize(hWnd))
	{
		PostQuitMessage(0);
	}
	else
	{
		if (!Shader_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext()))
		{
			PostQuitMessage(0);
		}
		else
		{
			Texture_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext());
			Sprite_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext());
		}
	}

	hal::DebugText dt(Direct3D_GetDevice(), Direct3D_GetDeviceContext(),
		L"resource/texture/consolab_ascii_512.png",
		Direct3D_GetBackBufferWidth(), Direct3D_GetBackBufferHeight(),
		0.0f, 0.0f,
		0, 0,
		0.0f, 0.0f
	);

	//Mouse_SetVisible(false); //マウスカーソル表示ON/OFF----------------------------

	Fade_Initialize();
	Fade_Start(0.0, false);//-----------------------------------------------

	Scene_Initialize();

	//表示するウィンドウ 
	ShowWindow(hWnd, nCmdShow);	//ウィンドウの表示
	UpdateWindow(hWnd);	//ウィンドウの更新
	
	//時間計測用
	double exec_last_time = 0.0;
	double fps_last_time = 0.0;
	double current_time = 0.0;
	ULONG frame_conut = 0;
	double fps = 0.0;

	exec_last_time = fps_last_time = SystemTimer_GetTime();

	//メッセージ & ゲームループ
	MSG msg;

	do 
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{//ウィンドウメッセージが来ていたら
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{//ゲーム処理

			current_time = SystemTimer_GetTime(); //システム時刻を取得
			double elapsed_time = current_time - fps_last_time; //fps計測用の更新時間を計算

			if (elapsed_time >= 1.0) //1秒ごとに実行
			{
				fps = frame_conut / elapsed_time;
				fps_last_time = current_time; //FPSを測定した時刻を保存
				frame_conut = 0; //カウントをクリア
			}

			//1/60秒ごとに実行
			elapsed_time = current_time - exec_last_time;
			if (current_time - exec_last_time >= 1.0 / 60.0)
			{
			//if(true)
				exec_last_time = current_time; //処理した時刻を保存

				KeyLogger_Update();

				Scene_Update(elapsed_time);
				Fade_Update(elapsed_time);

				Direct3D_Clear();

				Scene_Draw();
				Fade_Draw();

#if defined(DEBUG) || defined(_DEBUG)
				std::stringstream ss;
				ss << "fps:" << fps << std::endl;
				dt.SetText(ss.str().c_str());

				dt.Draw();
				dt.Clear();
#endif
				Direct3D_Present();

				frame_conut++;

				Scene_ChangeScene();
			}
		}
	} while (msg.message != WM_QUIT);

	//終了処理
	Scene_Finalize();
	Sprite_Finalize();
	Texture_Finalize();
	Fade_Finalize();
	Shader_Finalize();
	Direct3D_Finalize();
	Mouse_Finalize();
	UninitAudio();

	return (int)msg.wParam;
}

//--------------------------------------------------------------------------------------
// ウィンドウプロシージャ
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_ACTIVATEAPP:
		Keyboard_ProcessMessage(message, wParam, lParam);
		Mouse_ProcessMessage(message, wParam, lParam);
		break;
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Mouse_ProcessMessage(message, wParam, lParam);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{//Escキーで終了
			SendMessage(hWnd, WM_CLOSE, 0, 0);	//WM_CLOSEメッセージの送信
		}
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard_ProcessMessage(message, wParam, lParam);
		break;

	case WM_CLOSE:	//ウィンドウを閉じるメッセージ
		if (MessageBox(hWnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			DestroyWindow(hWnd);	//指定のウィンドウにWM_DESTROYメッセージを送る
		}
		break;	//DefWindowProc関数にメッセージを流さずに終了することによって何もなかったことにする

	case WM_DESTROY:	//ウィンドウ破壊メッセージ
		PostQuitMessage(0);	//WM_QUITメッセージの送信
		break;
	default:
		//通常のメッセージ処理はこの関数に任せる
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//lll