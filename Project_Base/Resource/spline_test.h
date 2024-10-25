//============================================================================
//
// �X�v���C���Ȑ��H�̃e�X�g�A�w�b�_�t�@�C�� [spline_test.h]
// Author : ���c����
//
//============================================================================

#pragma once	// ��d�C���N���[�h�h�~

#include <nlohmann/json.hpp>

//************************************
// using�f�B���N�e�B�u
//************************************
using JSON = nlohmann::json;

// �Ă���������
class CSpline{
public:
	CSpline() :m_pVtxBuff{ nullptr }, m_pIdxBuff{ nullptr }, m_Pos{ 0.0f, 0.0f, 0.0f } {// ���[���h�s��̏�����
		D3DXMatrixIdentity(&m_MtxWorld); };
	~CSpline() { Uninit(); };
	void Init();
	void Uninit();
	void Update();
	void Draw();
private:
	HRESULT CreateVtxBuff();	// ���_�o�b�t�@�̐���
	HRESULT CreateIdxBuff();	// �C���f�b�N�X�o�b�t�@�̐�
	void SetMtxWorld();			// ���[���h�s��ݒ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// �C���f�b�N�X�o�b�t�@�̃|�C���^
	D3DXVECTOR3 m_Pos;	
	D3DXMATRIX m_MtxWorld;				// ��[��ǂ�
	JSON m_json;						// ��������
	int m_nNumVtx;
	int m_nNumIdx;
	int m_nNumPrim;
};