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
	{ // �ړ�

		// ���W���擾
		Vec3 Pos = GetPos();
		Pos += m_Accel;
		SetPos(Pos);
	}

	// ���o�E���f�B���O�ɒ��S�_�Z�b�g
	m_pBndSphere->SetCenterPos(GetPos());

	// �G�Ƃ̓����蔻��
	HitCheckEnemy();

	{ // �k��
	
		// �T�C�Y���擾
		Vec3 Scale = GetScale();

		// ������
		float fAdjust = 0.01f;

		// �X�P�[������
		Scale = {
			Scale.x - fAdjust,
			Scale.y - fAdjust,
			Scale.z - fAdjust
		};

		// ������ݒ�
		if (Scale. x < 0.0f)
		{
			Scale = VEC3_INIT;

			SetRelease();
		}
	}

	{ // ��]

		// �������擾
		Vec3 Rot = GetRot();
		Rot.y += 0.001f;
		SetRot(Rot);
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
CStone* CStone::Create(D3DXVECTOR3 Accel)
{
	// �C���X�^���X�𐶐�
	CStone* pNewInstance = DBG_NEW CStone();

	// �^�C�v��ݒ�
	pNewInstance->SetType(TYPE::NONE);

	// �����ݒ�
	pNewInstance->Init();

	// ���f�����Z�b�g
	pNewInstance->BindModel(CX_Manager::TYPE::POWERSTONE);

	// ���a��ݒ�
	pNewInstance->SetRadius(2.5f);

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