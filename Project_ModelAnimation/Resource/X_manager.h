//============================================================================
// 
// X���f���}�l�[�W���[�A�w�b�_�t�@�C�� [X_manager.h]
// Author : ���c����
// 
//============================================================================

#ifndef _X_MANAGER_H_
#define _X_MANAGER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// X���f���}�l�[�W���[�N���X
//****************************************************
class CX_Manager final
{
public:

	//****************************************************
	// ���f������
	//****************************************************
	enum class TYPE : WORD
	{
		STATUE = 0,	// grid
		WAIST,		// ��
		BODY,		// ��
		HEAD,		// ��
		LEGL,		// ���r
		FOOTL,		// ����
		LEGR,		// �E�r
		FOOTR,		// �E��
		ARML,		// ���r
		HANDL,		// ����
		ARMR,		// �E�r
		HANDR,		// �E��
		TREE,
		STONE,
		BUTBODY,
		BUTLWING,
		BUTRWING,
		AROUND,		// ����
		DOWN0,		// ��0
		DOWN1,		// ��1
		DOWN2,		// ��2
		DOWN3,		// ��3
		UP0,		// ��0
		UP1,		// ��1
		UP2,		// ��2
		UP3,		// ��3
		UP5,		// ��4
		PUMPKIN,	// �p���v�L��
		SIDE0,
		SIDE1,
		FLOOR,
		GHOST,		// �S�[�X�g
		MONUP,		// �����X�^�[��
		MONDOWN,	// �����X�^�[��
		GRID,
		FLYBODY,	// �t���C���[��
		FLYLEFT,	// �t���C���[���H
		FLYRIGHT,	// �t���C���[�E�H
		SUMCIRCLE,	// ���@�w
		SUM0,		// ���[��0
		SUM1,		// ���[��1
		SUM2,		// ���[��2
		SUM3,		// ���[��3
		SUM4,		// ���[��4
		SUM5,		// ���[��5
		SUM6,		// ���[��6
		SUM7,		// ���[��7
		SUM8,		// ���[��8
		SUM9,		// ���[��9
		SUM10,		// ���[��10
		SUM11,		// ���[��11
		BRIGHT_0,	// �M��0
		BRIGHT_1,	// �M��1
		MAX
	};

	//****************************************************
	// ���f�����̍\���̂̒�`
	//****************************************************
	struct MODEL
	{
		LPD3DXMESH			pMesh;		// ���b�V�����
		D3DXVECTOR3			Size;		// �T�C�Y
		LPD3DXBUFFER		pBuffMat;	// �}�e���A�����
		DWORD				dwNumMat;	// �}�e���A����
		D3DXCOLOR*			apColMat;	// �}�e���A���F	
		LPDIRECT3DTEXTURE9*	apTex;		// �e�N�X�`��
	};

	// <special function>
	CX_Manager(const CX_Manager&) = delete;				// �R�s�[�R���X�g���N�^
	CX_Manager& operator=(const CX_Manager&) = delete;	// �R�s�[������Z�q
	CX_Manager(CX_Manager&&) = delete;					// ���[�u�R���X�g���N�^
	CX_Manager& operator=(CX_Manager&&) = delete;		// ���[�u������Z�q

	// <getter>
	MODEL* GetModel(TYPE Type);	// ���f�����擾

	// <static function>
	static HRESULT	Create();	// ����
	static void		Release();	// ���

	// <static getter>
	static CX_Manager* GetInstance();	// X���f���}�l�[�W���[���擾

private:

	// <special function>
	CX_Manager();	// �R���X�g���N�^
	~CX_Manager();	// �f�X�g���N�^

	// <function>
	HRESULT		Init();							// �����ݒ�
	D3DXVECTOR3 LoadSize(std::string FileName);	// �T�C�Y�ǂݍ���
	void		Uninit();						// �I������

	// <data>
	MODEL m_apModelTemp[static_cast<int>(TYPE::MAX)];	// X���f�����

	// <static data>
	static CX_Manager* m_pXModelManager;	// X���f���}�l�[�W���[�̖{��
};

#endif // _X_MANAGER_H_