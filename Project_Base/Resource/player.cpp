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

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CPlayer::CPlayer() :
	CObject_X(static_cast<int>(CObject::LAYER::MIDDLE)),
	m_PosTarget(VEC3_INIT),
	m_fMoveSpeed(DEFAULT_MOVE_SPEED),
	m_fDirection(0.0f)
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
	// �������p��ݒ�
	//m_fDirection = D3DX_PI * -0.5f;

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
	// ���N���X�̏I������
	CObject_X::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CPlayer::Update()
{
	// ���p�̕␳
	/* �K�������̐擪�� */
	CUtility::AdjustDirection(m_fDirection);

	// ����
	Control();

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

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ����
//============================================================================
void CPlayer::Control()
{
	//****************************************************
	// using�f�B���N�e�B�u
	//****************************************************
	using namespace field_manager;

	// �C���v�b�g�n�擾
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();	// �L�[�{�[�h
	CInputPad* pPad = CManager::GetPad();					// �p�b�h

	// X���̓���
	if (pKeyboard->GetPress(DIK_A) || pPad->GetPress(CInputPad::JOYKEY::LEFT) || pPad->GetJoyStickL().X < 0)
	{
		m_fDirection += -m_fMoveSpeed;
	}
	else if (pKeyboard->GetPress(DIK_D) || pPad->GetPress(CInputPad::JOYKEY::RIGHT) || pPad->GetJoyStickL().X > 0)
	{
		m_fDirection += m_fMoveSpeed;
	}

	// ���W�𔽉f
	Vec3 NewPos = VEC3_INIT;
	NewPos.x = cosf(m_fDirection) * CField_Manager::FIELD_RADIUS;
	NewPos.z = sinf(m_fDirection) * CField_Manager::FIELD_RADIUS;
	SetPos(NewPos);

	// �����𔽉f
	Vec3 NewRot = VEC3_INIT;
	//NewRot.y = atan2f(NewRot.x - NewPos.x, NewRot.z - NewPos.z);
	SetRot(NewRot);
}