//============================================================================
// 
// �v���C���[ [player.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "player.h"

// �C���v�b�g�擾
#include "manager.h"

// �t�B�[���h�}�l�[�W���[
#include "field_manager.h"

// �f�o�b�O�\���p
#include "renderer.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CPlayer::CPlayer() :
	CCharacter{}
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CPlayer::~CPlayer()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CPlayer::Init()
{
	// �v���C���[�X�e�[�g�}�l�[�W���[�̐���
	//if (!m_pPlayerStateManager)
	//{
	//	m_pPlayerStateManager = CPlayer_State_Manager::Create();
	//}

	// �������p��ݒ�
	SetDirection(D3DX_PI * -0.5f);

	// �����̗͂�ݒ�
	SetLife(MAX_LIFE);

	// �L�����N�^�[�N���X�̏����ݒ�
	if (FAILED(CCharacter::Init()))
	{
		assert(false && "�v���C���[�N���X�̏����ݒ�Ɏ��s");
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CPlayer::Uninit()
{
	// �v���C���[�X�e�[�g�}�l�[�W���[�̔j��
	//if (m_pPlayerStateManager)
	//{
	//	// ���
	//	m_pPlayerStateManager->Release();
	//	
	//	// �|�C���^��������
	//	m_pPlayerStateManager = nullptr;
	//}

	// �L�����N�^�[�N���X�̏I������
	CCharacter::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CPlayer::Update()
{		
	// �v���C���[�X�e�[�g�}�l�[�W���[�̍X�V
	//m_pPlayerStateManager->Update();
	
#ifdef _DEBUG

	// �V�����̗͏����쐬
	int nNewLife = GetLife();

	// �̗͂�ϓ�
	if (CManager::GetKeyboard()->GetTrigger(DIK_1))
	{
		nNewLife > 0 ? nNewLife += -1 : nNewLife = 0;
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_2))
	{
		nNewLife < MAX_LIFE ? nNewLife += 1 : nNewLife = MAX_LIFE;
	}

	// �ϓ������̗͏��𔽉f
	SetLife(nNewLife);

#endif // _DEBUG

	// �L�����N�^�[�N���X�̍X�V����
	CCharacter::Update();

#ifdef _DEBUG

	CRenderer::GetInstance()->SetDebugString("�v���C���[�̗́@�@ : " + to_string(GetLife()));
	CRenderer::GetInstance()->SetDebugString("�v���C���[���p�@�@ : " + to_string(GetDirection() * (180 / D3DX_PI)));
	CRenderer::GetInstance()->SetDebugString("�v���C���[���W�@�@ : " + to_string(GetPos().x) + " :  " + to_string(GetPos().y) + " : " + to_string(GetPos().z));
	CRenderer::GetInstance()->SetDebugString("�v���C���[���x�@�@ : " + to_string(GetMoveSpeed()));
	CRenderer::GetInstance()->SetDebugString("�v���C���[�����@�@ : " + to_string(GetRot().x * (180 / D3DX_PI)) + " :  " + to_string(GetRot().y * (180 / D3DX_PI)) + " : " + to_string(GetRot().z * (180 / D3DX_PI)));
	CRenderer::GetInstance()->SetDebugString("�v���C���[�ڕW���� : " + to_string(GetRotTarget().x * (180 / D3DX_PI)) + " :  " + to_string(GetRotTarget().y * (180 / D3DX_PI)) + " : " + to_string(GetRotTarget().z * (180 / D3DX_PI)));

#endif // _DEBUG
}

//============================================================================
// �`�揈��
//============================================================================
void CPlayer::Draw()
{
	// ���N���X�̕`�揈��
	CObject_X::Draw();
}

//============================================================================
// �_���[�W��Ԃ�
//============================================================================
void CPlayer::To_Damage(int nDamage)
{
	//m_nLife += -nDamage;

	//if (m_nLife < 0)
	//{
	//	m_nLife = 0;
	//}

	// �ύX�\��̃X�e�[�g�𐶐�
	//m_pPlayerStateManager
}

//============================================================================
// ����
//============================================================================
CPlayer* CPlayer::Create()
{
	// �C���X�^���X�𐶐�
	CPlayer* pNewInstance = DBG_NEW CPlayer();

	// �^�C�v��ݒ�
	pNewInstance->SetType(TYPE::PLAYER);
	
	// �����ݒ�
	pNewInstance->Init();

	// ���f����ݒ�
	pNewInstance->BindModel(CModel_X_Manager::TYPE::SAMUS);

	return pNewInstance;
}