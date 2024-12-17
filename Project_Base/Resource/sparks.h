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
	static void AutoGenerate();					// ��������
	static void AreaGenerate(D3DXVECTOR3 Pos);	// �Ǐ�����
	static void FuryGenerate();					// �Ҕ���

private:

	// <static fuction>
	static void AutoCreate();					// ��������
	static void AreaCreate(D3DXVECTOR3 Pos);	// �Ǐ�����
	static void FuryCreate();					// �Ґ���

	// <data>
	D3DXVECTOR3	m_InitPos;		// �������W
	float		m_fWaveCoef;	// �E�F�[�u���x
	float		m_fAdderY;		// �㏸��

	// <static data>
	static JSON m_InitParam;				// ��b�p�����[�^
	static int  m_nCntGenerate;				// �����J�E���g
	static const int m_nAutoGenerateSpan;	// ���������X�p��
	static const int m_nAreaGenerateSpan;	// �Ǐ������X�p��
	static const int m_nFuryGenerateSpan;	// �Ҕ����X�p��
};

#endif	// _SPARKS_H_