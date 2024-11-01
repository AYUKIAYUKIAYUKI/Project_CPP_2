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

	CCharacter_State();								// �f�t�H���g�R���X�g���N�^
	CCharacter_State(CCharacter* pCharacter);		// �L�����N�^�[�擾�R���X�g���N�^
	~CCharacter_State() override;					// �f�X�g���N�^

	virtual void Update() override = 0;	// �X�V����

	CCharacter_State* GetNextState();				// �ύX�\��̃X�e�[�g���擾
	template <typename T> void SetNetState(T* pT);	// �ύX�\��̃X�e�[�g��ݒ�

protected:

	CCharacter* m_pCharacter;	// �L�����N�^�[�̃|�C���^

private:

	CCharacter* RetrieveCharacter();	// �L�����N�^�[�|�C���^���擾

	CCharacter_State* m_pNextState;	// �ύX�\��̃X�e�[�g
};

//============================================================================
// �ύX�\��̃X�e�[�g��ݒ�
//============================================================================
template <typename T> void CCharacter_State::SetNetState(T* pT)
{
	m_pNextState = pT;
}

#endif	// _CHARACTER_STATE_