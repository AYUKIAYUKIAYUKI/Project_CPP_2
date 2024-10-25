//============================================================================
//
// �X�v���C���Ȑ��̃e�X�g�A�w�b�_�t�@�C�� [spline_test.h]
// Author : ���c����
//
//============================================================================

#ifndef _SPLINE_TEST_
#define _SPLINE_TEST_	// ��d�C���N���[�h�h�~

//****************************************************
// �X�v���C���e�X�g�N���X
//****************************************************
class CSpline_Test
{
public:

	CSpline_Test() :m_pVtxBuff{ nullptr }, m_pIdxBuff{ nullptr }, m_Pos{ 0.0f, 0.0f, 0.0f } {// ���[���h�s��̏�����
		D3DXMatrixIdentity(&m_MtxWorld); };	// �f�t�H���g�R���X�g���N�^
	~CSpline_Test() { Uninit(); };	// �f�X�g���N�^

	HRESULT Init();		// �����ݒ�
	void	Uninit();	// �I������
	void	Update();	// �X�V����
	void	Draw();		// �`�揈��

private:

	HRESULT CreateVtxBuff();	// ���_�o�b�t�@�̐���
	HRESULT CreateIdxBuff();	// �C���f�b�N�X�o�b�t�@�̐���
	void	SetMtxWorld();		// ���[���h�s��ݒ�

	JSON					m_Json;		// �W�F�C�\���I�u�W�F�N�g
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;	// ���_�o�b�t�@�̃|�C���^
	int						m_nNumVtx;	// ���_��
	int						m_nNumPrim;	// �v���~�e�B�u��
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;	// �C���f�b�N�X�o�b�t�@�̃|�C���^
	int						m_nNumIdx;	// �C���f�b�N�X��
	D3DXVECTOR3				m_Pos;		// ���W
	D3DXMATRIX				m_MtxWorld;	// ���[���h�s��
};

#endif	// _SPLINE_TEST_