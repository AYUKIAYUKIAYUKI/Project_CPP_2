//============================================================================
// 
// HUD�I�u�W�F�N�g�A�w�b�_�t�@�C�� [object_HUD.h]
// Author : ���c����
// 
//============================================================================

#ifndef _OBJECT_HUD_H_
#define _OBJECT_HUD_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_2D.h"

//****************************************************
// HUD�I�u�W�F�N�g�N���X
//****************************************************
class CObject_HUD : public CObject_2D
{
public:

	CObject_HUD(int nPriority = static_cast<int>(LAYER::UI));	// �`��D��x�w��R���X�g���N�^
	~CObject_HUD() override;									// �f�X�g���N�^

	HRESULT	Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	const D3DXVECTOR3& GetPosTarget() const;	// �ڕW���W�擾
	void SetPosTarget(D3DXVECTOR3 PosTarget);	// �ڕW���W�ݒ�

	const D3DXVECTOR3& GetRotTarget() const;	// �ڕW�����擾
	void SetRotTarget(D3DXVECTOR3 RotTarget);	// �ڕW�����ݒ�

	const D3DXVECTOR3& GetSizeTarget() const;	// �ڕW�T�C�Y�擾
	void SetSizeTarget(D3DXVECTOR3 SizeTarget);	// �ڕW�T�C�Y�ݒ�

	const D3DXCOLOR& GetColTarget() const;	// �ڕW�F�擾
	void SetColTarget(D3DXCOLOR ColTarget);	// �ڕW�F�ݒ�

	static CObject_HUD* Create();						// ����
	static CObject_HUD* Create(std::string FilePath);	// ����

private:

	/// <summary> �ڕW�l�ւ̕�ԋ��x </summary>
	static constexpr float COEF_ADJUST = 0.1f;

	void AdjustToTarget();	// �ڕW�l�ւ̕␳

	D3DXVECTOR3	m_PosTarget;	// �ڕW���W
	D3DXVECTOR3	m_RotTarget;	// �ڕW����
	D3DXVECTOR3 m_SizeTarget;	// �ڕW�T�C�Y
	D3DXCOLOR	m_ColTarget;	// �ڕW�F
};

#endif // _OBJECT_HUD_H_