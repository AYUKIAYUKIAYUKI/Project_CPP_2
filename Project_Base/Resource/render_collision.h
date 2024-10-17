//============================================================================
// 
// ����\���A�w�b�_�t�@�C�� [render_collision.h]
// Author : ���c����
// 
//============================================================================

#ifndef _RENDER_COLLISION_H_
#define _RENDER_COLLISION_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.h"

#include "object_X.h"

//****************************************************
// ����\���N���X
//****************************************************
class CRender_Collision : public CObject
{
public:

	CRender_Collision(CObject_X* pObj, D3DXVECTOR3& m_posRef, D3DXVECTOR3& sizeRef, int nPriority = static_cast<int>(LAYER::FRONT));	// �R���X�g���N�^
	~CRender_Collision() override;	// �f�X�g���N�^

	HRESULT Init() override;	// �����ݒ�
	void Uninit() override;		// �I������
	void Update() override;		// �X�V����
	void Draw() override;		// �`�揈��

	// ���^�̍쐬
	static CRender_Collision* Create(CObject_X* pObj, D3DXVECTOR3& posRef, D3DXVECTOR3& sizeRef);	// ����

private:

	// ���^�݂̂̒萔
	static const int m_nNumVtx;			// ���_��
	static const int m_nNumPrimitive;	// �Ӑ�
	static const int m_nNumIdx;			// �C���f�b�N�X��

	HRESULT CreateVtxBuff();	// ���_�o�b�t�@�̐���
	HRESULT CreateIdxBuff();	// �C���f�b�N�X�o�b�t�@�̐���
	void SetVtx();				// ���_�̐ݒ�
	void SetMtxWorld();			// ���[���h�s��ݒ�

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// �C���f�b�N�X�o�b�t�@�̃|�C���^
	CObject* m_pRefPtr;					// �Q�Ɛ�
	D3DXVECTOR3& m_posRef;				// �Q�ƈʒu
	D3DXCOLOR m_col;					// �F
	D3DXMATRIX m_mtxWorld;				// ���[���h�s��

	// ���^�݂̂̃����o
	D3DXVECTOR3& m_size;	// �T�C�Y
};

#endif	// _RENDER_COLLISION_H_