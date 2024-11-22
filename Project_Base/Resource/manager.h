//============================================================================
// 
// �}�l�[�W���[�A�w�b�_�t�@�C�� [managaer.h]
// Author : ���c����
// 
//============================================================================

#ifndef _MANAGER_H_
#define _MANAGER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "camera.h"
#include "light.h"
#include "input_keyboard.h"
#include "input_pad.h"
#include "scene.h"

//****************************************************
// �O���錾
//****************************************************
class CMask_Rectangle;

//****************************************************
// �}�l�[�W���[�N���X
//****************************************************
class CManager final
{
public:

	// <special function>
	CManager(const CManager&) = delete;				// �R�s�[�R���X�g���N�^
	CManager& operator=(const CManager&) = delete;	// �R�s�[������Z�q
	CManager(CManager&&) = delete;					// ���[�u�R���X�g���N�^
	CManager& operator=(CManager&&) = delete;		// ���[�u������Z�q

	// <function>
	void Update();	// �X�V����
	void Draw();	// �`�揈��

	// <getter>
	CMask_Rectangle*	GetMask_Rectangle() const;	// �l�p�`�}�X�N���擾
	CCamera*			GetCamera() const;			// �J�������擾
	CLight*				GetLight() const;			// ���C�g���擾
	CScene*				GetScene() const;			// �V�[�����擾
	
	// <setter>
	void SetScene(CScene::MODE Mode);	// �V�[���̐ݒ�

	// <static function>
	static HRESULT Create(HINSTANCE hInstance, HWND hWnd);	// ����
	static void	Release();									// ���

	// <static getter>
	static CManager*		GetManager();	// �}�l�[�W���[���擾
	static CInputKeyboard*	GetKeyboard();	// �L�[�{�[�h���擾
	static CInputPad*		GetPad();		// �p�b�h���擾

private:

	// <special function>
	CManager();		// �R���X�g���N�^
	~CManager();	// �f�X�g���N�^

	// <function>
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// �����ݒ�
	void	Uninit();								// �I������

	// <datas>
	CInputKeyboard*		m_pKeyboard;		// �L�[�{�[�h
	CInputPad*			m_pPad;				// �p�b�h
	CMask_Rectangle*	m_pMask_Rectangle;	// �l�p�`�}�X�N
	CCamera*			m_pCamera;			// �J����
	CLight*				m_pLight;			// ���C�g
	CScene*				m_pScene;			// �V�[��

	// <static data>
	static CManager* m_pManager;	// �}�l�[�W���[�̖{��
};

#endif // _MANAGER_H_
