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
using namespace player;
using namespace player_state_manager;

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CPlayer::CPlayer() :
	CObject_X{ static_cast<int>(CObject::LAYER::MIDDLE) },
	m_pPlayerStateManager{ nullptr },
	m_PosTarget{ VEC3_INIT },
	m_fMoveSpeed{ DEFAULT_MOVE_SPEED },
	m_fDirection{ 0.0f }
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
	if (!m_pPlayerStateManager)
	{
		m_pPlayerStateManager = CPlayer_State_Manager::Create();
	}

	// �������p��ݒ�
	m_fDirection = D3DX_PI * -0.5f;

	// ���N���X�̏����ݒ�
	HRESULT hr = CObject_X::Init();

	if (FAILED(hr))
	{
		assert(false && "�v���C���[�N���X�̏����ݒ�Ɏ��s");
	}

	return hr;
}

//============================================================================
// �I������
//============================================================================
void CPlayer::Uninit()
{
	// �v���C���[�X�e�[�g�}�l�[�W���[�̔j��
	if (m_pPlayerStateManager)
	{
		// ���
		m_pPlayerStateManager->Release();
		
		// �|�C���^��������
		m_pPlayerStateManager = nullptr;
	}

	// ���N���X�̏I������
	CObject_X::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CPlayer::Update()
{	
	// ���p�̕␳
	CUtility::AdjustDirection(m_fDirection);
	
	// �v���C���[�X�e�[�g�}�l�[�W���[�̍X�V
	m_pPlayerStateManager->Update();

	// ���N���X�̍X�V����
	CObject_X::Update();
	
#ifdef _DEBUG
	CRenderer::GetInstance()->SetDebugString("�v���C���[���W : " + to_string(GetPos().x) + " :  " + to_string(GetPos().y) + " : " + to_string(GetPos().z));
	CRenderer::GetInstance()->SetDebugString("�v���C���[���� : " + to_string(GetRot().x * (180 / D3DX_PI)) + " :  " + to_string(GetRot().y * (180 / D3DX_PI)) + " : " + to_string(GetRot().z * (180 / D3DX_PI)));
	CRenderer::GetInstance()->SetDebugString("�v���C���[���p" + to_string(m_fDirection * (180 / D3DX_PI)));
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
// �ڕW���W���擾
//============================================================================
const Vec3& CPlayer::GetPosTarget() const
{
	return m_PosTarget;
}

//============================================================================
// �ڕW���W��ݒ�
//============================================================================
void CPlayer::SetPosTarget(Vec3 PosTarget)
{
	m_PosTarget = PosTarget;
}

//============================================================================
// �ړ����x���擾
//============================================================================
const float& CPlayer::GetMoveSpeed() const
{
	return m_fMoveSpeed;
}

//============================================================================
// �ړ����x��ݒ�
//============================================================================
void CPlayer::SetMoveSpeed(float fMoveSpeed)
{
	m_fMoveSpeed = fMoveSpeed;
}

//============================================================================
// ���p���擾
//============================================================================
const float& CPlayer::GetDirection() const
{
	return m_fDirection;
}

//============================================================================
// ���p��ݒ�
//============================================================================
void CPlayer::SetDirection(float fDirection)
{
	m_fDirection = fDirection;
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