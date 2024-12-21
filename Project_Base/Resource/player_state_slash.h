//============================================================================
//
// �v���C���[�a���X�e�[�g�A�w�b�_�t�@�C�� [player_state_slash.h]
// Author : ���c����
//
//============================================================================

#ifndef _PLAYER_STATE_SLASH_
#define _PLAYER_STATE_SLASH_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "player_state.h"
#include "bounding_sphere.h"

//****************************************************
// �v���C���[�a���X�e�[�g�N���X
//****************************************************
class CPlayer_State_Slash : public CPlayer_State
{
public:

	//****************************************************
	// �a�������̃^�C�v���`
	//****************************************************
	enum class SLASH_TYPE : int
	{
		LEFT = 0,
		RIGHT,
		UP,
		DOWN,
		MAX
	};

	/// <summary> �a���L���X�g���� </summary>
	static constexpr int MAX_SLASH_CAST = 40;

	// <special function>
	CPlayer_State_Slash();				// �R���X�g���N�^
	~CPlayer_State_Slash() override;	// �f�X�g���N�^

	// <function>
	void Update() override;		// �X�V����
	void To_Default() override;	// �ʏ��Ԃ�
	void To_Damage() override;	// �_���[�W��Ԃ�

private:

	// <function>
	void AdjustGravity();	// �d�͂̕␳
	bool SlashHitCheck();	// �a�������蔻��

	// <data>
	SLASH_TYPE							m_SlashType;	// �a�������̃^�C�v
	std::unique_ptr<CBounding_Sphere>	m_pBndSlash;	// �a���̃o�E���f�B���O
	
};

#endif	// _PLAYER_STATE_SLASH_