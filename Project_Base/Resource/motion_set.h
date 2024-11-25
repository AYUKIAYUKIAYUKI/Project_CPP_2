//============================================================================
// 
// ���[�V�����Z�b�g�A�w�b�_�t�@�C�� [motion_set.h]
// Author : ���c����
// 
//============================================================================

#ifndef _MOTION_SET_H_
#define _MOTION_SET_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "motion_set.h"
#include "object.h"

//****************************************************
// �O���錾
//****************************************************
class CObject_Parts;

//****************************************************
// ���[�V�����Z�b�g�N���X
//****************************************************
class CMotion_Set : public CObject
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
	struct Key
	{
		int			nMaxFrame;	// �L�[�̑��t���[����
		KeyDest*	apDest;		// �p�[�c���Ƃ̓���ڕW�l
	};

	//****************************************************
	// ���[�V�������̍\���̂��`
	//****************************************************
	struct Motion
	{
		bool bLoop;				// ���[�v�t���O
		WORD wMaxKey;			// ���[�V�����̑��L�[��
		std::vector<Key> vpKey;	// �L�[���
	};

	// <special function>
	CMotion_Set();				// �R���X�g���N�^
	~CMotion_Set() override;	// �f�X�g���N�^

	// <function>
	HRESULT	Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <setter>
	void SetPosSync(D3DXVECTOR3 Pos);	// �V���N�����W��ݒ�

	// <getter/setter>
	WORD GetNowMotion();			// �Đ����̃��[�V�����ԍ����擾
	void SetNowMotion(WORD wIdx);	// �Đ����̃��[�V�����ԍ���ݒ�

	// <getter>
	CObject_Parts* GetParentParts();	// �e�p�[�c���擾

	// <static function>
	static CMotion_Set* Create(JSON Json);	// ����

private:

	// <function>
	void Animation();		// ����
	void CountFrame();		// �t���[�����J�E���g
	void CorrectTarget();	// �ڕW�l�ւ̕␳

	// <getter>
	CMotion_Set::Motion* const GetNowMotionPtr();	// ���݂̃��[�V�����̃|�C���^���擾
	CMotion_Set::Key* const GetNowKey();			// ���݂̃L�[�̃|�C���^���擾

	// <data>
	WORD						m_wNowMotion;	// �Đ����̃��[�V����
	WORD						m_wNowKey;		// �Đ����̃L�[
	WORD						m_wNowFrame;	// �Đ����̃t���[��
	std::vector<CObject_Parts*>	m_vpModelParts;	// ���f���p�[�c
	CObject_Parts*				m_pMostParent;	// �e�p�[�c
	WORD						m_wMaxParts;	// ���f���̑��p�[�c��
	WORD						m_wMaxMotion;	// �����[�V������
	std::vector<Motion>			m_vpMotion;		// ���[�V�������
};

#endif	// _MOTION_SET_