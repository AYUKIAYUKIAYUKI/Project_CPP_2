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
	CManager();		// �f�t�H���g�R���X�g���N�^
	~CManager();	// �f�X�g���N�^

	// <function>
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);	// �����ݒ�
	void	Uninit();											// �I������
	void	Update();											// �X�V����
	void	Draw();												// �`�揈��

	// �l�p�`�}�X�N�̎擾
	static CMask_Rectangle* GetMask_Rectangle()
	{ return m_pMask_Rectangle; }
	
	// <static getter>
	static CCamera*			GetCamera();	// �J�����擾
	static CLight*			GetLight();		// ���C�g�擾
	static CInputKeyboard*	GetKeyboard();	// �L�[�{�[�h�擾
	static CInputPad*		GetPad();		// �p�b�h�擾
	static CScene*			GetScene();		// �V�[���擾
	
	// <static setter>
	static void SetScene(CScene::MODE mode);	// �V�[���ݒ�

private:

	// <static datas>
	static CMask_Rectangle*	m_pMask_Rectangle;	// �l�p�`�}�X�N�̃|�C���^
	static CCamera*			m_pCamera;			// �J�����Ǘ�
	static CLight*			m_pLight;			// ���C�g�Ǘ�
	static CInputKeyboard*	m_pKeyboard;		// �L�[�{�[�h�Ǘ�
	static CInputPad*		m_pPad;				// �p�b�h�Ǘ�
	static CScene*			m_pScene;			// �V�[���Ǘ�
};

#endif // _MANAGER_H_
