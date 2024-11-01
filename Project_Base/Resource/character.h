//============================================================================
//
// �L�����N�^�[�A�w�b�_�t�@�C�� [character.h]
// Author : ���c����
//
//============================================================================

#ifndef _CHARACTER_H_
#define _CHARACTER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_X.h"

//****************************************************
// �L�����N�^�[�N���X
//****************************************************
class CCharacter : public CObject_X
{
public:

	CCharacter();			// �f�t�H���g�R���X�g���N�^
	~CCharacter()override;	// �f�X�g���N�^

	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I���ݒ�
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	const float& GetDirection() const;		// ���p���擾
	void SetDirection(float fDirection);	// ���p��ݒ�

	const float& GetMoveSpeed() const;		// �ړ����x���擾
	void SetMoveSpeed(float fMoveSpeed);	// �ړ����x��ݒ�

	const D3DXVECTOR3& GetPosTarget() const;	// �ڕW���W���擾
	void SetPosTarget(D3DXVECTOR3 PosTarget);	// �ڕW���W��ݒ�

	const D3DXVECTOR3& GetRotTarget() const;	// �ڕW�������擾
	void SetRotTarget(D3DXVECTOR3 RotTarget);	// �ڕW������ݒ�

	const int& GetLife() const;	// �̗͂��擾
	void SetLife(int nLife);	// �̗͂�ݒ�

private:

	void CorrectToTarget();	// �ڕW�l�ւ̕␳
	void AdjustLife();		// �̗͂̒���

	float		m_fCorrectionCoef;	// �␳�W��
	float		m_fDirection;		// ���p
	float		m_fMoveSpeed;		// �ړ����x
	D3DXVECTOR3	m_PosTarget;		// �ڕW���W
	D3DXVECTOR3	m_RotTarget;		// �ڕW����
	int			m_nLife;			// �̗�
};

#endif // _PLAYER_H_