////============================================================================
////
//// �v���C���[�a���X�e�[�g�A�w�b�_�t�@�C�� [player_state_slash.h]
//// Author : ���c����
////
////============================================================================
//
//#ifndef _PLAYER_STATE_SLASH_
//#define _PLAYER_STATE_SLASH_	// ��d�C���N���[�h�h�~
//
////****************************************************
//// �C���N���[�h�t�@�C��
////****************************************************
//#include "player_state.h"
//
////****************************************************
//// �v���C���[�W�����v�X�e�[�g�N���X
////****************************************************
//class CPlayer_State_Slash : public CPlayer_State
//{
//public:
//
//	//****************************************************
//	// �a�������̃^�C�v���`
//	//****************************************************
//	enum class SLASH_TYPE : int
//	{
//		LEFT = 0,
//		RIGHT,
//		UP,
//		DOWN,
//		MAX
//	};
//
//	// <special function>
//	CPlayer_State_Slash();			// �R���X�g���N�^
//	~CPlayer_State_Slash() override;	// �f�X�g���N�^
//
//	// <function>
//	void Update() override;		// �X�V����
//	void To_Default() override;	// �ʏ��Ԃ�
//	void To_Dash() override;	// �_���[�W��Ԃ�
//
//private:
//
//	/// <summary> �a���p������ </summary>
//	static constexpr int MAX_DASH_DURATION = 10;
//
//	// <function>
//	void AdjustGravity();	// �d�͂̕␳
//
//	// <data>
//	int			m_nDuration;	// �p������
//	SLASH_TYPE	m_SlashType;	// �a�������̃^�C�v
//};
//
//#endif	// _PLAYER_STATE_SLASH_