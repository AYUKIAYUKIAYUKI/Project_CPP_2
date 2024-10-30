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

#include "field_manager.h"
#include "player.h"

#include "renderer.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;
using namespace field_manager;
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
	CObject_X{ static_cast<int>(CObject::LAYER::MIDDLE) },
	m_fDirection{ 0.0f },
	m_PosTarget{ VEC3_INIT },
	m_fMoveSpeed{ DEFAULT_MOVE_SPEED },
	m_RotTarget{ VEC3_INIT },
	m_nLife{ MAX_LIFE }
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
	// �������p��ݒ�
	m_fDirection = D3DX_PI * 0.5f;

	// X�I�u�W�F�N�g�̏����ݒ�
	if (FAILED(CObject_X::Init()))
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
	// ���N���X�̏I������
	CObject_X::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CEnemy::Update()
{
	// ���p�̕␳
	CUtility::AdjustDirection(m_fDirection);

	// ���p�̕ϓ�
	m_fDirection += DEFAULT_MOVE_SPEED;

	// �����̔��f
	Vec3 NewRotTarget = GetRotTarget();					// �ڕW�������擾
	const Vec3& MoveVec = GetPosTarget() - GetPos();	// �ړ������̃x�N�g�����쐬
	NewRotTarget.y = atan2f(-MoveVec.x, -MoveVec.z);	// �ڕW�������ړ�������
	SetRotTarget(NewRotTarget);							// �ڕW�����𔽉f

	// ���p�̔��f
	Vec3 NewPosTarget = VEC3_INIT;										// �V�K�ڕW���W���쐬
	NewPosTarget.x = cosf(m_fDirection) * CField_Manager::FIELD_RADIUS;	// X�����̍��W��ݒ�
	NewPosTarget.z = sinf(m_fDirection) * CField_Manager::FIELD_RADIUS;	// Z�����̍��W��ݒ�
	SetPosTarget(NewPosTarget);											// �ڕW���W�𔽉f

	// �ڕW�l�ւ̕␳
	AdjustToTarget();

	// �v���C���[�ւ̍U��
	AttackOnPlayer();

	// ���N���X�̍X�V����
	CObject_X::Update();

#ifdef _DEBUG

	CRenderer::GetInstance()->SetDebugString("�G�l�~�[�̗́@�@ : " + to_string(m_nLife));
	CRenderer::GetInstance()->SetDebugString("�G�l�~�[���p�@�@ : " + to_string(m_fDirection * (180 / D3DX_PI)));
	CRenderer::GetInstance()->SetDebugString("�G�l�~�[���W�@�@ : " + to_string(GetPos().x) + " :  " + to_string(GetPos().y) + " : " + to_string(GetPos().z));
	CRenderer::GetInstance()->SetDebugString("�G�l�~�[���x�@�@ : " + to_string(m_fMoveSpeed));
	CRenderer::GetInstance()->SetDebugString("�G�l�~�[�����@�@ : " + to_string(GetRot().x * (180 / D3DX_PI)) + " :  " + to_string(GetRot().y * (180 / D3DX_PI)) + " : " + to_string(GetRot().z * (180 / D3DX_PI)));
	CRenderer::GetInstance()->SetDebugString("�G�l�~�[�ڕW���� : " + to_string(GetRotTarget().x * (180 / D3DX_PI)) + " :  " + to_string(GetRotTarget().y * (180 / D3DX_PI)) + " : " + to_string(GetRotTarget().z * (180 / D3DX_PI)));

#endif // _DEBUG
}

//============================================================================
// �`�揈��
//============================================================================
void CEnemy::Draw()
{
	// ���N���X�̕`�揈��
	CObject_X::Draw();
}

//============================================================================
// �̗͂��擾
//============================================================================
const int& CEnemy::GetLife() const
{
	return m_nLife;
}

//============================================================================
// �̗͂�ݒ�
//============================================================================
void CEnemy::SetLife(int nLife)
{
	m_nLife = nLife;
}

//============================================================================
// �ڕW���W���擾
//============================================================================
const Vec3& CEnemy::GetPosTarget() const
{
	return m_PosTarget;
}

//============================================================================
// �ڕW���W��ݒ�
//============================================================================
void CEnemy::SetPosTarget(Vec3 PosTarget)
{
	m_PosTarget = PosTarget;
}

//============================================================================
// �ڕW�������擾
//============================================================================
const Vec3& CEnemy::GetRotTarget() const
{
	return m_RotTarget;
}

//============================================================================
// �ڕW������ݒ�
//============================================================================
void CEnemy::SetRotTarget(Vec3 RotTarget)
{
	m_RotTarget = RotTarget;
}


//============================================================================
// �ړ����x���擾
//============================================================================
const float& CEnemy::GetMoveSpeed() const
{
	return m_fMoveSpeed;
}

//============================================================================
// �ړ����x��ݒ�
//============================================================================
void CEnemy::SetMoveSpeed(float fMoveSpeed)
{
	m_fMoveSpeed = fMoveSpeed;
}

//============================================================================
// ���p���擾
//============================================================================
const float& CEnemy::GetDirection() const
{
	return m_fDirection;
}

//============================================================================
// ���p��ݒ�
//============================================================================
void CEnemy::SetDirection(float fDirection)
{
	m_fDirection = fDirection;
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

//============================================================================
// �ڕW�l�ւ̕␳
//============================================================================
void CEnemy::AdjustToTarget()
{
	// �ڕW���W�ֈړ�
	Vec3 NowPos = GetPos();
	NowPos += (m_PosTarget - NowPos) * COEF_ADJUST;
	SetPos(NowPos);

	// �ڕW�����֕␳
	Vec3 NowRot = GetRot();
	CUtility::AdjustAngle(NowRot.y, m_RotTarget.y);	// �p�x�̍���␳
	NowRot += (m_RotTarget - NowRot) * COEF_ADJUST;
	SetRot(NowRot);
}