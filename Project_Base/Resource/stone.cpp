//============================================================================
// 
// �X�g�[�� [stone.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "stone.h"
#include "bounding_sphere.h"

#include "renderer.h"
#include "sound.h"
#include "field_manager.h"
#include "collision.h"
#include "enemy.h"

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
CStone::CStone() :
	CObject_X{ LAYER::DEFAULT },
	m_pBndSphere{ DBG_NEW CBounding_Sphere() },
	m_Accel{ VEC3_INIT },
	m_nDuration{ 0 }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CStone::~CStone()
{
	// ���o�E���f�B���O�̔j��
	if (m_pBndSphere != nullptr)
	{
		// �����������
		delete m_pBndSphere;

		// �|�C���^��������
		m_pBndSphere = nullptr;
	}
}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CStone::Init()
{
	// X�I�u�W�F�N�g�N���X�̏����ݒ�
	if (FAILED(CObject_X::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CStone::Uninit()
{
	// X�I�u�W�F�N�g�N���X�̏I������
	CObject_X::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CStone::Update()
{
	++m_nDuration;

	// �ړ�
	if (m_nDuration > WAIT_DURATION)
	{
		// ���W���擾
		Vec3 Pos = GetPos();
		Pos += m_Accel;
		SetPos(Pos);
	}

	// ���o�E���f�B���O�ɒ��S�_�Z�b�g
	m_pBndSphere->SetCenterPos(GetPos());

	// �G�Ƃ̓����蔻��
	HitCheckEnemy();

	if (m_nDuration > WAIT_DURATION)
	{
		{ // �k��

			// �X�P�[�����擾
			Vec3 Scale = GetScale();

			// ������
			float fAdjust = 0.0075f;

			// �X�P�[������
			Scale = {
				Scale.x - fAdjust,
				Scale.y - fAdjust,
				Scale.z - fAdjust
			};

			// ���a���X�V
			SetRadius(Scale.x * 7.5f);

			// ������ݒ�
			if (Scale.x < 0.15f)
			{
				Scale = VEC3_INIT;

				SetRelease();
			}

			SetScale(Scale);
		}

		{ // ��]

			float fCoef = fabsf(utility::GetRandomValue<float>() * 0.005f);

			// �������擾
			Vec3 Rot = GetRot();
			Rot.x += fCoef;
			Rot.y += fCoef;
			SetRot(Rot);
		}
	}

	// X�I�u�W�F�N�g�N���X�̍X�V����
	CObject_X::Update();
}

//============================================================================
// �`�揈��
//============================================================================
void CStone::Draw()
{
	// X�I�u�W�F�N�g�N���X�̕`�揈��
	CObject_X::Draw();
}

//============================================================================
// ���a���擾
//============================================================================
float CStone::GetRadius() const
{
	return m_pBndSphere->GetRadius();
}

//============================================================================
// ���a��ݒ�
//============================================================================
void CStone::SetRadius(float fRadius)
{
	m_pBndSphere->SetRadius(fRadius);
}

//============================================================================
// ����
//============================================================================
CStone* CStone::Create(D3DXVECTOR3 InitPos, D3DXVECTOR3 Accel)
{
	// �C���X�^���X�𐶐�
	CStone* pNewInstance = DBG_NEW CStone();

	// �^�C�v��ݒ�
	pNewInstance->SetType(TYPE::NONE);

	// �����ݒ�
	pNewInstance->Init();

	// ���f�����Z�b�g
	pNewInstance->BindModel(CX_Manager::TYPE::POWERSTONE);

	// �����X�P�[��
	pNewInstance->SetScale({ 0.4f, 0.4f, 0.4f });

	// ����������ݒ�
	float fCoef = fabsf(utility::GetRandomValue<float>());
	Vec3 Rot = pNewInstance->GetRot();
	Rot.x += fCoef;
	Rot.y += fCoef;
	pNewInstance->SetRot(Rot);

	// �����ʒu��ݒ�
	pNewInstance->SetPos(InitPos);

	// ���i�͂��Z�b�g
	pNewInstance->m_Accel = Accel;

	return pNewInstance;
}

//============================================================================
//
// private�����o
// 
//============================================================================

//============================================================================
// �G�l�~�[�Ƃ̐ڐG�����o
//============================================================================
void CStone::HitCheckEnemy()
{
	// �G�l�~�[�^�C�v�̃I�u�W�F�N�g��T��
	CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::ENEMY);

	// �a���o�E���f�B���O���R�s�[
	const CBounding_Sphere* pSlash = m_pBndSphere;

	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::ENEMY)
		{ // �G�l�~�[�^�C�v�Ȃ�

			// �G�l�~�[�N���X�ɃL���X�g
			CEnemy* pEnemy = CObject::DownCast<CEnemy>(pObj);

			// �U���ƃG�l�~�[�ł̓����蔻��
			if (collision::HitCylinderToSphere(pEnemy->CEnemy::GetBndCylinder(), pSlash))
			{
				// �_���[�W��^����
				pEnemy->SetDamage(-1);

				// ���g��j���\��
				SetRelease();
			}
		}

		// ���̃I�u�W�F�N�g��
		pObj = pObj->GetNext();
	}
}