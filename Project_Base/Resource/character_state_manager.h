//============================================================================
//
// �L�����N�^�[�X�e�[�g�}�l�[�W���[�A�w�b�_�t�@�C�� [character_state_manager.h]
// Author : ���c����
//
//============================================================================
//
//#ifndef _CHARACTER_STATE_MANAGER_H_
//#define _CHARACTER_STATE_MANAGER_H_	// ��d�C���N���[�h�h�~
//
//****************************************************
// �O���錾
//****************************************************
//class CCharacter_State;
//
//****************************************************
// �L�����N�^�[�X�e�[�g�}�l�[�W���[�N���X
//****************************************************
//class CCharacter_State_Manager
//{
//public:
//
//	 <special function>
//	CCharacter_State_Manager(CCharacter_State* pState);	// �R���X�g���N�^
//	~CCharacter_State_Manager();						// �f�X�g���N�^
//
//	 <function>
//	void Release();				// �j��
//	void Update();				// �X�V����
//	void CheckChangeState();	// �X�e�[�g�̕ύX���m�F
//
//	 <getter>
//	CCharacter_State* GetNowState();	// ���݂̃X�e�[�g���擾
//
//	 <static function>
//	static CCharacter_State_Manager* Create(CCharacter_State* pState);	// ����
//
//private:
//
//	 <function>
//	HRESULT Init(CCharacter_State* pState);	// �����ݒ�
//	void	Uninit();						// �I���ݒ�
//
//	 <data>
//	CCharacter_State* m_pState;	// �X�e�[�g�̃|�C���^
//};
//
//#endif	// _CHARACTER_STATE_MANAGER_H_