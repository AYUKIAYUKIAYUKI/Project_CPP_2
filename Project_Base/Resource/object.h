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

	/// <summary> �펯�I�ȃI�u�W�F�N�g�̍ő吔 </summary>
	static constexpr int MAX_OBJECT = 2048;

	//****************************************************
	// �I�u�W�F�N�g�̃��C���[����
	//****************************************************
	enum class LAYER : WORD
	{
		BG = 0,		// �w�i
		DEFAULT,	// �ʏ�
		FRONT,		// �O��
		STENCIL,	// �X�e���V��
		UI,			// UI
		MAX
	};

	//****************************************************
	// �I�u�W�F�N�g�̎�ގ���
	//****************************************************
	enum class TYPE : WORD
	{
		NONE = 0,	// ����
		PLAYER,		// �v���C���[
		BOSS,		// �{�X
		ENEMY,		// �G�l�~�[
		BLOCK,		// �u���b�N
		ITEM,		// �A�C�e��
		MAX
	};

	// <special function>
	CObject();									// �R���X�g���N�^
	CObject(LAYER Priority = LAYER::DEFAULT);	// �`��D��x�w��R���X�g���N�^
	virtual ~CObject() = 0;						// �f�X�g���N�^

	// <function>
	void SetRelease();	// ����\��ݒ�
	void Release();		// �ʉ������

	// <virtual function>
	virtual HRESULT	Init() = 0;		// �����ݒ�
	virtual void	Uninit() = 0;	// �I������
	virtual void	Update() = 0;	// �X�V����
	virtual void	Draw() = 0;		// �`�揈��

	// <getter/setter>
	const TYPE& GetType() const;	// �^�C�v���擾
	void SetType(TYPE Type);		// �^�C�v��ݒ�

	// <getter>
	CObject* GetPrev();	// �O�̃I�u�W�F�N�g�̃|�C���^���擾
	CObject* GetNext();	// ���̃I�u�W�F�N�g�̃|�C���^���擾

	// <static function>
	static void ReleaseAll();		// �S�I�u�W�F�N�g�������
	static void UpdateAll();		// �S�I�u�W�F�N�g�X�V����
	static void LateUpdateAll();	// �S�I�u�W�F�N�g��X�V����
	static void DrawAll();			// �S�I�u�W�F�N�g�`�揈��

	// <static getter>
	static CObject* GetTopObject();					// �擪�I�u�W�F�N�g�̃|�C���^�擾
	static CObject* GetTopObject(int nPriority);	// �擪�I�u�W�F�N�g�̃|�C���^�擾
	static CObject* GetTopObject(LAYER Priority);	// �擪�I�u�W�F�N�g�̃|�C���^�擾
	static CObject* FindSpecificObject(TYPE Type);	// ����^�C�v�̃I�u�W�F�N�g�T��
	static int		CountSpecificObject(TYPE Type);	// ����^�C�v�̃I�u�W�F�N�g�����擾

	// <>
	template <typename T> static T* DownCast(CObject* pBase);	// �I�u�W�F�N�g�h����p�_�E���L���X�g

private:

	// <data>
	WORD	 m_wPriority;	// �`��D��x
	CObject* m_pPrev;		// �O�̃I�u�W�F�N�g�̃|�C���^
	CObject* m_pNext;		// ���̃I�u�W�F�N�g�̃|�C���^
	TYPE	 m_Type;		// �^�C�v���� 
	bool	 m_bDeath;		// ���S�t���O

	// <static data>
	static int		m_nNumAll;								// �I�u�W�F�N�g����
	static CObject* m_pTop[static_cast<int>(LAYER::MAX)];	// �擪�I�u�W�F�N�g�̃|�C���^
	static CObject* m_pCur[static_cast<int>(LAYER::MAX)];	// �I�[�I�u�W�F�N�g�̃|�C���^
};

//============================================================================
// �I�u�W�F�N�g�h����p�_�E���L���X�g
//============================================================================
template <typename T> T* CObject::DownCast(CObject* pBase)
{
	// �n���ꂽ�I�u�W�F�N�g���`�F�b�N
	if (typeid(*pBase) != typeid(T))
	{
		return nullptr;
	}

	// �w�肳�ꂽ�N���X�Ƀ_�E���L���X�g
	T* pNew = dynamic_cast<T*>(pBase);

	// �_�E���L���X�g���s
	if (!pNew)
	{
		return nullptr;
	}
	
	return pNew;
}

#endif // _OBJECT_H_