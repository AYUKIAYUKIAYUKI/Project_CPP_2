//============================================================================
// 
// �G�l�~�[ [enemy.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "enemy.h"

#include "player.h"
#include "player_state_damage.h"

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
CEnemy::CEnemy() :
	CCharacter{}
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CEnemy::~CEnemy()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CEnemy::Init()
{
	// �L�����N�^�[�̏����ݒ�
	if (FAILED(CCharacter::Init()))
	{
		assert(false && "�G�l�~�[�N���X�̏����ݒ�Ɏ��s");
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CEnemy::Uninit()
{
	// �L�����N�^�[�N���X�̏I������
	CCharacter::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CEnemy::Update()
{
	// ���p�̎擾
	float fNewDirection = GetDirection();
	CUtility::AdjustDirection(fNewDirection);
	fNewDirection += -DEFAULT_MOVE_SPEED * 3.0f;
	SetDirection(fNewDirection);

	// �v���C���[�ւ̍U��
	AttackOnPlayer();

	// �L�����N�^�[�N���X�̍X�V����
	CCharacter::Update();

#if 0
#ifdef _DEBUG

	CRenderer::SetDebugString("��������������������������������������������������������������");
	CRenderer::SetDebugString("�G�l�~�[�̗́@�@ : " + to_string(GetLife()));
	CRenderer::SetDebugString("�G�l�~�[���p�@�@ : " + to_string(GetDirection() * (180 / D3DX_PI)));
	CRenderer::SetDebugString("�G�l�~�[���W�@�@ : " + to_string(GetPos().x) + " :  " + to_string(GetPos().y) + " : " + to_string(GetPos().z));
	CRenderer::SetDebugString("�G�l�~�[���x�@�@ : " + to_string(GetMoveSpeed()));
	CRenderer::SetDebugString("�G�l�~�[�����@�@ : " + to_string(GetRot().x * (180 / D3DX_PI)) + " :  " + to_string(GetRot().y * (180 / D3DX_PI)) + " : " + to_string(GetRot().z * (180 / D3DX_PI)));
	CRenderer::SetDebugString("�G�l�~�[�ڕW���� : " + to_string(GetRotTarget().x * (180 / D3DX_PI)) + " :  " + to_string(GetRotTarget().y * (180 / D3DX_PI)) + " : " + to_string(GetRotTarget().z * (180 / D3DX_PI)));
	CRenderer::SetDebugString("��������������������������������������������������������������");

#endif // _DEBUG
#endif
}

//============================================================================
// �`�揈��
//============================================================================
void CEnemy::Draw()
{
	// �L�����N�^�[�̕`�揈��
	CCharacter::Draw();
}

//============================================================================
// ����
//============================================================================
CEnemy* CEnemy::Create()
{
	// �C���X�^���X�𐶐�
	CEnemy* pNewInstance = DBG_NEW CEnemy();

	// �^�C�v��ݒ�
	pNewInstance->SetType(TYPE::ENEMY);

	// �����ݒ�
	pNewInstance->Init();

	// ���f����ݒ�
	pNewInstance->BindModel(CModel_X_Manager::TYPE::ENEMY);

	return pNewInstance;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �v���C���[�֍U��
//============================================================================
void CEnemy::AttackOnPlayer()
{		
	// �v���C���[�̃|�C���^���쐬
	CPlayer* pPlayer = nullptr;

	// �v���C���[������
	if (CObject::FindSpecificObject(CObject::TYPE::PLAYER) != nullptr)
	{
		// �v���C���[�I�u�W�F�N�g��ێ�
		pPlayer = CUtility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));

		// ���Ƀ_���[�W��Ԃ̏ꍇ�͏������I��
		if (typeid(*pPlayer->GetNowState()) == typeid(CPlayer_State_Damage))
		{
			return;
		}

		// �v���C���[�̎��~���͈͓��ɐN�����Ă�����
		if (CUtility::CylinderAndPoint(pPlayer->GetPos(), 10.0f, 100.0f, GetPos()))
		{
			// �_���[�W��Ԃ�
			pPlayer->To_Damage(-1);
		}
	}
	else
	{
		assert(false && "�v���C���[�̌������ʂ�����܂���ł���");
	}
}