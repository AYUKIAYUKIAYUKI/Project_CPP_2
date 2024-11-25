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

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������

	// <special function>
	CTitle();			// �R���X�g���N�^
	~CTitle() override;	// �f�X�g���N�^

	// <data>
	CMotion_Set* m_pTree;	// ��
};

#endif // _TITLE_H_