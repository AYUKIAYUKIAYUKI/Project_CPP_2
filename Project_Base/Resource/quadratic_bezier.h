//============================================================================
//
// �񎟃x�W�F�Ȑ��A�w�b�_�t�@�C�� [quadratic_bezier.h]
// Author : ���c����
//
//============================================================================

#ifndef _QUADRATIC_BEZIER_
#define _QUADRATIC_BEZIER_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_X.h"

//****************************************************
// 2���x�W�F�Ȑ��N���X�̒�`
//****************************************************
class CQuadratic_Bezier
{
public:

	CQuadratic_Bezier(const D3DXVECTOR3& Pos1, const D3DXVECTOR3& Pos2, const D3DXVECTOR3& Pos3);	// ���W�n���R���X�g���N�^
	~CQuadratic_Bezier();																			// �f�X�g���N�^

	HRESULT Init();		// �����ݒ�
	void	Uninit();	// �I������
	void	Update();	// �X�V����
	void	Draw();		// �`�揈��

private:

	/// <summary> ����_�̐� </summary>
	static constexpr WORD NUM_CONTROLPOINT = 3;

	/// <summary> �����ڂ̐� </summary>
	static constexpr WORD NUM_VISUAL = 3;

	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;							// ���_�o�b�t�@�̃|�C���^
	D3DXVECTOR3				m_ControlPoint[NUM_CONTROLPOINT];	// ����_
	float					m_fParameter;						// �i�s�x
	CObject_X*				m_pParameterVisual[NUM_VISUAL];		// �i�s�x�̌�����
};

#endif	// _QUADRATIC_BEZIER_