//============================================================================
//
// �v���C���[�X�e�[�g�}�l�[�W���[�A�w�b�_�t�@�C�� [player_state_manager.h]
// Author : ���c����
//
//============================================================================

#ifndef _PLAYER_STATE_MANAGER_H_
#define _PLAYER_STATE_MANAGER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "state.h"
#include "player_state_default.h"

//****************************************************
// ���O��Ԃ̒�`
//****************************************************
namespace player_state_manager
{
	//****************************************************
	// using�f�B���N�e�B�u
	//****************************************************
	using namespace abbr;

	//****************************************************
	// �v���C���[�X�e�[�g�}�l�[�W���[�N���X
	//****************************************************
	class CPlayer_State_Manager final
	{
	public:

		CPlayer_State_Manager();	// �f�t�H���g�R���X�g���N�^
		~CPlayer_State_Manager();	// �f�X�g���N�^

		void Release();	// �j��
		void Update() ;	// �X�V����

		void SetState(CState* pState);	// ��Ԃ�ݒ�

		static CPlayer_State_Manager* Create();	// ����

	private:

		HRESULT Init();		// �����ݒ�
		void	Uninit();	// �I���ݒ�

		CState* m_pState;	// �X�e�[�g
	};
}

#endif	// _PLAYER_STATE_H_