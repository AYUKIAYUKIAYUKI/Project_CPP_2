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

	/// <summary> ���������X�p�� </summary>
	static constexpr WORD AUTOGENERATE_SPAN = 30;

	/// <summary> �Ǐ������X�p�� </summary>
	static constexpr WORD AREAGENERATE_SPAN = 20;

	/// <summary> �Ҕ����X�p�� </summary>
	static constexpr WORD FURYGENERATE_SPAN = 10;

	// <static fuction>
	static void AutoCreate();					// ��������
	static void AreaCreate(D3DXVECTOR3 Pos);	// �Ǐ�����
	static void FuryCreate();					// �Ҕ���

	// <data>
	D3DXVECTOR3	m_InitPos;		// �������W
	float		m_fWaveCoef;	// �E�F�[�u���x
	float		m_fAdderY;		// �㏸��

	// <static data>
	static WORD m_nCntGenerateSpan;	// �����X�p���̃J�E���g
	static JSON m_InitParam;		// ��b�p�����[�^
};

#endif	// _SPARKS_H_