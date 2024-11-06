//============================================================================
// 
// �t�B�[���h�}�l�[�W���[�A�w�b�_�t�@�C�� [field_manager.h]
// Author : ���c����
// 
//============================================================================

#ifndef _FIELD_MANAGER_H_
#define _FIELD_MANAGER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_X.h"
#include "player.h"

//****************************************************
// �O���錾
//****************************************************
class CObject_HUD;
class CFan;

//****************************************************
// �t�B�[���h�}�l�[�W���[�N���X
//****************************************************
class CField_Manager final
{
public:

	/// <summary> �t�B�[���h���a </summary>
	static constexpr float FIELD_RADIUS = 300.0f;

	/// <summary> �����͈͂̔��a </summary>
	static constexpr float GENERATE_RANGE_RADIUS = 350.0f;

	/// <summary> �ő�u���b�N�� </summary>
	static constexpr int MAX_BLOCK = 25;

	/// <summary> �d�� </summary>
	static constexpr float FIELD_GRAVITY = -0.025f;

	// <function>
	HRESULT Init();	// �����ݒ�
	void Release();	// ���
	void Update();	// �X�V����
	void Draw();	// �`�揈��

	// <static function>
	static CField_Manager* GetInstance();	// ���C���X�^���X���擾

private:

	// <special function>
	CField_Manager();	// �f�t�H���g�R���X�g���N�^
	~CField_Manager();	// �f�X�g���N�^

	// <function>
	void Create();										// ����
	void Uninit();										// �I������
	void TestCreate();									// ���̐������\�b�h
	bool DetectAdjacentBlock(const D3DXVECTOR3& Pos);	// �אڂ������u���b�N�����o
	void TestDelete();									// ���̔j�����\�b�h
	void TestDeleteAll();								// ���̑S�j�����\�b�h
	void UpdateHUD();									// HUD�̍X�V����

	// <data>
	CObject_HUD*	m_pMap;								// �}�b�v
	CPlayer*		m_pPlayer;							// �v���C���[	
	CObject_HUD*	m_pPlayerLife[CPlayer::MAX_LIFE];	// �v���C���[�̗̑�
	CObject_HUD*	m_pPlayerGauge;						// �v���C���[�̃Q�[�W
	CObject_HUD*	m_pPlayerGaugeWindow;				// �v���C���[�̃Q�[�W�E�B���h�E
	CObject_X*		m_pCylinderCollider;				// �~������
	CFan*			m_pFan;								// ��`

	// <static data>
	static CField_Manager* m_pField_Manager;	// �t�B�[���h�}�l�[�W���[�̖{�� 
};

#endif // _FIELD_MANAGER_H_