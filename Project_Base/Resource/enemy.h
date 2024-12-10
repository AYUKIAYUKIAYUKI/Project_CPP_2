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
#include "character.h"

//****************************************************
// �O���錾
//****************************************************
class CPlayer;
class CBounding_Cylinder;

//****************************************************
// �G�l�~�[�N���X
//****************************************************
class CEnemy : public CCharacter
{
public:

	// <special function>
	CEnemy();			// �R���X�g���N�^
	~CEnemy() override;	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;					// �����ݒ�
	void	Uninit() override;					// �I���ݒ�
	void	Update() override;					// �X�V����
	void	Draw() override;					// �`�揈��
	void	SetDamage(int nDamage) override;	// �_���[�W���󂯂�

	// <getter>
	float GetRadius() const;								// ���a���擾
	float GetHeight() const;								// �������擾
	const CBounding_Cylinder* const GetBndCylinder() const;	// �o�E���f�B���O�V�����_�[���擾

protected:

	// <function>
	virtual bool HitCheck(); // �Փˌ��o
	
	// <setter>
	void SetBndSize(float fRadius, float fHeight);	// �o�E���f�B���O�T�C�Y�̐ݒ�

	// <getter>
	CPlayer* const FindPlayer() const;	// �v���C���[����

private:

	// <function>
	void AdjustHeight() override;	// ������␳

	// <data>
	CBounding_Cylinder* m_pBndCylinder;	// �~���o�E���f�B���O
};

#endif // _ENEMY_H_