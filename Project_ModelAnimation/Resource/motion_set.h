//============================================================================
// 
// ���[�V�����}�l�[�W���[�A�w�b�_�t�@�C�� [motion_manager.h]
// Author : ���c����
// 
//============================================================================

#ifndef _MOTION_SET_H_
#define _MOTION_SET_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �O���錾
//****************************************************
class CObject_Parts;

//****************************************************
// ���[�V�����Z�b�g�N���X
//****************************************************
class CMotion_Set
{
	friend class CMotion_Manager;

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
	struct Key
	{
		WORD		wMaxFrame;	// �L�[�̑��t���[����
		KeyDest*	apDest;		// �p�[�c���Ƃ̓���ڕW�l
	};

	//****************************************************
	// ���[�V�������̍\���̂��`
	//****************************************************
	struct Motion
	{
		bool bLoop;		// ���[�v�t���O
		WORD wMaxKey;	// ���[�V�����̑��L�[��
		Key* apKey;		// �L�[���
	};

	// <special function>
	CMotion_Set();	// �R���X�g���N�^
	~CMotion_Set();	// �f�X�g���N�^

	// <function>
	void Update();			// �X�V����
	void Animation();		// ����
	void CountFrame();		// �t���[�����J�E���g
	void CorrectTarget();	// �ڕW�l�ւ̕␳

private:

	// <getter>
	CMotion_Set::Motion* const GetNowMotion() const;	// ���݂̃��[�V�����̃|�C���^���擾
	CMotion_Set::Key* const GetNowKey() const;			// ���݂̃L�[�̃|�C���^���擾

	// <data>
	WORD						m_wNowMotion;	// �Đ����̃��[�V����
	WORD						m_wNowKey;		// �Đ����̃L�[
	WORD						m_wNowFrame;	// �Đ����̃t���[��
	std::vector<CObject_Parts*>	m_vpModelParts;	// ���f���p�[�c
	WORD						m_wMaxMotion;	// �����[�V������
	Motion*						m_apMotion;		// ���[�V�������
};

#endif	// _MOTION_SET_