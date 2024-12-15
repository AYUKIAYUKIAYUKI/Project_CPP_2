//============================================================================
// 
// 3D�I�u�W�F�N�g�A�w�b�_�t�@�C�� [object_3D.h]
// Author : ���c����
// 
//============================================================================

#ifndef _OBJECT_3D_H_
#define _OBJECT_3D_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.h"
#include "texture_manager.h"

//****************************************************
// 3D�I�u�W�F�N�g�N���X
//****************************************************
class CObject_3D : public CObject
{
public:

	// <special function>
	CObject_3D(LAYER Priority = LAYER::BG);	// �`��D��x�w��R���X�g���N�^
	~CObject_3D() override;					// �f�X�g���N�^

	// <function>
	HRESULT	Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <getter>
	LPDIRECT3DSURFACE9 GetSurface();	// �T�[�t�F�C�X���擾

	// <setter>
	//void BindTex(LPDIRECT3DTEXTURE9 pTex);		// �e�N�X�`������
	//void BindTex(CTexture_Manager::TYPE Type);	// �����ƃe�N�X�`������

	// <getter/setter>
	const D3DXVECTOR3& GetSize() const;	// �T�C�Y�擾
	void SetSize(D3DXVECTOR3 Size);		// �T�C�Y�ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetRot() const;	// �����擾
	void SetRot(D3DXVECTOR3 Rot);		// �����ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetPos() const;	// ���W�擾
	void SetPos(D3DXVECTOR3 Pos);		// ���W�ݒ�

	// <getter/setter>
	const D3DXCOLOR& GetCol() const;	// �F�擾
	void SetCol(D3DXCOLOR Col);			// �F�ݒ�

	// <getter/setter>
	const float& GetAlpha() const;	// �A���t�@�l�擾
	void SetAlpha(float fAlpha);	// �A���t�@�l�ݒ�

	// <getter>
	const float& GetLength() const;	// �W�J�p�Ίp���擾

	// <static function>
	static CObject_3D* Create();	// ����

private:

	/// <summary> ���_�� </summary>
	static constexpr WORD NUM_VTX = 4;

	/// <summary> �v���~�e�B�u�� </summary>
	static constexpr WORD NUM_PRIM = 2;

	// <function>
	HRESULT CreateVtxBuff();	// ���_�o�b�t�@�̐���
	void	SetMtxWorld();		// ���[���h�s��ݒ�

	// <data>
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTex;			// �e�N�X�`���̃|�C���^
	LPDIRECT3DSURFACE9		m_pSurface;		// �T�[�t�F�C�X�̃|�C���^
	D3DXVECTOR3				m_Size;			// �T�C�Y
	D3DXVECTOR3				m_Rot;			// ����
	D3DXVECTOR3				m_Pos;			// ���W
	D3DXCOLOR				m_Col;			// �F
	float					m_fLength;		// �W�J�p�Ίp��
	float					m_fAngle;		// �Ίp���p�p�x
	D3DXMATRIX				m_MtxWorld;		// ���[���h�s��
};

#endif // _OBJECT_3D_H_