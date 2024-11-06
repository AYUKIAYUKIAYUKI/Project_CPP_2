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

	// <function>
	HRESULT Create(HINSTANCE hInstance, HWND hWnd);	// ����
	void	Release();								// ���
	void	Update();								// �X�V����
	void	Draw();									// �`�揈��

	// <getter>
	CMask_Rectangle*	GetMask_Rectangle() const;	// �l�p�`�}�X�N���擾
	CCamera*			GetCamera() const;			// �J�������擾
	CLight*				GetLight() const;			// ���C�g���擾
	CScene*				GetScene() const;			// �V�[�����擾
	
	// <setter>
	void SetScene(CScene::MODE Mode);	// �V�[���̐ݒ�

	// <static getter>
	static CManager*		GetManager();	// �}�l�[�W���[���擾
	static CInputKeyboard*	GetKeyboard();	// �L�[�{�[�h���擾
	static CInputPad*		GetPad();		// �p�b�h���擾

private:

	// <special function>
	CManager();		// �f�t�H���g�R���X�g���N�^
	~CManager();	// �f�X�g���N�^

	// <function>
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// �����ݒ�
	void	Uninit();								// �I������

	// <datas>
	CInputKeyboard*		m_pKeyboard;		// �L�[�{�[�h
	CInputPad*			m_pPad;				// �p�b�h
	CMask_Rectangle*	m_pMask_Rectangle;	// �l�p�`�}�X�N�̃|�C���^
	CCamera*			m_pCamera;			// �J����
	CLight*				m_pLight;			// ���C�g
	CScene*				m_pScene;			// �V�[��

	// <static data>
	static CManager* m_pManager;	// �}�l�[�W���[�̖{��
};

#endif // _MANAGER_H_
