//============================================================================
//
// �L�����N�^�[�X�e�[�g�A�w�b�_�t�@�C�� [character_state.h]
// Author : ���c����
//
//============================================================================

#ifndef _CHARACTER_STATE_
#define _CHARACTER_STATE_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "state.h"
#include "character.h"

//****************************************************
// �L�����N�^�[�X�e�[�g�N���X
//****************************************************
class CCharacter_State : public CState
{
public:

	CCharacter_State();						// �f�t�H���g�R���X�g���N�^
	virtual ~CCharacter_State() override;	// �f�X�g���N�^

	virtual void Update() override = 0;	// �X�V����

	CCharacter_State* GetNextState();				// �ύX�\��̃X�e�[�g���擾
	template <typename T> void SetNextState(T* pT);	// �ύX�\��̃X�e�[�g��ݒ�

protected:

	CCharacter*			m_pCharacter;	// �L�����N�^�[�̃|�C���^
	CCharacter_State*	m_pNextState;	// �ύX�\��̃X�e�[�g
};

//============================================================================
// �ύX�\��̃X�e�[�g��ݒ�
//============================================================================
template <typename T> void CCharacter_State::SetNextState(T* pT)
{
	m_pNextState = pT;
}

#endif	// _CHARACTER_STATE_