//============================================================================
// 
// �M���A�w�b�_�t�@�C�� [bright.h]
// Author : ���c����
// 
//============================================================================

#ifndef _BRIGHT_H_
#define _BRIGHT_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "motion_set.h"

//****************************************************
// �M���N���X
//****************************************************
class CBright : public CMotion_Set
{
public:

	// <special fuction>
	CBright();	// �R���X�g���N�^
	~CBright();	// �f�X�g���N�^

	// <function fuction>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <static function>
	static void Generate(D3DXVECTOR3 Pos);	// ����

private:

	// <static function>
	static void Create(D3DXVECTOR3 Pos);	// ����

	// <static data>
	static WORD m_nCntGenerateSpan;	// �����X�p���J�E���g
	static JSON m_InitParam;		// ��b�p�����[�^
};

#endif	// _BRIGHT_H_