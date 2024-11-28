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
class CObject_HUD;
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

	/// <summary> �ő�u���b�N�� </summary>
	static constexpr int MAX_BLOCK = 1;

	/// <summary> �d�� </summary>
	static constexpr float FIELD_GRAVITY = -0.5f;

	// <special function>
	CField_Manager(const CField_Manager&) = delete;				// �R�s�[�R���X�g���N�^
	CField_Manager& operator=(const CField_Manager&) = delete;	// �R�s�[������Z�q
	CField_Manager(CField_Manager&&) = delete;					// ���[�u�R���X�g���N�^
	CField_Manager& operator=(CField_Manager&&) = delete;		// ���[�u������Z�q

	// <function>
	void Update();		// �X�V����
	void Draw();		// �`�揈��
	void AppearBoss();	// �{�X�o��

	// <setter>
	/* ���@���l���Ă�������*/
	void SetPlayer(CPlayer* p) { m_pPlayer = p; };

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
	HRESULT Init();											// �����ݒ�
	void	InitEnvironment();								// �������̏����ݒ�
	void	Uninit();										// �I������
	void	UpdateEnvironment();							// �������̍X�V����
	void	AppearBossEvent();								// �{�X�o��C�x���g
	void	GenerateBlock();								// �u���b�N�̎�������
	bool	DetectAdjacentBlock(const D3DXVECTOR3& Pos);	// �אڂ������u���b�N�����o
	void	DestroyBlock();									// �u���b�N�̎����폜
	void	DestroyAllBlock();								// �S�u���b�N�̍폜
	void	UpdateHUD();									// HUD�̍X�V����

	/* �����܂� */
	void DEBUG_CIRCLE();	// �f�o�b�O�T�[�N��

	// <data>
	CObject_X*	 m_pDome;				// �h�[��
	CMotion_Set* m_pStatue;				// ����
	int			 m_nCntStatueVibration;	// �����U���J�E���g
	CPlayer*	 m_pPlayer;				// �v���C���[���
	CObject_HUD* m_pMap;				// �}�b�v�\��
	CObject_HUD* m_pPlayerLife[5];		// �v���C���[�̗͕̑\��
	CObject_HUD* m_pPlayerGauge;		// �v���C���[�̃Q�[�W�\��
	CObject_HUD* m_pPlayerGaugeWindow;	// �v���C���[�̃Q�[�W�E�B���h�E�\��
	CFan*		 m_pRenderFan;			// ��`�\��

	// <static data>
	static CField_Manager* m_pFieldManager;	// �t�B�[���h�}�l�[�W���[�̖{�� 
};

#endif // _FIELD_MANAGER_H_