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
#include "object_X.h"
#include "player_state_manager.h"

//****************************************************
// ���O��Ԃ��`
//****************************************************
namespace player
{
	//****************************************************
	// using�f�B���N�e�B�u
	//****************************************************
	using namespace abbr;

	//****************************************************
	// �v���C���[�N���X
	//****************************************************
	class CPlayer : public CObject_X
	{
	public:

		/// <summary> �f�t�H���g�̃v���C���[�ړ����x </summary>
		static constexpr float DEFAULT_MOVE_SPEED = 0.01f;

		CPlayer();	// �f�t�H���g�R���X�g���N�^
		~CPlayer();	// �f�X�g���N�^

		HRESULT Init() override;	// �����ݒ�
		void	Uninit() override;	// �I���ݒ�
		void	Update() override;	// �X�V����
		void	Draw() override;	// �`�揈��

		const Vec3& GetPosTarget() const;	// �ڕW���W���擾
		void SetPosTarget(Vec3 PosTarget);	// �ڕW���W��ݒ�

		const float& GetDirection() const;		// ���p���擾
		void SetDirection(float fDirection);	// ���p���擾

		static CPlayer* Create();	// ����

	private:

		void Control();	// ����

		player_state_manager::CPlayer_State_Manager* m_pPlayerStateManager;	// �v���C���[�X�e�[�g�}�l�[�W���[

		Vec3	m_PosTarget;	// �ڕW���W
		float	m_fMoveSpeed;	// �ړ����x
		float	m_fDirection;	// ���p
	};
}

#endif // _PLAYER_H_