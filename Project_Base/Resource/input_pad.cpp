//============================================================================
// 
// 入力処理管理 [input.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "input_pad.h"

//****************************************************
// プリプロセッサディレクティブ
//****************************************************
#define LEFT_JOYSTICK_CONVERT 0	// 左スティック入力を変換

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// デフォルトコンストラクタ
//============================================================================
CInputPad::CInputPad() :
	m_aKeyState{},			// プレス情報
	m_aKeyStateTrigger{}	// トリガー情報
{

}

//============================================================================
// デストラクタ
//============================================================================
CInputPad::~CInputPad()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CInputPad::Init()
{
	// プレス情報クリア
	memset(&m_aKeyState, 0, sizeof(XINPUT_STATE));

	// トリガー情報クリア
	memset(&m_aKeyStateTrigger, 0, sizeof(XINPUT_STATE));

	// XInputのステート設定を有効にする
	XInputEnable(true);

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CInputPad::Uninit()
{
	// XInputのステート設定を無効にする
	XInputEnable(false);

	// インプットクラスの終了処理
	CInput::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CInputPad::Update()
{
	XINPUT_STATE state_joypad = {};	// コントローラの入力情報

	// コントローラの状態を取得
	if (XInputGetState(0, &state_joypad) == ERROR_SUCCESS)
	{
#if LEFT_JOYSTICK_CONVERT	// 左スティック入力を変換

		// 左スティック入力を十字キーに変換
		g_state_joypad.press.Gamepad.wButtons |= ConvertJoyStick();

#else

		// スティック入力の調整
		AdjustJoyStick();

#endif	// LEFT_JOYSTICK_CONVERT

		// コントローラのトリガー情報を保存
		m_aKeyStateTrigger.Gamepad.wButtons = (m_aKeyState.Gamepad.wButtons ^ state_joypad.Gamepad.wButtons) & state_joypad.Gamepad.wButtons;
		m_aKeyStateTrigger.Gamepad.bLeftTrigger = (m_aKeyState.Gamepad.bLeftTrigger ^ state_joypad.Gamepad.bLeftTrigger) & state_joypad.Gamepad.bLeftTrigger;
		m_aKeyStateTrigger.Gamepad.bRightTrigger = (m_aKeyState.Gamepad.bRightTrigger ^ state_joypad.Gamepad.bRightTrigger) & state_joypad.Gamepad.bRightTrigger;

		// コントローラのプレス情報を保存
		m_aKeyState = state_joypad;
	}
}

//============================================================================
// プレス情報取得
//============================================================================
bool CInputPad::GetPress(JOYKEY Key)
{
	return (m_aKeyState.Gamepad.wButtons & (0x01 << static_cast<int>(Key))) ? true : false;
}

//============================================================================
// トリガー情報取得
//============================================================================
bool CInputPad::GetTrigger(JOYKEY Key)
{
	return (m_aKeyStateTrigger.Gamepad.wButtons & (0x01 << static_cast<int>(Key))) ? true : false;
}

//============================================================================
// 左スティックの傾き取得
//============================================================================
CInputPad::JOYSTICK CInputPad::GetJoyStickL(void)
{
	// スティックの傾きを格納
	JOYSTICK joystickL =
	{
		m_aKeyState.Gamepad.sThumbLX,
		m_aKeyState.Gamepad.sThumbLY
	};

	return joystickL;
}

//============================================================================
// 右スティックの傾き取得
//============================================================================
CInputPad::JOYSTICK CInputPad::GetJoyStickR(void)
{
	// スティックの傾きを格納
	JOYSTICK joystickR =
	{
		m_aKeyState.Gamepad.sThumbRX,
		m_aKeyState.Gamepad.sThumbRY
	};

	return joystickR;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// スティック入力の調整
//============================================================================
void CInputPad::AdjustJoyStick(void)
{
	// 左
	if (m_aKeyState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_aKeyState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_aKeyState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_aKeyState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		m_aKeyState.Gamepad.sThumbLX = 0;
		m_aKeyState.Gamepad.sThumbLY = 0;
	}

	// 右
	if (m_aKeyState.Gamepad.sThumbRX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_aKeyState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_aKeyState.Gamepad.sThumbRY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_aKeyState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		m_aKeyState.Gamepad.sThumbRX = 0;
		m_aKeyState.Gamepad.sThumbRY = 0;
	}
}