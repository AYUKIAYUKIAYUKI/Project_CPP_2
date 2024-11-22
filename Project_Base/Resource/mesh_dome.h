//============================================================================
//
// ���b�V���h�[���A�w�b�_�t�@�C�� [mesh_dome.h]
// Author : ���c����
//
//============================================================================

#ifndef _MESH_DOME_H_
#define _MESH_DOME_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.h"

//****************************************************
// ���b�V���h�[���N���X
//****************************************************
class CMesh_Dome : public CObject
{
public:

	// <special funvtion>
	CMesh_Dome();	// �R���X�g���N�^
	~CMesh_Dome();	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;		// �����ݒ�
	void	Uninit() override;		// �I���ݒ�
	void	Update() override;		// �X�V����
	void	Draw() override;		// �`�揈��

	// <static function>
	static CMesh_Dome* Create();	// ����

private:

	// <function>
	HRESULT CreateVtxBuff();	// ���_�o�b�t�@�̐���
	HRESULT CreateIdxBuff();	// �C���f�b�N�X�o�b�t�@�̐���
	void	SetVtx();			// ���_�̐ݒ�
	void	SetMtxWorld();		// ���[���h�s��̐ݒ�

	// <data>
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;	// �C���f�b�N�X�o�b�t�@�̃|�C���^
	D3DXVECTOR3				m_Pos;		// ���W
	D3DXMATRIX				m_MtxWorld;	// ���[���h�s��
};

#endif // _MESH_DOME_H_