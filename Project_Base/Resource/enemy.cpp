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

#include "collision.h"
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
// �R���X�g���N�^
//============================================================================
CEnemy::CEnemy() :
	CCharacter{},
	m_pBndCylinder{ DBG_NEW CBounding_Cylinder(this) }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CEnemy::~CEnemy()
{
	// �o�E���f�B���O�V�����_�[�̔j��
	if (m_pBndCylinder != nullptr)
	{
		// �����������
		delete m_pBndCylinder;

		// �|�C���^��������
		m_pBndCylinder = nullptr;
	}
}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CEnemy::Init()
{
	// �␳���x��ݒ�
	SetCorrectCoef(0.5f);

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
	// �����蔻��̒��S�_��ݒ�
	m_pBndCylinder->SetCenterPos(GetPos());

	// ���p�̎擾
	float fNewDirection = GetDirection();
	utility::AdjustDirection(fNewDirection);
	fNewDirection += -DEFAULT_MOVE_SPEED * 3.0f;
	SetDirection(fNewDirection);

	// �ڕW�������ړ�������
	AutoSetRotTarget();

	// ���p���玩���ō��W��ݒ�
	AutoSetPosTarget();

	// �v���C���[�ւ̍U��
	AttackOnPlayer();

	// �L�����N�^�[�N���X�̍X�V����
	CCharacter::Update();

#if 1
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
// ���a���擾
//============================================================================
float CEnemy::GetRadius() const
{
	return m_pBndCylinder->GetRadius();
}

//============================================================================
// �������擾
//============================================================================
float CEnemy::GetHeight() const
{
	return m_pBndCylinder->GetHeight();
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
	pNewInstance->BindModel(CX_Manager::TYPE::ENEMY);

	// ���a��ݒ�
	float fRad = 0.0f;
	pNewInstance->GetModel()->Size.x > pNewInstance->GetModel()->Size.z ? fRad = pNewInstance->GetModel()->Size.x : fRad = pNewInstance->GetModel()->Size.z;
	pNewInstance->m_pBndCylinder->SetRadius(fRad);

	// ������ݒ�
	pNewInstance->m_pBndCylinder->SetHeight(pNewInstance->GetModel()->Size.y);

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
		pPlayer = utility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));

		// �v���C���[�̎��~���͈͓��ɐN�����Ă�����
		if (collision::HitCylinderToCylinder(m_pBndCylinder, pPlayer->GetBndCylinder()))
		{
			// ���Ƀ_���[�W��Ԃ̏ꍇ�͏������I��
			/* ���̏������͔���\���̐F��߂����߂ɂ����ɂ���܂����A�{���͓����蔻��O�ɂ�����𔻒肵�܂� */
			if (typeid(*pPlayer->GetNowState()) == typeid(CPlayer_State_Damage))
			{
				return;
			}

			/* ����\����ԐF�� */
			m_pBndCylinder->SetCol({ 1.0f, 0.0f, 0.0f, 0.5f });

			// �_���[�W��^����
			pPlayer->SetDamage(-1);
		}
		else
		{
			/* ����\����ʏ�F�ɖ߂� */
			m_pBndCylinder->SetCol({ 1.0f, 1.0f, 1.0f, 0.5f });
		}
	}
	else
	{
		assert(false && "�v���C���[�̌������ʂ�����܂���ł���");
	}
}