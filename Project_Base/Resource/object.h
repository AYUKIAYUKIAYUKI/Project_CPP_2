//============================================================================
// 
// �I�u�W�F�N�g�Ǘ��A�w�b�_�t�@�C�� [object.h]
// Author : ���c����
// 
//============================================================================

#ifndef _OBJECT_H_
#define _OBJECT_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �I�u�W�F�N�g�N���X
//****************************************************
class CObject
{
public:

	static const int MAX_OBJ = 2048;	// �펯�I�ȃI�u�W�F�N�g�̍ő吔

	//****************************************************
	// �I�u�W�F�N�g�̎�ގ���
	//****************************************************
	enum class TYPE
	{
		NONE = 0,		// ����
		MAX,
	};

	//****************************************************
	// �I�u�W�F�N�g�̃��C���[����
	//****************************************************
	enum class LAYER
	{
		NONE = 0,	// ����
		BG,			// �w�i
		BACK,		// ���
		MIDDLE,		// ����
		FRONT,		// �O��
		UI,			// UI
		MAX,
	};

	CObject();												// �f�t�H���g�R���X�g���N�^
	CObject(int nPriority = static_cast<int>(LAYER::NONE));	// �v���C�I���e�B�w��R���X�g���N�^
	virtual ~CObject() = 0;									// �f�X�g���N�^

	virtual HRESULT Init() = 0;	// �����ݒ�
	virtual void Uninit() = 0;	// �I������
	virtual void Update() = 0;	// �X�V����
	virtual void Draw() = 0;	// �`�揈��
	void SetRelease();			// ����\��ݒ�
	void Release();				// �ʉ������

	TYPE GetType();				// �^�C�v���擾
	void SetType(TYPE type);	// �^�C�v��ݒ�

	CObject* GetPrev();	// �O�̃I�u�W�F�N�g�̃|�C���^���擾
	CObject* GetNext();	// ���̃I�u�W�F�N�g�̃|�C���^���擾

	static void ReleaseAll();		// �S�I�u�W�F�N�g�������
	static void UpdateAll();		// �S�I�u�W�F�N�g�X�V����
	static void LateUpdateAll();	// �S�I�u�W�F�N�g��X�V����
	static void DrawAll();			// �S�I�u�W�F�N�g�`�揈��

	static CObject* GetObject(int nPriority);	// �擪�I�u�W�F�N�g�̃|�C���^�擾
	static CObject* FindObject(TYPE type);		// ����^�C�v�̃I�u�W�F�N�g�T��

private:

	static int m_nNumAll;										// �I�u�W�F�N�g����
	static CObject* m_pTop[static_cast<int>(LAYER::MAX)];		// �擪�I�u�W�F�N�g�̃|�C���^
	static CObject* m_pCur[static_cast<int>(LAYER::MAX)];		// �I�[�I�u�W�F�N�g�̃|�C���^

	int m_nPriority;	// �`��D��x
	CObject* m_pPrev;	// �O�̃I�u�W�F�N�g�̃|�C���^
	CObject* m_pNext;	// ���̃I�u�W�F�N�g�̃|�C���^
	TYPE m_type;		// �^�C�v���� 
	bool m_bDeath;		// ���S�t���O
};

#endif // _OBJECT_H_