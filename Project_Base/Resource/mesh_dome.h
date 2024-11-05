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

	/// <summary> �v���C���[�̍ő�̗� </summary>
	static constexpr int MAX_LIFE = 5;

	/// <summary> �v���C���[�̃f�t�H���g�ړ����x </summary>
	static constexpr float DEFAULT_MOVE_SPEED = 0.005f;

	/// <summary> �ڕW�l�ւ̕�ԋ��x </summary>
	static constexpr float COEF_ADJUST = 0.1f;

	CMesh_Dome();	// �f�t�H���g�R���X�g���N�^
	~CMesh_Dome();	// �f�X�g���N�^

	HRESULT Init() override;		// �����ݒ�
	void	Uninit() override;		// �I���ݒ�
	void	Update() override;		// �X�V����
	void	Draw() override;		// �`�揈��
};

#endif // _MESH_DOME_H_