//============================================================================
// 
// �{�b�N�X�\���A�w�b�_�t�@�C�� [render_box.h]
// Author : ���c����
// 
//============================================================================

#ifndef _RENDER_BOX_H_
#define _RENDER_BOX_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "render_collision.h"

//****************************************************
// ����\���N���X
//****************************************************
class CRender_Box : public CRender_Collision
{
public:

	// <special function>
	CRender_Box(LAYER Priority = LAYER::FRONT);	// �`��D��x�w��R���X�g���N�^
	~CRender_Box() override;					// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <static function>
	static CRender_Box* Create(CObject_X* pRef);	// ����

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

	// <data>
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;	// �C���f�b�N�X�o�b�t�@�̃|�C���^
};

#endif	// _RENDER_BOX_H_