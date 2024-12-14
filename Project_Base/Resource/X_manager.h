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
		RENDER_SPHERE = 0,	// render sphere
		RENDER_CYLINDER,	// render cylinder
		RENDER_BOX,			// render box
		BUTBODY,			// ����
		BUTLWING,			// �����H
		BUTRWING,			// ���E�H
		PLAWAIST,			// �l��
		PLABODY,			// �l��
		PLAHEAD,			// �l��
		PLALEGL,			// �l���r
		PLAFOOTL,			// �l����
		PLALEGR,			// �l�E�r
		PLAFOOTR,			// �l�E��
		PLAARML,			// �l���r
		PLAHANDL,			// �l����
		PLAARMR,			// �l�E�r
		PLAHANDR,			// �l�E��
		ONLYTREE,			// ���̂�
		ONLYSTONE,			// ���̐�
		STAAROUND,			// ��������
		STADOWN0,			// ������0
		STADOWN1,			// ������1
		STADOWN2,			// ������2
		STADOWN3,			// ������3
		STAUP0,				// ������0
		STAUP1,				// ������1
		STAUP2,				// ������2
		STAUP3,				// ������3
		STAUP4,				// ������4
		AROUND,				// ���ӗ̈�
		PUMPKIN,			// �p���v�L��
		GHOST,				// �S�[�X�g
		MONUP,				// �����X�^�[��
		MONDOWN,			// �����X�^�[��
		FLYBODY,			// �t���C���[��
		FLYLEFT,			// �t���C���[���H
		FLYRIGHT,			// �t���C���[�E�H
		BLONORMAL,			// �ʏ�u���b�N
		BLOSIDE,			// �����u���b�N
		BLOTALL,			// �c���u���b�N
		LIFE,				// ���C�t
		SUMCIRCLE,			// ���@�w
		SUM0,				// ���[��0
		SUM1,				// ���[��1
		SUM2,				// ���[��2
		SUM3,				// ���[��3
		SUM4,				// ���[��4
		SUM5,				// ���[��5
		SUM6,				// ���[��6
		SUM7,				// ���[��7
		SUM8,				// ���[��8
		SUM9,				// ���[��9
		SUM10,				// ���[��10
		SUM11,				// ���[��11
		MAX
	};

	// +���Z�q�̃I�[�o�[���[�h
	friend TYPE operator+(TYPE Type, WORD wNum)
	{
		return static_cast<TYPE>(static_cast<WORD>(Type) + wNum);
	}

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
		TYPE				Type;		// �^�C�v
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