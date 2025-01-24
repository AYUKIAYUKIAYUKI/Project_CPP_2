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
class CField_Builder;
class CObject;
class CObject_X;
class CObject_PopUp;
class CMotion_Set;
class CPlayer;

//****************************************************
// �t�B�[���h�}�l�[�W���[�N���X
//****************************************************
class CField_Manager final
{
public:

	/// <summary> �t�B�[���h���a </summary>
	static constexpr float FIELD_RADIUS = 300.0f;

	/// <summary> �t�B�[���h�d�� </summary>
	static constexpr float FIELD_GRAVITY = -0.15f;

	/// <summary> �ő�j��u���b�N�� </summary>
	static constexpr int MAX_DESTROY_BLOCK = 300;

	// <special function>
	CField_Manager(const CField_Manager&) = delete;				// �R�s�[�R���X�g���N�^
	CField_Manager& operator=(const CField_Manager&) = delete;	// �R�s�[������Z�q
	CField_Manager(CField_Manager&&) = delete;					// ���[�u�R���X�g���N�^
	CField_Manager& operator=(CField_Manager&&) = delete;		// ���[�u������Z�q

	// <function>
	void InitForTitle();						// �^�C�g�������ǉ������ݒ�
	void InitForGame();							// �Q�[�������ǉ������ݒ�
	void Update();								// �X�V����
	void Draw();								// �`�揈��
	bool AdjustPosToFieldSize(CObject* pObj);	// ���W���t�B�[���h�T�C�Y�ɒ���
	void AAABBB();	// ������

	// <getter>
	CField_Builder* const GetFieldBuilder() const;	// �t�B�[���h�r���_�[�̎擾

	// <setter>
	void SetSyncPlayer(const CPlayer* const pPlayer);	// �v���C���[���Z�b�g

	// <static function>
	static HRESULT	Create();	// ����
	static void		Release();	// ���

	// <static getter>
	static CField_Manager* GetInstance();	// �t�B�[���h�}�l�[�W���[���擾

private:

	/// <summary> �ő哺���U���J�E���g </summary>
	static constexpr int MAX_CNT_STATUEVIBERATION = 300;

	// <special function>
	CField_Manager();	// �R���X�g���N�^
	~CField_Manager();	// �f�X�g���N�^

	// <function>
	HRESULT Init();						// �����ݒ�
	void	InitEnvironment();			// �������̏����ݒ�
	void	InitBlockSet();				// �����u���b�N�̃Z�b�g
	void	Uninit();					// �I������
	void	UpdatePhase();				// �t�F�[�Y�̍X�V
	void	UpdateEnvironment();		// �������̍X�V����
	void	AppearBossEvent();			// �{�X�o��C�x���g
	void	NotifySceneTransition();	// �V�[���J�ڂ�ʒm����
	void	PrintDebug();				// �f�o�b�O�\��

	// <data>
	CField_Builder* m_pFieldBuilder;		// �t�B�[���h�r���_�[
	int				m_nPhase;				// �t�F�[�Y�ԍ�
	CObject_PopUp*	m_pPopUp;				// �|�b�v�A�b�v�\��
	CMotion_Set*	m_pStatue;				// ����
	int				m_nCntStatueVibration;	// �����U���J�E���g
	const CPlayer*	m_pSyncPlayer;			// �v���C���[�̃|�C���^

	// <static data>
	static CField_Manager* m_pFieldManager;	// �t�B�[���h�}�l�[�W���[�̖{��
	static bool			   m_bRetry;		// ���񔻒�
};

#endif // _FIELD_MANAGER_H_