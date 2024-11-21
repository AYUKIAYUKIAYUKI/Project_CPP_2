//============================================================================
// 
// ���[�V�����}�l�[�W���[�A�w�b�_�t�@�C�� [motion_manager.h]
// Author : ���c����
// 
//============================================================================

#ifndef _MOTION_MANAGER_H_
#define _MOTION_MANAGER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "motion_set.h"

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
	static CMotion_Manager* GetInstance();	// ���[�V�����}�l�[�W���[���擾

private:

	// <special function>
	CMotion_Manager();	// �R���X�g���N�^
	~CMotion_Manager();	// �f�X�g���N�^

	// <function>
	HRESULT	Init();			// �����ݒ�
	void	Uninit();		// �I������
	void	PrintDebug();	// �f�o�b�O�\��
	void	Edit();			// �ҏW
	void	EditParts();	// �p�[�c���̕ҏW
	void	EditMotion();	// ���[�V�������̕ҏW
	void	EditKey();		// �L�[���̕ҏW
	void	EditFrame();	// �t���[�����̕ҏW
	void	EditDest();		// �ڕW�l���̕ҏW
	void	Export();		// �G�N�X�|�[�g
	void	Reset();		// ���Z�b�g

	// <getter>
	CObject_Parts* const GetSelectParts() const;		// �I�𒆂̃p�[�c�̃|�C���^���擾
	CMotion_Set::Motion* const GetSelectMotion() const;	// �I�𒆂̃��[�V�����̃|�C���^���擾
	CMotion_Set::Key* const GetSelectKey() const;		// �I�𒆂̃L�[�̃|�C���^���擾

	// <data>
	JSON			m_Json;				// �W�F�C�\���f�[�^
	CMotion_Set*	m_MotionSet;		// ���[�V�����Z�b�g
	WORD			m_wSelectParts;		// �I���p�[�c
	WORD			m_wSelectMotion;	// �I�����[�V����
	WORD			m_wSelectKey;		// �I���L�[
	bool			m_bPlay;			// �Đ�

	// <static data>
	static CMotion_Manager* m_pMotionManager;	// ���[�V�����}�l�[�W���[�̖{��
};

#endif // _X_MANAGER_H_