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

//****************************************************
// �O���錾
//****************************************************
class CObject_X;

//****************************************************
// ����\���N���X
//****************************************************
class CRender_Collision : public CObject
{
public:

	// <special function>
	CRender_Collision(LAYER Priority = LAYER::FRONT);	// �`��D��x�w��R���X�g���N�^
	~CRender_Collision() override;						// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <setter>
	void SetRefObj(CObject_X* pRef);	// �ΏۃI�u�W�F�N�g�̐ݒ�

	// <static function>
	static CRender_Collision* Create(CObject_X* pRef);	// ����

private:

	/// <summary> ���_�� </summary>
	static constexpr WORD NUM_VTX = 8;
	
	/// <summary> �v���~�e�B�u�� </summary>
	static constexpr WORD NUM_PRIM = 12;
	
	/// <summary> �C���f�b�N�X�� </summary>
	static constexpr WORD NUM_IDX = 24;

	// <function>
	HRESULT CreateVtxBuff();	// ���_�o�b�t�@�̐���
	HRESULT CreateIdxBuff();	// �C���f�b�N�X�o�b�t�@�̐���
	void	SetVtx();			// ���_�̐ݒ�
	void	SetMtxWorld();		// ���[���h�s��ݒ�

	// <data>
	CObject_X*				m_pRef;		// �ΏۃI�u�W�F�N�g
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;	// �C���f�b�N�X�o�b�t�@�̃|�C���^
	D3DXCOLOR				m_Col;		// �F
	D3DXMATRIX				m_MtxWorld;	// ���[���h�s��
};

#endif	// _RENDER_COLLISION_H_