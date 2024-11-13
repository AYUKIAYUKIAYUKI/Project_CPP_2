//============================================================================
//
// �v���C���[�A�w�b�_�t�@�C�� [player.h]
// Author : ���c����
//
//============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "character.h"
#include "character_state_manager.h"

//****************************************************
// �O���錾
//****************************************************
class CBounding_Sphere;

//****************************************************
// �v���C���[�N���X
//****************************************************
class CPlayer : public CCharacter
{
public:

	/// <summary> �v���C���[�̍ő�̗� </summary>
	static constexpr int MAX_LIFE = 5;

	/// <summary> �v���C���[�̃f�t�H���g�ړ����x </summary>
	static constexpr float DEFAULT_MOVE_SPEED = 0.005f;

	/// <summary> �ڕW�l�ւ̕�ԋ��x </summary>
	static constexpr float COEF_ADJUST = 0.1f;

	// <special function>
	CPlayer();	// �f�t�H���g�R���X�g���N�^
	~CPlayer();	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;		// �����ݒ�
	void	Uninit() override;		// �I���ݒ�
	void	Update() override;		// �X�V����
	void	Draw() override;		// �`�揈��
	void	To_Damage(int nDamage);	// �X�e�[�g - �_���[�W��Ԃ�

	// <getter>
	float GetRadius() const override;	// ���a���擾
	CCharacter_State* GetNowState();	// ���݂̃X�e�[�g���擾

	// <static function>
	static CPlayer* Create();	// ����

private:

	// <function>
	void HitCheck();		// �����蔻��
	void AdjustHeight();	// ������␳

	// <data>
	CBounding_Sphere*			m_pBndSphere;		// �o�E���f�B���O�X�t�B�A
	CCharacter_State_Manager*	m_pStateManager;	// �X�e�[�g�}�l�[�W���[
};

#endif // _PLAYER_H_