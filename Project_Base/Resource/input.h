//============================================================================
// 
// ���͏����Ǘ��A�w�b�_�t�@�C�� [input.h]
// Author : ���c����
// 
//============================================================================

#ifndef _INPUT_H_
#define _INPUT_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �}�N����`
//****************************************************
#define MAX_KEY	256	// �L�[�ő吔

// �v���v���Z�b�T�f�B���N�e�B�u
#define LEFT_JOYSTICK_CONVERT 0	// ���X�e�B�b�N���͂�ϊ�

//****************************************************
// �C���v�b�g�N���X
//****************************************************
class CInput
{
public:

	CInput();	// �f�t�H���g�R���X�g���N�^
	~CInput();	// �f�X�g���N�^

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// �����ݒ�
	virtual void Uninit();		// �I������
	virtual void Update() = 0;	// �X�V����

protected:

	static LPDIRECTINPUT8 m_Input;	// DirectInput�I�u�W�F�N�g
	LPDIRECTINPUTDEVICE8 m_pDevice;	// ���̓f�o�C�X���
};

//****************************************************
// �L�[�{�[�h�N���X
//****************************************************
class CInputKeyboard : public CInput
{
public:

	CInputKeyboard();	// �f�t�H���g�R���X�g���N�^
	~CInputKeyboard();	// �f�X�g���N�^

	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;	// �����ݒ�
	void Uninit() override;	// �I������
	void Update() override;	// �X�V����

	bool GetPress(int nKey);	// �v���X���擾
	bool GetTrigger(int nKey);	// �g���K�[���擾

private:

	BYTE m_aKeyState[MAX_KEY];			// �v���X���
	BYTE m_aKeyStateTrigger[MAX_KEY];	// �g���K�[���
};

//****************************************************
// �p�b�h�N���X
//****************************************************
class CInputPad : public CInput
{
public:

	//****************************************************
	// �p�b�h�̃L�[����
	//****************************************************
	enum class JOYKEY
	{
		UP = 0,	// �\����
		DOWN,	// �\����
		LEFT,	// �\����
		RIGHT,	// �\���E
		START,	// �X�^�[�g�{�^��
		BACK,	// �o�b�N�{�^��
		L3,		// ���X�e�B�b�N��������
		R3,		// �E�X�e�B�b�N��������
		L,		// L�{�^��
		R,		// R�{�^��
		NONE1,
		NONE2,
		A,		// A�{�^��
		B,		// B�{�^��
		X,		// X�{�^��
		Y,		// Y�{�^��
		MAX,
	};

	//****************************************************
	// �X�e�B�b�N���̍\����
	//****************************************************
	typedef struct
	{
		SHORT X;
		SHORT Y;
	} JOYSTICK;

	CInputPad();	// �f�t�H���g�R���X�g���N�^
	~CInputPad();	// �f�X�g���N�^

	HRESULT Init();			// �����ݒ�
	void Uninit() override;	// �I������
	void Update() override;	// �X�V����

	bool GetPress(JOYKEY Key);		// �v���X���擾
	bool GetTrigger(JOYKEY Key);	// �g���K�[���擾
	JOYSTICK GetJoyStickL();		// ���X�e�B�b�N�̌X���擾
	JOYSTICK GetJoyStickR();		// �E�X�e�B�b�N�̌X���擾

private:

	void AdjustJoyStick();	// �X�e�B�b�N�̒���

	XINPUT_STATE m_aKeyState;			// �v���X���
	XINPUT_STATE m_aKeyStateTrigger;	// �g���K�[���
};

#endif // _INPUT_H_