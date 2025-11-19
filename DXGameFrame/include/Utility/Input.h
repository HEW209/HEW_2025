/******************************************************************//**
 * @file   Input.h
 * @brief  入力の取得を行う
 * 
 * @author 石田怜
 * @author 三品怜 - コントローラー入力部分
 * @date   2025/11/19
 *********************************************************************/
#pragma once

#include <Utility/Vector2.h>
#include <Windows.h>
#undef max
#undef min
#include <cstdint>
#include "Xinput.h"

#pragma comment(lib,"Xinput.lib")
#pragma comment(lib,"Xinput9_1_0.lib")

#define MAX_KEY_TYPE (256)			//キーの種類数

 /**
  * @brief キーボード入力コード
  */
enum class KeyCode : uint8_t
{
	//マウスコード
	MOUSE_LEFT = VK_LBUTTON,		//0x01		マウスの左ボタン
	MOUSE_RIGHT = VK_RBUTTON,		//0x02		マウスの右ボタン
	MOUSE_MID = VK_MBUTTON,			//0x04		マウスの中央ボタン
	MOUSE_X1 = VK_XBUTTON1,			//0x05		X1マウスボタン
	MOUSE_X2 = VK_XBUTTON2,			//0x06		X2マウスボタン

	//キーコード
	BACKSPASE = VK_BACK,			//0x08		BackSpaceキー
	TAB = VK_TAB,					//0x09		Tabキー
	CLEAR = VK_CLEAR,				//0x0C		Clearキー
	ENTER = VK_RETURN,				//0x0D		Enterキー
	SHIFT = VK_SHIFT,				//0x10		Shiftキー
	CTRL = VK_CONTROL,				//0x11		Ctrlキー
	ALT = VK_MENU,					//0x12		Altキー
	PAUSE = VK_PAUSE,				//0x13		Pause
	CAPSLK = VK_CAPITAL,			//0x14		CAPS LOCKキー
	NFER = VK_KANA,					//0x15		かなモード
	ESC = VK_ESCAPE,				//0x18		Escキー
	XFER = VK_CONVERT,				//0x1C		IME変換
	SPACE = VK_SPACE,				//0x20		Spaceキー
	PAGEUP = VK_PRIOR,				//0x21		PageUpキー
	PAGEDOWN = VK_NEXT,				//0x22		PageDownキー
	END = VK_END,					//0x23		Endキー
	HOME = VK_HOME,					//0x24		Homeキー
	LEFT = VK_LEFT,					//0x25		左矢印キー
	UP = VK_UP,						//0x26		上矢印キー
	RIGHT = VK_RIGHT,				//0x27		右矢印キー
	DOWN = VK_DOWN,					//0x28		下矢印キー
	INS = VK_INSERT,				//0x2D		Insキー
	DEL = VK_DELETE,				//0x2E		Deleteキー
	HELP = VK_HELP,					//0x2F		Helpキー

	KEY_0 = 0x30,					//0x30		0キー
	KEY_1 = 0x31,					//0x31		1キー
	KEY_2 = 0x32,					//0x32		2キー
	KEY_3 = 0x33,					//0x33		3キー
	KEY_4 = 0x34,					//0x34		4キー
	KEY_5 = 0x35,					//0x35		5キー
	KEY_6 = 0x36,					//0x36		6キー
	KEY_7 = 0x37,					//0x37		7キー
	KEY_8 = 0x38,					//0x38		8キー
	KEY_9 = 0x39,					//0x39		9キー

	A = 0x41,						//0x41		Aキー
	B = 0x42,						//0x42		Bキー
	C = 0x43,						//0x43		Cキー
	D = 0x44,						//0x44		Dキー
	E = 0x45,						//0x45		Eキー
	F = 0x46,						//0x46		Fキー
	G = 0x47,						//0x47		Gキー
	H = 0x48,						//0x48		Hキー
	I = 0x49,						//0x49		Iキー
	J = 0x4A,						//0x4A		Jキー
	K = 0x4B,						//0x4B		Kキー
	L = 0x4C,						//0x4C		Lキー
	M = 0x4D,						//0x4D		Mキー
	N = 0x4E,						//0x4E		Nキー
	O = 0x4F,						//0x4F		Oキー
	P = 0x50,						//0x50		Pキー
	Q = 0x51,						//0x51		Qキー
	R = 0x52,						//0x52		Rキー
	S = 0x53,						//0x53		Sキー
	T = 0x54,						//0x54		Tキー
	U = 0x55,						//0x55		Uキー
	V = 0x56,						//0x56		Vキー
	W = 0x57,						//0x57		Wキー
	X = 0x58,						//0x58		Xキー
	Y = 0x59,						//0x59		Yキー
	Z = 0x5A,						//0x5A		Zキー

	NUMPAD_0 = VK_NUMPAD0,			//0x60		テンキーの[0]
	NUMPAD_1 = VK_NUMPAD1,			//0x61		テンキーの[1]
	NUMPAD_2 = VK_NUMPAD2,			//0x62		テンキーの[2]
	NUMPAD_3 = VK_NUMPAD3,			//0x63		テンキーの[3]
	NUMPAD_4 = VK_NUMPAD4,			//0x64		テンキーの[4]
	NUMPAD_5 = VK_NUMPAD5,			//0x65		テンキーの[5]
	NUMPAD_6 = VK_NUMPAD6,			//0x66		テンキーの[6]
	NUMPAD_7 = VK_NUMPAD7,			//0x67		テンキーの[7]
	NUMPAD_8 = VK_NUMPAD8,			//0x68		テンキーの[8]
	NUMPAD_9 = VK_NUMPAD9,			//0x69		テンキーの[9]
	MUL = VK_MULTIPLY,				//0x6A		テンキーの[*]
	ADD = VK_ADD,					//0x6B		テンキーの[+]
	SUB = VK_SUBTRACT,				//0x6D		テンキーの[-]
	DOT = VK_DECIMAL,				//0x6E		テンキーの[.]
	DIV = VK_DIVIDE,				//0x0F		テンキーの[/]

