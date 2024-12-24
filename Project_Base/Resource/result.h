//============================================================================
// 
// ���U���g�A�w�b�_�t�@�C�� [result.h]
// Author : ���c����
// 
//============================================================================

#ifndef _RESULT_H_
#define _RESULT_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "scene.h"

//****************************************************
// �O���錾
//****************************************************
class CObject_TextMesh;
class CMotion_Set;

//****************************************************
// ���U���g�N���X
//****************************************************
class CResult : public CScene
{
public:

	// <function>
	void Update() override;		// �X�V����
	void Draw() override;		// �`�揈��
	void To_Next() override;	// ���̃V�[����

	// <static function>
	static CResult* Create();	// ���U���g�𐶐�

private:

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	UpdateFakePlayer();	// �t�F�C�N�v���C���[�p�X�V

	// <special function>
	CResult();				// �R���X�g���N�^
	~CResult() override;	// �f�X�g���N�^

	// <Data>
	D3DXVECTOR3  m_Path;			// �p�X
	CMotion_Set* m_pFakePlayer;		// �t�F�C�N�v���C���[
	CObject_TextMesh* m_pTextMesh;	// �e�L�X�g
};

#endif // _RESULT_H_