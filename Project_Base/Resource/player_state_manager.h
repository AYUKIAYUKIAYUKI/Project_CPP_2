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

namespace player_state_manager
{
	//****************************************************
	// using�f�B���N�e�B�u
	//****************************************************
	using namespace abbr;

	//****************************************************
	// �v���C���[�X�e�[�g�N���X
	//****************************************************
	class CPlayer_State_Manager
	{
	public:

		CPlayer_State_Manager();	// �f�t�H���g�R���X�g���N�^
		~CPlayer_State_Manager();	// �f�X�g���N�^

		void Release();	// �j��
		void Update() ;	// �X�V����

		static CPlayer_State_Manager* Create();	// ����

	private:

		HRESULT Init();		// �����ݒ�
		void	Uninit();	// �I���ݒ�
	};
}

#endif	// _PLAYER_STATE_H_