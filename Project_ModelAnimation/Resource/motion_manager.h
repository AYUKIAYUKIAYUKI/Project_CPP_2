//============================================================================
// 
// ���[�V�����}�l�[�W���[�A�w�b�_�t�@�C�� [motion_manager.h]
// Author : ���c����
// 
//============================================================================

#ifndef _MOTION_MANAGER_H_
#define _MOTION_MANAGER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �O���錾
//****************************************************
class CObject_Parts;

//****************************************************
// ���[�V�����}�l�[�W���[�N���X
//****************************************************
class CMotion_Manager final
{
public:

	//****************************************************
	// ����ڕW�l�̍\���̂��`
	//****************************************************
	struct KeyDest
	{
		D3DXVECTOR3 ScaleTarget;	// �ڕW�k��
		D3DXVECTOR3 RotTarget;		// �ڕW����
		D3DXVECTOR3 PosTarget;		// �ڕW���W
	};

	//****************************************************
	// �L�[���̍\���̂��`
	//****************************************************
	struct MotionKey
	{
		WORD		wMaxFrame;	// �L�[�̑��t���[����
		KeyDest*	apDest;		// �p�[�c���Ƃ̓���ڕW�l
	};

	//****************************************************
	// ���[�V�������̍\���̂��`
	//****************************************************
	struct ActorMotion
	{
		bool		bLoop;		// ���[�v�t���O
		WORD		wMaxKey;	// ���[�V�����̑��L�[��
		MotionKey*	apKey;		// �L�[���
	};

	//****************************************************
	// �A�N�^�[�̍\���̂��`
	//****************************************************
	struct Actor
	{
		WORD						wNowMotion;		// �Đ����̃��[�V����
		WORD						wNowKey;		// �Đ����̃L�[
		WORD						wNowFrame;		// �Đ����̃t���[��
		std::vector<CObject_Parts*>	vpModelParts;	// ���f���p�[�c
		WORD						wMaxMotion;		// �����[�V������
		ActorMotion*				apMotion;		// ���[�V�������
	};

	// <special function>
	CMotion_Manager(const CMotion_Manager&) = delete;				// �R�s�[�R���X�g���N�^
	CMotion_Manager& operator=(const CMotion_Manager&) = delete;	// �R�s�[������Z�q
	CMotion_Manager(CMotion_Manager&&) = delete;					// ���[�u�R���X�g���N�^
	CMotion_Manager& operator=(CMotion_Manager&&) = delete;			// ���[�u������Z�q

	// <function>
	void Update();	// �X�V����

	// <static function>
	static HRESULT	Create();	// ����
	static void		Release();	// ���

	// <static getter>
	static CMotion_Manager* GetInstance();	// X���f���}�l�[�W���[���擾

private:

	// <special function>
	CMotion_Manager();	// �R���X�g���N�^
	~CMotion_Manager();	// �f�X�g���N�^

	// <function>
	HRESULT	Init();				// �����ݒ�
	void	Uninit();			// �I������
	void	Edit();				// �ҏW
	void	EditParts();		// �p�[�c���̕ҏW
	void	EditDest();			// �ڕW�l���̕ҏW
	void	EditMotion();		// ���[�V�������̕ҏW
	void	EditKey();			// �L�[���̕ҏW
	void	EditFrame();		// �t���[�����̕ҏW
	void	Export();			// �G�N�X�|�[�g
	void	Animation();		// ����
	void	CountFrame();		// �t���[�����J�E���g
	void	CorrectTarget();	// �ڕW�l�ւ̕␳
	void	Reset();			// ���Z�b�g

	// <getter>
	CObject_Parts* const GetSelectParts() const;	// �I�𒆂̃p�[�c�̃|�C���^���擾
	ActorMotion* const GetNowMotion() const;		// ���݂̃��[�V�����̃|�C���^���擾
	ActorMotion* const GetSelectMotion() const;		// �I�𒆂̃��[�V�����̃|�C���^���擾
	MotionKey* const GetNowKey() const;				// ���݂̃L�[�̃|�C���^���擾
	MotionKey* const GetSelectKey() const;			// �I�𒆂̃L�[�̃|�C���^���擾

	// <data>
	JSON	m_Json;				// �W�F�C�\���f�[�^
	Actor	m_Actor;			// �A�N�^�[
	WORD	m_wSelectParts;		// �I���p�[�c
	WORD	m_wPosEditCoef;		// ���W�ҏW�̋��x
	WORD	m_wSelectMotion;	// �I�����[�V����
	WORD	m_wSelectKey;		// �I���L�[
	bool	m_bPlay;			// �Đ�

	// <static data>
	static CMotion_Manager* m_pMotionManager;	// X���f���}�l�[�W���[�̖{��
};

#endif // _X_MANAGER_H_