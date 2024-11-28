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
// �Q�[���N���X
//****************************************************
class CGame : public CScene
{
public:

	// <function>
	void Update() override;		// �X�V����
	void Draw() override;		// �`�揈��
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
	int m_nApeearLimitBOSS;	// �{�X�o��J�E���g 
};

#endif // _GAME_H_