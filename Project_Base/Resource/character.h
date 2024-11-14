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

	// <special function>
	CCharacter();					// �f�t�H���g�R���X�g���N�^
	virtual ~CCharacter() override;	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I���ݒ�
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <getter/setter>
	const float& GetDirection() const;		// ���p���擾
	void SetDirection(float fDirection);	// ���p��ݒ�

	// <getter/setter>
	const float& GetMoveSpeed() const;		// �ړ����x���擾
	void SetMoveSpeed(float fMoveSpeed);	// �ړ����x��ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetRotTarget() const;	// �ڕW�������擾
	void SetRotTarget(D3DXVECTOR3 RotTarget);	// �ڕW������ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetPosTarget() const;	// �ڕW���W���擾
	void SetPosTarget(D3DXVECTOR3 PosTarget);	// �ڕW���W��ݒ�

	// <getter/setter>
	const float& GetAccelY() const;	// Y���̉����x���擾
	void SetAccelY(float fAccelY);	// Y���̉����x��ݒ�

	// <getter/setter>
	const int& GetLife() const;	// �̗͂��擾
	void SetLife(int nLife);	// �̗͂�ݒ�

protected:

	// <function>
	void CorrectToTarget();		// �ڕW�l�ւ̕␳
	void AdjustLife();			// �̗͂̒���

	// <virtual function>
	virtual void AutoSetRotTarget();	// �ڕW�������ړ��������玩���Őݒ�
	virtual void AutoSetPosTarget();	// �ڕW���W����p���玩���Őݒ�

private:

	// <data>
	float		m_fCorrectionCoef;	// �␳�W��
	float		m_fDirection;		// ���p
	float		m_fMoveSpeed;		// �ړ����x
	D3DXVECTOR3	m_RotTarget;		// �ڕW����
	D3DXVECTOR3	m_PosTarget;		// �ڕW���W
	float		m_fAccelY;			// Y���̉����x
	int			m_nLife;			// �̗�
};

#endif // _PLAYER_H_