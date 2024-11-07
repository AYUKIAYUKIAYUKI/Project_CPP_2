//============================================================================
// 
// X�I�u�W�F�N�g�A�w�b�_�t�@�C�� [object_X.h]
// Author : ���c����
// 
//============================================================================

#ifndef _OBJECT_X_H_
#define _OBJECT_X_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.h"

// X���f�����p
#include "model_X_manager.h"



//****************************************************
// X�I�u�W�F�N�g�N���X
//****************************************************
class CObject_X : public CObject
{
public:

	// <special function>
	CObject_X(LAYER Priority = LAYER::BG);	// �`��D��x�w��R���X�g���N�^
	~CObject_X() override;					// �f�X�g���N�^

	// <function>
	HRESULT	Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <setter>
	void BindModel(CModel_X_Manager::MODEL* pModel);	// ���f������
	void BindModel(CModel_X_Manager::TYPE Type);		// �����ƃ��f������

	// <getter/setter>
	const D3DXVECTOR3& GetPos() const;	// ���W�擾
	void SetPos(D3DXVECTOR3 Pos);		// ���W�ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetRot() const;	// �����擾
	void SetRot(D3DXVECTOR3 Rot);		// �����ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetSize() const;	// �T�C�Y�擾
	void SetSize(D3DXVECTOR3 Size);		// �T�C�Y�ݒ�

	// <getter/setter>
	const float& GetScale() const;	// �k�ڎ擾
	void SetScale(float fScale);	// �k�ڐݒ�

	// <getter/setter>
	const float& GetAlpha() const;	// �A���t�@�l�擾
	void SetAlpha(float fAlpha);	// �A���t�@�l�ݒ�

	// <static function>
	static CObject_X* Create();					// ����
	static CObject_X* Create(LAYER Priority);	// ����

private:

	/// <summary> �k�ڏ������p </summary>
	static constexpr float SCALE_INIT = 1.0f;

	/// <summary> �A���t�@�l�������p </summary>
	static constexpr float ALPHA_INIT = 1.0f;

	// <function>
	void SetMtxWorld();	// ���[���h�s��ݒ�

	// <data>
	CModel_X_Manager::MODEL* m_pModel;	// ���f�����

	// <data>
	D3DXVECTOR3 m_Pos;		// �ʒu
	D3DXVECTOR3 m_Rot;		// ����
	D3DXVECTOR3 m_Size;		// �T�C�Y
	float		m_fScale;	// �k��
	float		m_fAlpha;	// �A���t�@�l
	D3DXMATRIX	m_MtxWorld;	// ���[���h�s��
};

#endif // _OBJECT_X_H_