	F1 = VK_F1,						//0x70		F1キー
	F2 = VK_F2,						//0x71		F2キー
	F3 = VK_F3,						//0x72		F3キー
	F4 = VK_F4,						//0x73		F4キー
	F5 = VK_F5,						//0x74		F5キー
	F6 = VK_F6,						//0x75		F6キー
	F7 = VK_F7,						//0x76		F7キー
	F8 = VK_F8,						//0x77		F8キー
	F9 = VK_F9,						//0x78		F9キー
	F10 = VK_F10,					//0x79		F10キー
	F11 = VK_F11,					//0x7A		F11キー
	F12 = VK_F12,					//0x7B		F12キー

	LSHIFT = VK_LSHIFT,				//0xA0		左Shiftキー
	RSHIFT = VK_RSHIFT,				//0xA1		右Shiftキー
	LCTRL = VK_LCONTROL,			//0xA2		左Ctrlキー
	RCTRL = VK_RCONTROL,			//0xA3		右Ctrlキー
	LALT = VK_LMENU,				//0xA4		左Altキー
	RALT = VK_RMENU,				//0xA5		右Altキー

	COLON = 0xBA,					//0xBA		:キー
	SEMICOLON = 0xBB,				//0xBA		;キー
	LT = 0xBC,						//0xBA		,キー
	MINUS = 0xBD,					//0xBA		-キー
	GT = 0xBE,						//0xBA		.キー
	SLUSH = 0xBF,					//0xBA		?キー
	ATMARK = 0xC0,					//0xBA		@キー

	LBRACE = 0xDB,					//0xBA		[キー
	YEN = 0xDC,						//0xBA		\キー
	RBRACE = 0xDD,					//0xBA		]キー
	XOR = 0xDE,						//0xBA		^キー

	BSLUSH = 0xE2,					//0xBA		アンダーバー
	KANJI = 0xF3,					//0xBA		[半角/全角]キー
	CAPS = 0xF0						//0xBA		[英数][ひらがな]キー
};

/**
 * @brief コントローラー入力コード
 */
enum class PadCode : WORD
{
	UP = 0x0001,
	DOWN = 0x0002,
	LEFT = 0x0004,
	RIGHT = 0x0008,
	START = 0x0010,
	BACK = 0x0020,
	LEFT_THUMB = 0x0040,
	RIGHT_THUMB = 0x0080,
	LEFT_SHOULDER = 0x0100,
	RIGHT_SHOULDER = 0x0200,
	A = 0x1000,
	B = 0x2000,
	X = 0x4000,
	Y = 0x8000,
	LEFT_TRIGGER = 0x0003,
	RIGHT_TRIGGER = 0x0005,
};

/**
 * @brief コントローラースティック入力コード
 */
enum class StickCode : uint8_t
{
	LEFT = 0x00,
	RIGHT = 0x01,
};

/**
 * @brief 入力の取得を行う
 */
class Input
{
public:
	/**
	 * @brief キー入力状況を初期化する
	 */
	static void Init();

	/**
	 * @brief キー入力情報を更新する
	 * 毎フレーム1度だけ呼び出してください。
	 */
	static void Update();

public:
	/**
	 * @brief キーが押されているかを判定する
	 * @param keyCode キーの種類
	 * @return キーが押されているかの判定
	 */
	static bool GetKeyHold(KeyCode keyCode);

	/**
	 * @brief キーが押された瞬間を判定する
	 * @param keyCode キーの種類
	 * @return キーが押された瞬間の判定
	 */
	static bool GetKeyDown(KeyCode keyCode);

	/**
	 * @brief キーが離された瞬間を判定する
	 * @param keyCode キーの種類
	 * @return キーが離された瞬間の判定
	 */
	static bool GetKeyUp(KeyCode keyCode);

	/**
	 * @brief ボタンが押されているかを判定する
	 * @param padCode ボタンの種類
	 * @return ボタンが押されているかの判定
	 */
	static bool GetButtonHold(PadCode padCode);

	/**
	 * @brief ボタンが押された瞬間を判定する
	 * @param padCode ボタンの種類
	 * @return ボタンが押された瞬間の判定
	 */
	static bool GetButtonDown(PadCode padCode);

	/**
	 * @brief ボタンが離された瞬間を判定する
	 * @param padCode ボタンの種類
	 * @return ボタンが離された瞬間の判定
	 */
	static bool GetButtonUp(PadCode padCode);

	/**
	 * @brief 右スティックの入力を取得する
	 * @return 右スティックの入力情報
	 */
	static Vector2 GetRightStick();

	/**
	 * @brief 左スティックの入力を取得する
	 * @return 左スティックの入力情報
	 */
	static Vector2 GetLeftStick();

	/**
	 * @brief スティックの入力を取得する
	 * @param stickCode スティックの種類
	 * @return スティックの入力情報
	 */
	static Vector2 GetStick(StickCode stickCode);

	/**
	 * @brief マウス座標を取得
	 * @return マウス座標
	 */
	static Vector2 GetMousePos();

private:
	Input() = delete;

	/// 前のフレームのキー入力情報
	static uint8_t s_oldKeyTable[MAX_KEY_TYPE];

	/// 現在フレームのキー入力情報
	static uint8_t s_keyTable[MAX_KEY_TYPE];
	
	/// 前フレームのコントローラー入力情報
	static XINPUT_STATE s_oldPadState;

	/// 現在フレームのコントローラー入力情報
	static XINPUT_STATE s_padState;
};