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
#include "player_state_default.h"

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
	CCharacter{},
	m_pStateManager{ nullptr }
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
	// �X�e�[�g�}�l�[�W���[�̐���
	if (!m_pStateManager)
	{
		m_pStateManager = CCharacter_State_Manager::Create(DBG_NEW CPlayer_State_Default());
	}

	// �������p��ݒ�
	SetDirection(D3DX_PI * -0.5f);

	// �����ړ����x��ݒ�
	SetMoveSpeed(DEFAULT_MOVE_SPEED);

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
	// �X�e�[�g�}�l�[�W���[�̔j��
	if (m_pStateManager)
	{
		// ���
		m_pStateManager->Release();
		
		// �|�C���^��������
		m_pStateManager = nullptr;
	}

	// �L�����N�^�[�N���X�̏I������
	CCharacter::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CPlayer::Update()
{		
	// �X�e�[�g�}�l�[�W���[�̍X�V
	m_pStateManager->Update();
	
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
	// �L�����N�^�[�N���X�̕`�揈��
	CCharacter::Draw();

#if 1	// �X�e���V���o�b�t�@�Ƀv���C���[�̃V���G�b�g��`�悷��

	// �X�e���V���o�b�t�@��L����
	//CRenderer::GetInstance()->GetDeviece()->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// �f�v�X�o�b�t�@�̔�r���@��ύX��������
	CRenderer::GetInstance()->GetDeviece()->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);

	// �L�����N�^�[�N���X�̕`�揈�� (�X�e���V���o�b�t�@)
	CCharacter::Draw();

	// �X�e���V���o�b�t�@�𖳌���
	//CRenderer::GetInstance()->GetDeviece()->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	// �f�v�X�o�b�t�@�̔�r���@��ʏ�ɖ߂��L����
	CRenderer::GetInstance()->GetDeviece()->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

#endif

#if 0
	for (DWORD i = 0; i < 10; ++i)
	{
		// �X�e�[�g�}�l�[�W���[�̍X�V
		CCharacter::Update();

		// �L�����N�^�[�N���X�̃N���X�̕`�揈��
		CCharacter::Draw();
	}
#endif
}

//============================================================================
// �X�e�[�g - �_���[�W��Ԃ�
//============================================================================
void CPlayer::To_Damage(int nDamage)
{
	// �_���[�W�ʕ��A�̗͂�ϓ�
	 int nNewLife = GetLife();
	 nNewLife += nDamage;
	 SetLife(nNewLife);

	// �}�l�[�W���[�Ƀ_���[�W��Ԃ֕ύX����悤���߂���
	m_pStateManager->To_Damage();
}

//============================================================================
// ���݂̃X�e�[�g���擾
//============================================================================
CCharacter_State* CPlayer::GetNowState()
{
	// �X�e�[�g�}�l�[�W���[�̎��X�e�[�g�I�u�W�F�N�g�̃|�C���^��Ԃ�
	return m_pStateManager->GetNowState();
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