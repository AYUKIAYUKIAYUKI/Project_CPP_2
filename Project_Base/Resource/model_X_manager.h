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
class CModel_X_Manager final
{
public:

	//****************************************************
	// ���f������
	//****************************************************
	enum class TYPE
	{
		TEST = 0,	// �e�X�g
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

	HRESULT Load();	// ���f���Ǎ�
	void Release();	// ���

	MODEL* GetModel(TYPE Type);	// ���f�����擾

	static CModel_X_Manager* GetInstance();	// X���f���}�l�[�W���[���擾

private:

	CModel_X_Manager();		// �f�t�H���g�R���X�g���N�^
	~CModel_X_Manager();	// �f�X�g���N�^

	void Create();									// ����
	D3DXVECTOR3 ImportSize(std::string Filename);	// �T�C�Y�ǂݍ���
	void Unload();									// ���f���j��

	MODEL m_apModelTemp[static_cast<int>(TYPE::MAX)];	// X���f�����

	static CModel_X_Manager* m_pInstnce;	// X���f���}�l�[�W���[
};

#endif // _MODEL_X_MANAGER_H_