//============================================================================
// 
// 2D�I�u�W�F�N�g�A�w�b�_�t�@�C�� [object_2D.h]
// Author : ���c����
// 
//============================================================================

#ifndef _OBJECT_2D_H_
#define _OBJECT_2D_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.h"

// �e�N�X�`�����蓖�ėp
#include "texture_manager.h"

//****************************************************
// 2D�I�u�W�F�N�g�N���X
//****************************************************
class CObject_2D : public CObject
{
public:

	CObject_2D(LAYER Priority = LAYER::UI);		// �`��D��x�w��R���X�g���N�^
	CObject_2D(CTexture_Manager::TYPE Type);	// �e�N�X�`���w��R���X�g���N�^
	~CObject_2D() override;						// �f�X�g���N�^

	HRESULT	Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	void BindTex(LPDIRECT3DTEXTURE9 pTex);		// �e�N�X�`������
	void BindTex(CTexture_Manager::TYPE Type);	// �����ƃe�N�X�`������

	const D3DXVECTOR3& GetPos() const;	// ���W�擾
	void SetPos(D3DXVECTOR3 Pos);		// ���W�ݒ�

	const D3DXVECTOR3& GetRot() const;	// �����擾
	void SetRot(D3DXVECTOR3 Rot);		// �����ݒ�

	const D3DXVECTOR3& GetSize() const;	// �T�C�Y�擾
	void SetSize(D3DXVECTOR3 Size);		// �T�C�Y�ݒ�

	const D3DXCOLOR& GetCol() const;	// �F�擾
	void SetCol(D3DXCOLOR Col);			// �F�ݒ�

	const float& GetAlpha() const;	// �A���t�@�l�擾
	void SetAlpha(float fAlpha);	// �A���t�@�l�ݒ�

	const float& GetLength() const;	// �W�J�p�Ίp���擾

	void SetTexWidth(float fWidth);		// ���e�N�X�`���������ݒ�
	void SetTexHeight(float fHeight);	// �c�e�N�X�`���������ݒ�

	const int& GetNowPatternU() const;		// ���݂̃e�N�X�`�����������擾
	void SetNowPatternU(int nNowPatternU);	// ���݂̃e�N�X�`�����������ݒ�

	const int& GetNowPatternV() const;		// ���݂̃e�N�X�`���c�������擾
	void SetNowPatternV(int nNowPatternV);	// ���݂̃e�N�X�`���c�������ݒ�

	static CObject_2D* Create();	// ����

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
	float					m_fTexWidth;	// ���e�N�X�`��������
	float					m_fTexHeight;	// �c�e�N�X�`�����c��
	int						m_nNowPatternU;	// ���݂̉��e�N�X�`�����
	int						m_nNowPatternV;	// ���݂̏c�e�N�X�`�����
};

#endif // _OBJECT_2D_H_