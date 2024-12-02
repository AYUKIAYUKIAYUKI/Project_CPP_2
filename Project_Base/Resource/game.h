//============================================================================
// 
// �Q�[���A�w�b�_�t�@�C�� [game.h]
// Author : ���c����
// 
//============================================================================

#ifndef _GAME_H_
#define _GAME_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "scene.h"

//****************************************************
// �O���錾
//****************************************************
class CObject_HUD;

//****************************************************
// �Q�[���N���X
//****************************************************
class CGame : public CScene
{
public:

	// <function>
	void Update() override;		// �X�V����
	void Draw() override;		// �`�揈��
	void SetTransition();		// �J�ڊJ�n
	void To_Next() override;	// ���̃V�[����

	// <static function>
	static CGame* Create();	// �Q�[���𐶐�

private:

	// <special function>
	CGame();			// �R���X�g���N�^
	~CGame() override;	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������

	// <data>
	bool		 m_bTransition;	// �J�ڃt���O
	CObject_HUD* m_pRenderFade;	// �t�F�[�h�\��
};

#endif // _GAME_H_