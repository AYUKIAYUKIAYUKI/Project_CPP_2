//============================================================================
// 
// �����A�w�b�_�t�@�C�� [constellation.h]
// Author : ���c����
// 
//============================================================================

#ifndef _CONSTELLATION_H_
#define _CONSTELLATION_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_Effect.h"

//****************************************************
// �����I�u�W�F�N�g�N���X
//****************************************************
class CConstellation : public CObject_Effect
{
public:

	// <special function>
	CConstellation();			// �R���X�g���N�^
	~CConstellation() override;	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <static function>
	static void GenerateSpread(D3DXVECTOR3 Pos);	// �g�U����

private:

	/// <summary> �g�U�����X�p�� </summary>
	static constexpr WORD SPREAD_SPAN = 10;

	// <static fuction>
	static void Create(D3DXVECTOR3 Pos);	// ����

	// <data>
	D3DXVECTOR3	m_InitPos;	// �������W

	// <static data>
	static WORD m_nCntGenerateSpan;	// �����X�p���̃J�E���g
	static JSON m_InitParam;		// ��b�p�����[�^
};

#endif	// _CONSTELLATION_H_