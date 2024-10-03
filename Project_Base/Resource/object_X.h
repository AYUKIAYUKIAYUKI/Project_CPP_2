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

	CObject_X(int nPriority = static_cast<int>(LAYER::NONE));	// �R���X�g���N�^
	~CObject_X() override;										// �f�X�g���N�^

	HRESULT Init() override;	// �����ݒ�
	void Uninit() override;		// �I������
	void Update() override;		// �X�V����
	void Draw() override;		// �`�揈��

	void BindModel(CModel_X_Manager::MODEL* pModel);	// ���f������
	void BindModel(CModel_X_Manager::TYPE Type);		// �����ƃ��f������

	D3DXVECTOR3 GetPos();			// ���W�擾
	void SetPos(D3DXVECTOR3 Pos);	// ���W�ݒ�

	D3DXVECTOR3& GetRot();			// �����擾
	void SetRot(D3DXVECTOR3 Rot);	// �����ݒ�

	D3DXVECTOR3 GetSize();			// �T�C�Y�擾
	void SetSize(D3DXVECTOR3 Size);	// �T�C�Y�ݒ�

	float& GetScale() { return m_fScale; }				// �k�ڎ擾
	void SetScale(float fScale) { m_fScale = fScale; }	// �k�ڐݒ�

	float& GetAlpha();				// �A���t�@�l�擾
	void SetAlpha(float fAlpha);	// �A���t�@�l�ݒ�

	static CObject_X* Create();	// ����

private:

	static const float DEFAULT_SCALE_VALUE;	// �f�t�H���g�X�P�[���l
	static const float DEFAULT_ALPHA_VALUE;	// �f�t�H���g�A���t�@�l

	void SetMtxWorld();	// ���[���h�s��ݒ�

	CModel_X_Manager::MODEL* m_pModel;	// ���f�����
	D3DXVECTOR3 m_Pos;					// �ʒu
	D3DXVECTOR3 m_Rot;					// ����
	D3DXVECTOR3 m_Size;					// �T�C�Y
	float m_fScale;						// �k��
	float m_fAlpha;						// �A���t�@�l
	D3DXMATRIX m_MtxWorld;				// ���[���h�s��
};

#endif // _OBJECT_X_H_