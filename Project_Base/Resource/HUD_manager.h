//============================================================================
// 
// HUD�}�l�[�W���[�A�w�b�_�t�@�C�� [HUD_manager.h]
// Author : ���c����
// 
//============================================================================

#ifndef _HUD_MANAGER_H_
#define _HUD_MANAGER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "player.h"

//****************************************************
// �O���錾
//****************************************************
class CObject_HUD;

//****************************************************
// �t�B�[���h�}�l�[�W���[�N���X
//****************************************************
class CHUD_Manager final
{
public:

	// <special function>
	CHUD_Manager(const CHUD_Manager&) = delete;				// �R�s�[�R���X�g���N�^
	CHUD_Manager& operator=(const CHUD_Manager&) = delete;	// �R�s�[������Z�q
	CHUD_Manager(CHUD_Manager&&) = delete;					// ���[�u�R���X�g���N�^
	CHUD_Manager& operator=(CHUD_Manager&&) = delete;		// ���[�u������Z�q

	// <function>
	HRESULT Init();		// �����ݒ�
	void	Update();	// �X�V����
	void	Draw();		// �`�揈��

	// <setter>
	void SetSyncPlayer(const CPlayer* const pPlayer);	// �v���C���[���Z�b�g

	// <static function>
	static HRESULT	Create();	// ����
	static void		Release();	// ���

	// <static getter>
	static CHUD_Manager* GetInstance();	// HUD�}�l�[�W���[���擾

private:

	// <special function>
	CHUD_Manager();		// �R���X�g���N�^
	~CHUD_Manager();	// �f�X�g���N�^

	// <function>
	void Uninit();		// �I������
	bool DetectError();	// �G���[���o

	// <data>
	const CPlayer* m_pSyncPlayer;						// �v���C���[�̃|�C���^
	CObject_HUD*   m_pMap;								// �}�b�v�\��
	CObject_HUD*   m_pPlayerGauge;						// �v���C���[�̃Q�[�W�\��
	CObject_HUD*   m_pPlayerGaugeWindow;				// �v���C���[�̃Q�[�W�g�\��
	CObject_HUD*   m_pPlayerLife[CPlayer::MAX_LIFE];	// �v���C���[�̗͕̑\��

	// <static data>
	static CHUD_Manager* m_pHUDManager;	// HUD�}�l�[�W���[�̖{�� 
};

#endif // _FIELD_MANAGER_H_