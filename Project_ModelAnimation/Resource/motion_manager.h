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
class CObject_X;

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
		WORD					wNowMotion;		// �Đ����̃��[�V����
		WORD					wNowKey;		// �Đ����̃L�[
		WORD					wNowFrame;		// �Đ����̃t���[��
		std::vector<CObject_X*>	vpModelParts;	// ���f���p�[�c
		WORD					wMaxMotion;		// �����[�V������
		ActorMotion*			apMotion;		// ���[�V�������
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
	void	Reset();			// ���Z�b�g
	void	Animation();		// ����
	void	CountFrame();		// �t���[�����J�E���g
	void	CorrectTarget();	// �ڕW�l�ւ̕␳

	// <data>
	Actor m_Actor;	// �A�N�^�[

	// <static data>
	static CMotion_Manager* m_pMotionManager;	// X���f���}�l�[�W���[�̖{��
};

#endif // _X_MANAGER_H_