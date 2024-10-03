//============================================================================
// 
// ���͏����Ǘ� [input.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "input.h"

//****************************************************
// �ÓI�����o�̏�����
//****************************************************
LPDIRECTINPUT8 CInput::m_Input = nullptr;	// DirectInput�I�u�W�F�N�g

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CInput::CInput() :
	m_pDevice{ nullptr }	// ���̓f�o�C�X���
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CInput::~CInput()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_Input == nullptr)
	{
		// DirectInput�I�u�W�F�N�g�̐���
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&m_Input), nullptr)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CInput::Uninit()
{
	// ���̓f�o�C�X�̔j��
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	// DirectInput�I�u�W�F�N�g�̔j��
	if (m_Input != nullptr)
	{
		m_Input->Release();
		m_Input = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CInputKeyboard::CInputKeyboard() :
	m_aKeyState{},			// �v���X���
	m_aKeyStateTrigger{}	// �g���K�[���
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_Input == nullptr)
	{
		// DirectInput�I�u�W�F�N�g�̐���
		if (FAILED(CInput::Init(hInstance, hWnd)))
		{
			return E_FAIL;
		}
	}

	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (m_pDevice == nullptr)
	{
		if (FAILED(m_Input->CreateDevice(GUID_SysKeyboard, &m_pDevice, nullptr)))
		{
			return E_FAIL;
		}
	}

	// �f�[�^�t�H�[�}�b�g�̐ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�F�t�H�A�O���E���h
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
	m_pDevice->Acquire();

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CInputKeyboard::Uninit()
{
	// ���N���X�̏I������
	CInput::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CInputKeyboard::Update()
{
	BYTE state_keyboard[MAX_KEY] = {};	// �L�[���͏��

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(state_keyboard), &state_keyboard[0])))
	{
		for (int i = 0; i < MAX_KEY; i++)
		{
			// �L�[�{�[�h�̃g���K�[����ۑ�
			m_aKeyStateTrigger[i] = (m_aKeyState[i] ^ state_keyboard[i]) & state_keyboard[i];

			// �L�[�{�[�h�̃v���X����ۑ�
			m_aKeyState[i] = state_keyboard[i];
		}
	}
	else
	{
		// �L�[�{�[�h�̃A�N�Z�X�����l��
		m_pDevice->Acquire();
	}
}

//============================================================================
// �v���X���擾
//============================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return ((m_aKeyState[nKey] & 0x80) != 0) ? true : false;
}

//============================================================================
// �g���K�[���擾
//============================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return ((m_aKeyStateTrigger[nKey] & 0x80) != 0) ? true : false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
		// ���X�e�B�b�N���͂�ϊ�
#if LEFT_JOYSTICK_CONVERT

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