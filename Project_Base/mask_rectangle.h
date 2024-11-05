//============================================================================
// 
// �l�p�`�}�X�N�A�w�b�_�t�@�C�� [mask_rectangle.h]
// Author : ���c����
// 
//============================================================================

#ifndef _MASK_RECTANGLE_H_
#define _MASK_RECTANGLE_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "texture_manager.h"

//****************************************************
// �l�p�`�}�X�N
//****************************************************
class CMask_Rectangle
{
public:

	CMask_Rectangle(CTexture_Manager::TYPE Type);	// �e�N�X�`���w��R���X�g���N�^
	~CMask_Rectangle();								// �f�X�g���N�^

	HRESULT	Init();		// �����ݒ�
	void	Uninit();	// �I������
	void	Update();	// �X�V����
	void	Draw();		// �`�揈��

	void BindTex(LPDIRECT3DTEXTURE9 pTex);		// �e�N�X�`������
	void BindTex(CTexture_Manager::TYPE Type);	// �����ƃe�N�X�`������

	static CMask_Rectangle* Create(CTexture_Manager::TYPE Type);	// ����

private:

	/// <summary> ���_�� </summary>
	static constexpr WORD NUM_VTX = 4;

	/// <summary> �v���~�e�B�u�� </summary>
	static constexpr WORD NUM_PRIM = 2;

	HRESULT CreateVtxBuff();	// ���_�o�b�t�@�̐���

	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;		// ���_�o�b�t�@�̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTex;			// �e�N�X�`���̃|�C���^
	D3DXVECTOR3				m_Pos;			// ���W
	D3DXVECTOR3				m_Rot;			// ����
	D3DXVECTOR3				m_Size;			// �T�C�Y
	D3DXCOLOR				m_Col;			// �F
	float					m_fLength;		// �W�J�p�Ίp��
	float					m_fAngle;		// �Ίp���p�p�x
};

#endif // _MASK_RECTANGLE_H_