//============================================================================
// 
// �t�B�[���h�}�l�[�W���[�A�w�b�_�t�@�C�� [field_manager.h]
// Author : ���c����
// 
//============================================================================

#ifndef _FIELD_MANAGER_H_
#define _FIELD_MANAGER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �O���錾
//****************************************************
class CObject_X;
class CMotion_Set;
class CPlayer;
class CFan;

//****************************************************
// �t�B�[���h�}�l�[�W���[�N���X
//****************************************************
class CField_Manager final
{
public:

	/// <summary> �t�B�[���h���a </summary>
	static constexpr float FIELD_RADIUS = 300.0f;

	/// <summary> �t�B�[���h�d�� </summary>
	static constexpr float FIELD_GRAVITY = -0.35f;

	/// <summary> �ő�j��u���b�N�� </summary>
	static constexpr int MAX_DESTROY_BLOCK = 50;

	// <special function>
	CField_Manager(const CField_Manager&) = delete;				// �R�s�[�R���X�g���N�^
	CField_Manager& operator=(const CField_Manager&) = delete;	// �R�s�[������Z�q
	CField_Manager(CField_Manager&&) = delete;					// ���[�u�R���X�g���N�^
	CField_Manager& operator=(CField_Manager&&) = delete;		// ���[�u������Z�q

	// <function>
	void InitForTitle();		// �^�C�g���ł̒ǉ������ݒ�
	void Update();				// �X�V����
	void Draw();				// �`�揈��
	void IncrementCntJump();	// �W�����v�����񐔂̃C���N�������g
	void IncrementCntDash();	// �_�b�V�������񐔂̃C���N�������g

	// <getter>
	int GetCntDestroyBlock();	// �u���b�N�̔j�󐔂��擾

	// <setter>
	void SetSyncPlayer(const CPlayer* const pPlayer);	// �v���C���[���Z�b�g

	// <static function>
	static HRESULT	Create();	// ����
	static void		Release();	// ���

	// <static getter>
	static CField_Manager* GetInstance();	// �t�B�[���h�}�l�[�W���[���擾

private:

	//****************************************************
	// �A�N�V�����f�[�^�\���̂̒�`
	//****************************************************
	struct ActionData
	{
		int nCntJump;	// �W�����v������
		int nCntDash;	// �_�b�V��������
	};

	//****************************************************
	// �t�B�[���h�^�C�v�̒�`
	//****************************************************
	enum class FIELD_TYPE : WORD
	{
		NORMAL, // ����
		JUMP,	// �W�����v
		DASH,	// �_�b�V��
		MAX
	};

	/// <summary> �ő哺���U���J�E���g </summary>
	static constexpr int MAX_CNT_STATUEVIBERATION = 300;

	// <special function>
	CField_Manager();	// �R���X�g���N�^
	~CField_Manager();	// �f�X�g���N�^

	// <function>
	HRESULT Init();								// �����ݒ�
	void	InitEnvironment();					// �������̏����ݒ�
	void	Uninit();							// �I������
	void	UpdateEnvironment();				// �������̍X�V����
	void	UpdateFan();						// ��`�̍X�V
	void	UpdateField();						// �t�B�[���h�X�V
	void	BranchFieldType();					// �t�B�[���h�^�C�v����
	void	AutoCreateItem();					// �A�C�e���̎�������
	void	AutoCreateBlockDash();				// �_�b�V���^�C�v�̃u���b�N��������
	bool	DetectNearBlock(D3DXVECTOR3 Pos);	// �אڂ������u���b�N�����o
	void	AutoDestroyBlock();					// �u���b�N�̎����폜
	void	DestroyAllBlock();					// �S�u���b�N�̍폜
	void	AppearBossEvent();					// �{�X�o��C�x���g
	void	NotifyTransition();					// �J�ڂ�ʒm����
	void	PrintDebug();						// �f�o�b�O�\��

	// <data>
	ActionData		m_ActionData;			// �A�N�V�����f�[�^
	FIELD_TYPE		m_FiledType;			// �t�B�[���h�^�C�v
	int				m_nCntDestroyBlock;		// �u���b�N�̔j��
	const CPlayer*	m_pSyncPlayer;			// �v���C���[�̃|�C���^
	CMotion_Set*	m_pStatue;				// ����
	int				m_nCntStatueVibration;	// �����U���J�E���g
	CFan*			m_pRenderFan;			// ��`�\��

	// <static data>
	static CField_Manager* m_pFieldManager;	// �t�B�[���h�}�l�[�W���[�̖{�� 
};

#endif // _FIELD_MANAGER_H_