////============================================================================
////
//// �L�����N�^�[(���[�V����)�A�w�b�_�t�@�C�� [character_motion.h]
//// Author : ���c����
////
////============================================================================
//
//#ifndef _CHARACTER_MOTION_H_
//#define _CHARACTER_MOTION_H_	// ��d�C���N���[�h�h�~
//
////****************************************************
//// �C���N���[�h�t�@�C��
////****************************************************
//#include "motion_set.h"
//
////****************************************************
//// �L�����N�^�[(���[�V����)�N���X
////****************************************************
//class CCharacter_Motion : public CMotion_Set
//{
//public:
//
//	// <special function>
//	CCharacter_Motion();					// �R���X�g���N�^
//	virtual ~CCharacter_Motion() override;	// �f�X�g���N�^
//
//	// <function>
//	HRESULT Init() override;	// �����ݒ�
//	void	Uninit() override;	// �I���ݒ�
//	void	Update() override;	// �X�V����
//	void	Draw() override;	// �`�揈��
//
//	// <setter>
//	void SetCorrectCoef(float fCoef);	// �␳���x��ݒ�
//
//	// <getter/setter>
//	const float& GetDirection() const;		// ���p���擾
//
//	// <getter/setter>
//	const float& GetDirectionTarget() const;	// �ڕW���p���擾
//	void SetDirectionTaget(float fDirection);	// �ڕW���p��ݒ�
//
//	// <getter/setter>
//	const float& GetMoveSpeed() const;		// �ړ����x���擾
//	void SetMoveSpeed(float fMoveSpeed);	// �ړ����x��ݒ�
//
//	// <getter/setter>
//	const D3DXVECTOR3& GetRotTarget() const;	// �ڕW�������擾
//	void SetRotTarget(D3DXVECTOR3 RotTarget);	// �ڕW������ݒ�
//
//	// <getter/setter>
//	const D3DXVECTOR3& GetPosTarget() const;	// �ڕW���W���擾
//	void SetPosTarget(D3DXVECTOR3 PosTarget);	// �ڕW���W��ݒ�
//
//	// <getter/setter>
//	const float& GetVelY() const;	// Y���̉����x���擾
//	void SetVelY(float fAccelY);	// Y���̉����x��ݒ�
//
//	// <getter/setter>
//	const int& GetLife() const;	// �̗͂��擾
//	void SetLife(int nLife);	// �̗͂�ݒ�
//
//protected:
//
//	// <function>
//	void CorrectToTarget();	// �ڕW�l�ւ̕��
//	void AdjustLife();		// �̗͂̒���
//
//	// <virtual function>
//	virtual void AutoSetRotTarget();	// �ڕW�������ړ��������玩���Őݒ�
//	virtual void AutoSetPosTarget();	// �ڕW���W����p���玩���Őݒ�
//
//private:
//
//	// <data>
//	float		m_fCorrectCoef;		// ��ԋ��x
//	float		m_fDirection;		// ���p
//	float		m_fDirectionTarget;	// �ڕW���p
//	float		m_fMoveSpeed;		// �ړ����x
//	D3DXVECTOR3	m_RotTarget;		// �ڕW����
//	D3DXVECTOR3	m_PosTarget;		// �ڕW���W
//	float		m_fVelY;			// Y���̉����x
//	int			m_nLife;			// �̗�
//};
//
//#endif // _PLAYER_H_