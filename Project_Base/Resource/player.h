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

		/// <summary> �v���C���[�̍ő�̗� </summary>
		static constexpr int MAX_LIFE = 5;

		/// <summary> �v���C���[�̃f�t�H���g�ړ����x </summary>
		static constexpr float DEFAULT_MOVE_SPEED = 0.005f;

		CPlayer();	// �f�t�H���g�R���X�g���N�^
		~CPlayer();	// �f�X�g���N�^

		HRESULT Init() override;	// �����ݒ�
		void	Uninit() override;	// �I���ݒ�
		void	Update() override;	// �X�V����
		void	Draw() override;	// �`�揈��

		const int& GetLife() const;	// �̗͂��擾
		void SetLife(int nLife);	// �̗͂�ݒ�

		const Vec3& GetPosTarget() const;	// �ڕW���W���擾
		void SetPosTarget(Vec3 PosTarget);	// �ڕW���W��ݒ�

		const Vec3& GetRotTarget() const;	// �ڕW�������擾
		void SetRotTarget(Vec3 RotTarget);	// �ڕW������ݒ�

		const float& GetMoveSpeed() const;		// �ړ����x���擾
		void SetMoveSpeed(float fMoveSpeed);	// �ړ����x��ݒ�

		const float& GetDirection() const;		// ���p���擾
		void SetDirection(float fDirection);	// ���p��ݒ�

		static CPlayer* Create();	// ����

	private:

		/// <summary> �ڕW�l�ւ̕�ԋ��x </summary>
		static constexpr float COEF_ADJUST = 0.1f;

		void AdjustToTarget();	// �ڕW�l�ւ̕␳

		player_state_manager::CPlayer_State_Manager* m_pPlayerStateManager;	// �v���C���[�X�e�[�g�}�l�[�W���[

		float	m_fDirection;	// ���p
		Vec3	m_PosTarget;	// �ڕW���W
		float	m_fMoveSpeed;	// �ړ����x
		Vec3	m_RotTarget;	// �ڕW����
		int		m_nLife;		// �̗�
	};
}

#endif // _PLAYER_H_