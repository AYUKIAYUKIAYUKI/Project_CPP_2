//============================================================================
//
// �G�l�~�[�A�w�b�_�t�@�C�� [enemy.h]
// Author : ���c����
//
//============================================================================

#ifndef _ENEMY_H_
#define _ENEMY_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_X.h"

//****************************************************
// �G�l�~�[�N���X
//****************************************************
class CEnemy : public CObject_X
{
public:

	/// <summary> �G�l�~�[�ő�̗� </summary>
	static constexpr int MAX_LIFE = 5;

	/// <summary> �G�l�~�[�̃f�t�H���g�ړ����x </summary>
	static constexpr float DEFAULT_MOVE_SPEED = 0.003f;

	CEnemy();	// �f�t�H���g�R���X�g���N�^
	~CEnemy();	// �f�X�g���N�^

	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I���ݒ�
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	const int& GetLife() const;	// �̗͂��擾
	void SetLife(int nLife);	// �̗͂�ݒ�

	const D3DXVECTOR3& GetPosTarget() const;	// �ڕW���W���擾
	void SetPosTarget(D3DXVECTOR3 PosTarget);	// �ڕW���W��ݒ�

	const D3DXVECTOR3& GetRotTarget() const;	// �ڕW�������擾
	void SetRotTarget(D3DXVECTOR3 RotTarget);	// �ڕW������ݒ�

	const float& GetMoveSpeed() const;		// �ړ����x���擾
	void SetMoveSpeed(float fMoveSpeed);	// �ړ����x��ݒ�

	const float& GetDirection() const;		// ���p���擾
	void SetDirection(float fDirection);	// ���p��ݒ�

	static CEnemy* Create();	// ����

private:

	/// <summary> �ڕW�l�ւ̕�ԋ��x </summary>
	static constexpr float COEF_ADJUST = 0.1f;

	void AttackOnPlayer();	// �v���C���[�֍U��
	void AdjustToTarget();	// �ڕW�l�ւ̕␳

	float		m_fDirection;	// ���p
	D3DXVECTOR3	m_PosTarget;	// �ڕW���W
	float		m_fMoveSpeed;	// �ړ����x
	D3DXVECTOR3	m_RotTarget;	// �ڕW����
	int			m_nLife;		// �̗�
};

#endif // _PLAYER_H_