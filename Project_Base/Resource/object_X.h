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
// �O���錾
//****************************************************
class CRender_Collision;

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

	// <getter/setter>
	CModel_X_Manager::MODEL* GetModel();				// ���f���擾
	void BindModel(CModel_X_Manager::MODEL* pModel);	// ���f������
	void BindModel(CModel_X_Manager::TYPE Type);		// �����ƃ��f������

	// <getter/setter>
	const D3DXVECTOR3& GetRot() const;	// �����擾
	void SetRot(D3DXVECTOR3 Rot);		// �����ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetPos() const;	// ���W�擾
	void SetPos(D3DXVECTOR3 Pos);		// ���W�ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetScale() const;	// �k�ڎ擾
	void SetScale(D3DXVECTOR3 Scale);		// �k�ڐݒ�

	// <getter/setter>
	const float& GetAlpha() const;	// �A���t�@�l�擾
	void SetAlpha(float fAlpha);	// �A���t�@�l�ݒ�

	// <virtual getter>
	virtual D3DXVECTOR3 GetSize() const;	// �T�C�Y���擾
	virtual float GetRadius() const;		// ���a���擾
	virtual float GetHeight() const;		// �������擾

	// <static function>
	static CObject_X* Create();												// ����
	static CObject_X* Create(LAYER Priority);								// ����
	static CObject_X* Create(LAYER Priority, CModel_X_Manager::TYPE Type);	// ����
	static CObject_X* Create(JSON Json);									// ����

private:

	/// <summary> �A���t�@�l�������p </summary>
	static constexpr float ALPHA_INIT = 1.0f;

	// <function>
	void AdjustRotAngle();	// �����𒲐�����
	void SetMtxWorld();		// ���[���h�s��ݒ�

	// <data>
	CModel_X_Manager::MODEL*	m_pModel;				// ���f�����
	CRender_Collision*			m_pRender_Collision;	// ����\��
	D3DXVECTOR3					m_Rot;					// ����
	D3DXVECTOR3					m_Pos;					// ���W
	D3DXVECTOR3					m_Scale;				// �k��
	float						m_fAlpha;				// �A���t�@�l
	D3DXMATRIX					m_MtxWorld;				// ���[���h�s��
};

#endif // _OBJECT_X_H_