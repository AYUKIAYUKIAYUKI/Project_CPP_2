//============================================================================
// 
// �^�C�g���A�w�b�_�t�@�C�� [title.h]
// Author : ���c����
// 
//============================================================================

#ifndef _TITLE_H_
#define _TITLE_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "scene.h"

//****************************************************
// �O���錾
//****************************************************
class CMotion_Set;
class CObject_X;

//****************************************************
// �^�C�g���N���X
//****************************************************
class CTitle : public CScene
{
public:

	// <function>
	void Update() override;		// �X�V����
	void Draw() override;		// �`�揈��
	void To_Next() override;	// ���̃V�[����

	// <static function>
	static CTitle* Create();	// �^�C�g���𐶐�

private:

	/// <summary> �^�C�g���̕\�����̃J�����̃Y�[���A�E�g </summary>
	static constexpr float DISTANCE_SHOW_TITLE = 300.0f;

	// <function>
	HRESULT Init() override;		// �����ݒ�
	void	Uninit() override;		// �I������
	void	UpdateEnvironment();	// �������̍X�V
	void	UpdateButterfly();		// ���̍X�V

	// <special function>
	CTitle();			// �R���X�g���N�^
	~CTitle() override;	// �f�X�g���N�^

	// <data>
	bool					 m_bTransition;	// �J�ڃt���O
	CMotion_Set*			 m_pButterfly;	// ��
	std::vector<D3DXVECTOR3> m_vPath;		// �p�X
	int						 m_nNowFrame;	// �t���[��
	int						 m_nMaxFrame;	// �ő�t���[��
	int						 m_nCntStart;	// �X�^�[�g�J�E���g
	CMotion_Set*			 m_pFakePlayer;	// �t�F�C�N�v���C���[
	CMotion_Set*			 m_pTree;		// ��
};

#endif // _TITLE_H_