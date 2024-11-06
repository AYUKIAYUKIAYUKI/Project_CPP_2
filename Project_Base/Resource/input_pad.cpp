//============================================================================
// 
// ���͏����Ǘ� [input.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "input_pad.h"

//****************************************************
// �v���v���Z�b�T�f�B���N�e�B�u
//****************************************************
#define LEFT_JOYSTICK_CONVERT 0	// ���X�e�B�b�N���͂�ϊ�

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CInputPad::CInputPad() :
	m_aKeyState{},			// �v���X���
	m_aKeyStateTrigger{}	// �g���K�[���
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CInputPad::~CInputPad()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CInputPad::Init()
{
	// �v���X���N���A
	memset(&m_aKeyState, 0, sizeof(XINPUT_STATE));

	// �g���K�[���N���A
	memset(&m_aKeyStateTrigger, 0, sizeof(XINPUT_STATE));

	// XInput�̃X�e�[�g�ݒ��L���ɂ���
	XInputEnable(true);

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CInputPad::Uninit()
{
	// XInput�̃X�e�[�g�ݒ�𖳌��ɂ���
	XInputEnable(false);

	// �C���v�b�g�N���X�̏I������
	CInput::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CInputPad::Update()
{
	XINPUT_STATE state_joypad = {};	// �R���g���[���̓��͏��

	// �R���g���[���̏�Ԃ��擾
	if (XInputGetState(0, &state_joypad) == ERROR_SUCCESS)
	{
#if LEFT_JOYSTICK_CONVERT	// ���X�e�B�b�N���͂�ϊ�

		// ���X�e�B�b�N���͂��\���L�[�ɕϊ�
		g_state_joypad.press.Gamepad.wButtons |= ConvertJoyStick();

#else

		// �X�e�B�b�N���͂̒���
		AdjustJoyStick();

#endif	// LEFT_JOYSTICK_CONVERT

		// �R���g���[���̃g���K�[����ۑ�
		m_aKeyStateTrigger.Gamepad.wButtons = (m_aKeyState.Gamepad.wButtons ^ state_joypad.Gamepad.wButtons) & state_joypad.Gamepad.wButtons;
		m_aKeyStateTrigger.Gamepad.bLeftTrigger = (m_aKeyState.Gamepad.bLeftTrigger ^ state_joypad.Gamepad.bLeftTrigger) & state_joypad.Gamepad.bLeftTrigger;
		m_aKeyStateTrigger.Gamepad.bRightTrigger = (m_aKeyState.Gamepad.bRightTrigger ^ state_joypad.Gamepad.bRightTrigger) & state_joypad.Gamepad.bRightTrigger;

		// �R���g���[���̃v���X����ۑ�
		m_aKeyState = state_joypad;
	}
}

//============================================================================
// �v���X���擾
//============================================================================
bool CInputPad::GetPress(JOYKEY Key)
{
	return (m_aKeyState.Gamepad.wButtons & (0x01 << static_cast<int>(Key))) ? true : false;
}

//============================================================================
// �g���K�[���擾
//============================================================================
bool CInputPad::GetTrigger(JOYKEY Key)
{
	return (m_aKeyStateTrigger.Gamepad.wButtons & (0x01 << static_cast<int>(Key))) ? true : false;
}

//============================================================================
// ���X�e�B�b�N�̌X���擾
//============================================================================
CInputPad::JOYSTICK CInputPad::GetJoyStickL(void)
{
	// �X�e�B�b�N�̌X�����i�[
	JOYSTICK joystickL =
	{
		m_aKeyState.Gamepad.sThumbLX,
		m_aKeyState.Gamepad.sThumbLY
	};

	return joystickL;
}

//============================================================================
// �E�X�e�B�b�N�̌X���擾
//============================================================================
CInputPad::JOYSTICK CInputPad::GetJoyStickR(void)
{
	// �X�e�B�b�N�̌X�����i�[
	JOYSTICK joystickR =
	{
		m_aKeyState.Gamepad.sThumbRX,
		m_aKeyState.Gamepad.sThumbRY
	};

	return joystickR;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �X�e�B�b�N���͂̒���
//============================================================================
void CInputPad::AdjustJoyStick(void)
{
	// ��
	if (m_aKeyState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_aKeyState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_aKeyState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_aKeyState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		m_aKeyState.Gamepad.sThumbLX = 0;
		m_aKeyState.Gamepad.sThumbLY = 0;
	}

	// �E
	if (m_aKeyState.Gamepad.sThumbRX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_aKeyState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_aKeyState.Gamepad.sThumbRY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_aKeyState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		m_aKeyState.Gamepad.sThumbRX = 0;
		m_aKeyState.Gamepad.sThumbRY = 0;
	}
}