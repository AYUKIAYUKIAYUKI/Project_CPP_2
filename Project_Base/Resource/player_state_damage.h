//============================================================================
//
// �v���C���[�_���[�W�X�e�[�g�A�w�b�_�t�@�C�� [player_state_damage.h]
// Author : ���c����
//
//============================================================================

#ifndef _PLAYER_STATE_DAMAGE_
#define _PLAYER_STATE_DAMAGE_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "player_state.h"

//****************************************************
// �v���C���[	�_���[�W�X�e�[�g�N���X
//****************************************************
class CPlayer_State_Damage : public CPlayer_State
{
public:

	CPlayer_State_Damage();				// �f�t�H���g�R���X�g���N�^
	~CPlayer_State_Damage() override;	// �f�X�g���N�^

	void Update() override;	// �X�V����

private:

	/// <summary> �_���[�W�p������ </summary>
	static constexpr int MAX_DASH_DURATION = 60;

	int m_nDamageDuration;	// �_���[�W�p������

	void To_Default();	// �ʏ��Ԃ�
};

#endif	// _PLAYER_STATE_DAMAGE_