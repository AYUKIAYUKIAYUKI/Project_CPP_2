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

#include "renderer.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;
using namespace player;

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
	fNewDirection += DEFAULT_MOVE_SPEED;
	SetDirection(fNewDirection);

	// �v���C���[�ւ̍U��
	AttackOnPlayer();

	// �L�����N�^�[�N���X�̍X�V����
	CCharacter::Update();

#ifdef _DEBUG

	CRenderer::GetInstance()->SetDebugString("�G�l�~�[�̗́@�@ : " + to_string(GetLife()));
	CRenderer::GetInstance()->SetDebugString("�G�l�~�[���p�@�@ : " + to_string(GetDirection() * (180 / D3DX_PI)));
	CRenderer::GetInstance()->SetDebugString("�G�l�~�[���W�@�@ : " + to_string(GetPos().x) + " :  " + to_string(GetPos().y) + " : " + to_string(GetPos().z));
	CRenderer::GetInstance()->SetDebugString("�G�l�~�[���x�@�@ : " + to_string(GetMoveSpeed()));
	CRenderer::GetInstance()->SetDebugString("�G�l�~�[�����@�@ : " + to_string(GetRot().x * (180 / D3DX_PI)) + " :  " + to_string(GetRot().y * (180 / D3DX_PI)) + " : " + to_string(GetRot().z * (180 / D3DX_PI)));
	CRenderer::GetInstance()->SetDebugString("�G�l�~�[�ڕW���� : " + to_string(GetRotTarget().x * (180 / D3DX_PI)) + " :  " + to_string(GetRotTarget().y * (180 / D3DX_PI)) + " : " + to_string(GetRotTarget().z * (180 / D3DX_PI)));

#endif // _DEBUG
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
	CPlayer* pPlayer = nullptr;

	// �v���C���[������
	if (CObject::FindSpecificObject(CObject::TYPE::PLAYER) != nullptr)
	{
		// �v���C���[�ւ̃|�C���^��ێ�
		pPlayer = CUtility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));

		// �v���C���[�̎��~���͈͓��ɐN�����Ă�����
		if (CUtility::CylinderAndPoint(pPlayer->GetPos(), 10.0f, 100.0f, GetPos()))
		{
			// �̗͂�����
			int nLife = pPlayer->GetLife();
			nLife--;
			pPlayer->SetLife(nLife);
		}
	}
	else
	{
		assert(false && "�v���C���[�̌������ʂ�����܂���ł���");
	}
}