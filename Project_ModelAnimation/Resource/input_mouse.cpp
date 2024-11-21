//============================================================================
// 
// �}�E�X���� [input_mouse.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "input_mouse.h"

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CInputMouse::CInputMouse() :
	m_aKeyState{},
	m_aKeyStateTrigger{},
	m_aKeyStateRelease{}
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CInputMouse::~CInputMouse()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_Input == nullptr)
	{
		// �C���v�b�g�N���X�̏����ݒ�
		if (FAILED(CInput::Init(hInstance, hWnd)))
		{
			return E_FAIL;
		}
	}

	// ���̓f�o�C�X(�}�E�X)�̐���
	if (FAILED(m_Input->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g�̐ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�F�t�H�A�O���E���h
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �}�E�X�ւ̃A�N�Z�X�����l��
	m_pDevice->Acquire();

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CInputMouse::Uninit()
{
	// ���̓f�o�C�X(�}�E�X)�̔j��
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
}

//============================================================================
// �X�V����
//============================================================================
void CInputMouse::Update()
{
	DIMOUSESTATE StateMouse = {};	// �}�E�X���͏��

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &StateMouse)))
	{
		for (WORD wCntKey = 0; wCntKey < 4; ++wCntKey)
		{
			// �}�E�X�̃����[�X����ۑ�
			m_aKeyStateRelease.rgbButtons[wCntKey] = ~m_aKeyState.rgbButtons[wCntKey] & StateMouse.rgbButtons[wCntKey];

			// �}�E�X�̃g���K�[����ۑ�
			m_aKeyStateTrigger.rgbButtons[wCntKey] = m_aKeyState.rgbButtons[wCntKey] & ~StateMouse.rgbButtons[wCntKey];

			// �}�E�X�̃v���X����ۑ�
			m_aKeyState.rgbButtons[wCntKey] = StateMouse.rgbButtons[wCntKey];

			// �}�E�X�̈ړ�����ۑ�
			m_aKeyState.lX = StateMouse.lX;
			m_aKeyState.lY = StateMouse.lY;
		}
	}
	else
	{
		// �}�E�X�̃A�N�Z�X�����l��
		m_pDevice->Acquire();
	}
}

//============================================================================
// �v���X���擾
//============================================================================
bool CInputMouse::GetPress(int nKey)
{
	return ((m_aKeyState.rgbButtons[nKey] & 0x80) != 0) ? true : false;
}

//============================================================================
// �g���K�[���擾
//============================================================================
bool CInputMouse::GetTrigger(int nKey)
{
	return ((m_aKeyStateTrigger.rgbButtons[nKey] & 0x80) != 0) ? true : false;
}

//============================================================================
// �����[�X���擾
//============================================================================
bool CInputMouse::GetRelease(int nKey)
{
	return ((m_aKeyStateRelease.rgbButtons[nKey] & 0x80) != 0) ? true : false;
}

//============================================================================
// �}�E�X�̈ړ��ʂ��擾
//============================================================================
D3DXVECTOR2 CInputMouse::GetMouseMove()
{
	return { static_cast<float>(m_aKeyState.lX), static_cast<float>(m_aKeyState.lY) };
}