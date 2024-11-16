//============================================================================
// 
// �΂̕��A�w�b�_�t�@�C�� [sparks.h]
// Author : ���c����
// 
//============================================================================

#ifndef _SPARKS_H_
#define _SPARKS_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_Effect.h"

//****************************************************
// �΂̕��I�u�W�F�N�g�N���X
//****************************************************
class CSparks : public CObject_Effect
{
public:

	// <special function>
	CSparks();				// �R���X�g���N�^
	~CSparks() override;	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <static function>
	static void AutoGenerate();	// ��������

private:

	/// <summary> �����X�p�� </summary>
	static constexpr WORD NUM_GENERATE_SPAN = 30;

	// <static fuction>
	static void Create();	// ����

	// <data>
	D3DXVECTOR3	m_InitPos;		// �������W
	float		m_fWaveCoef;	// �E�F�[�u���x
	float		m_fAdderY;		// �㏸��

	// <static data>
	static WORD m_nCntGenerateSpan;	// �����X�p���̃J�E���g
};

#endif	// _SPARKS_H_