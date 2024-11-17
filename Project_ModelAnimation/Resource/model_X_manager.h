//============================================================================
// 
// X���f���}�l�[�W���[�A�w�b�_�t�@�C�� [model_X_manager.h]
// Author : ���c����
// 
//============================================================================

#ifndef _MODEL_X_MANAGER_H_
#define _MODEL_X_MANAGER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// X���f���}�l�[�W���[�N���X
//****************************************************
class CX_Manager final
{
public:

	//****************************************************
	// ���f������
	//****************************************************
	enum class TYPE
	{
		TEST = 0,				// �e�X�g
		TEST2,					// �e�X�g2
		CYLINDER,				// cylinder
		SAMUS,					// �T���X
		ENEMY,					// �G
		RUBBLE_00,				// ���ꂫ00
		RUBBLE_01,				// ���ꂫ01
		RUBBLE_02,				// ���ꂫ02
		CYLINDERCOLLIDER,		// cylinder collider
		SPHERE,					// sphere
		RENDER_SPHERE,			// render sphere
		RENDER_SPHERE_HIT,		// render sphere hit
		RENDER_CYLINDER,		// render cylinder
		RENDER_CYLINDER_HIT,	// render cylinder hit
		STATUE,					// ����
		AROUND,					// ���ӗ̈�
		MAX,
	};

	//****************************************************
	// ���f�����̍\���̂̒�`
	//****************************************************
	struct MODEL
	{
		D3DXVECTOR3 Size;			// �T�C�Y
		LPD3DXMESH pMesh;			// ���b�V�����
		LPD3DXBUFFER pBuffMat;		// �}�e���A�����
		DWORD dwNumMat;				// �}�e���A���̐�
		LPDIRECT3DTEXTURE9* apTex;	// �e�N�X�`�����
	};

	// <special function>
	CX_Manager(const CX_Manager&) = delete;				// �R�s�[�R���X�g���N�^
	CX_Manager& operator=(const CX_Manager&) = delete;	// �R�s�[������Z�q
	CX_Manager(CX_Manager&&) = delete;					// ���[�u�R���X�g���N�^
	CX_Manager& operator=(CX_Manager&&) = delete;			// ���[�u������Z�q

	// <getter>
	MODEL* GetModel(TYPE Type);	// ���f�����擾

	// <static function>
	static HRESULT Create();	// ����
	static void	Release();		// ���

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

#endif // _MODEL_X_MANAGER_H_