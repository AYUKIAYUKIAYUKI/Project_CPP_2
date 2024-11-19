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

	// <special function>
	CCharacter_State();						// �f�t�H���g�R���X�g���N�^
	virtual ~CCharacter_State() override;	// �f�X�g���N�^

	// <virtual function>
	virtual void Update() override = 0;	// �X�V����

	// <function>
	template <typename T> T* CheckChangeState();	// �X�e�[�g�̕ύX���m�F

	// <setter/getter>
	CCharacter_State* GetNextState();				// �ύX�\��̃X�e�[�g���擾
	template <typename T> void SetNextState(T* pT);	// �ύX�\��̃X�e�[�g��ݒ�

protected:

	// <data>
	CCharacter*			m_pCharacter;	// �L�����N�^�[�̃|�C���^
	CCharacter_State*	m_pNextState;	// �ύX�\��̃X�e�[�g
};

//============================================================================
// �X�e�[�g�̕ύX�m�F
//============================================================================
template <typename T> T* CCharacter_State::CheckChangeState()
{
	// �ύX�\��̃X�e�[�g�͖���
	if (!m_pNextState)
	{
		return dynamic_cast<T*>(this);
	}

	// �ύX��̃|�C���^���R�s�[
	T* pNextState = utility::DownCast<T, CCharacter_State>(m_pNextState);

	// �����o�̎��ύX��̏���������
	m_pNextState = nullptr;

	// ���g��j��
	delete this;

	return pNextState;
}

//============================================================================
// �ύX�\��̃X�e�[�g��ݒ�
//============================================================================
template <typename T> void CCharacter_State::SetNextState(T* pT)
{
	m_pNextState = pT;
}

#endif	// _CHARACTER_STATE_