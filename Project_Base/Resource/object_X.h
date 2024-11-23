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
#include "X_manager.h"

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
	CX_Manager::MODEL* GetModel();				// ���f���擾
	void BindModel(CX_Manager::MODEL* pModel);	// ���f������
	void BindModel(CX_Manager::TYPE Type);		// �����ƃ��f������

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
	const D3DXCOLOR& GetCol() const;	// �F���擾
	void SetCol(D3DXCOLOR Col);			// �F��ݒ�

	// <getter/setter>
	const bool& GetUseCol() const;	// �F���f���擾
	void SetUseCol(bool Col);		// �F���f��ݒ�

	// <virtual getter>
	virtual D3DXVECTOR3 GetSize() const;	// �T�C�Y���擾
	virtual float GetRadius() const;		// ���a���擾
	virtual float GetHeight() const;		// �������擾

	// <static function>
	static CObject_X* Create(CX_Manager::TYPE Type);					// ����
	static CObject_X* Create(LAYER Priority, CX_Manager::TYPE Type);	// ����
	static CObject_X* Create(JSON Json);								// ����

private:

	/// <summary> �A���t�@�l�������p </summary>
	static constexpr float ALPHA_INIT = 1.0f;

	// <function>
	void AdjustRotAngle();	// �����𒲐�����
	void SetMtxWorld();		// ���[���h�s��ݒ�

	// <data>
	CX_Manager::MODEL*	m_pModel;	// ���f�����
	D3DXVECTOR3			m_Rot;		// ����
	D3DXVECTOR3			m_Pos;		// ���W
	D3DXVECTOR3			m_Scale;	// �k��
	D3DXCOLOR			m_Col;		// �F
	bool				m_bUseCol;	// �F���f
	D3DXMATRIX			m_MtxWorld;	// ���[���h�s��
};

#endif // _OBJECT_X_H_