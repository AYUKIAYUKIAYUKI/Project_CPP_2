//============================================================================
// 
// 3D�I�u�W�F�N�g�Ǘ��A�w�b�_�t�@�C�� [object_3D.h]
// Author : ���c����
// 
//============================================================================

#ifndef _OBJECT_3D_H_
#define _OBJECT_3D_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.h"

// �e�N�X�`�����蓖�ėp
#include "texture_manager.h"

//****************************************************
// 3D�I�u�W�F�N�g�N���X
//****************************************************
class CObject_3D : public CObject
{
public:

	CObject_3D(int nPriority = static_cast<int>(LAYER::NONE));	// �`��D��x�w��R���X�g���N�^
	~CObject_3D() override;										// �f�X�g���N�^

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

	static CObject_3D* Create();	// ����

private:

	void SetMtxWorld();	// ���[���h�s��ݒ�

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTex;		// �e�N�X�`���̃|�C���^

	D3DXVECTOR3	m_Pos;		// ���W
	D3DXVECTOR3	m_Rot;		// ����
	D3DXVECTOR3	m_Size;		// �T�C�Y
	D3DXCOLOR	m_Col;		// �F
	float		m_fLength;	// �W�J�p�Ίp��
	float		m_fAngle;	// �Ίp���p�p�x
	D3DXMATRIX	m_MtxWorld;	// ���[���h�s��
};

#endif // _OBJECT_3D_H_