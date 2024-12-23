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
#include "bounding_cylinder.h"

#include "collision.h"
#include "object_parts.h"
#include "field_manager.h"
#include "player.h"
#include "player_state_damage.h"
#include "block.h"
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
CEnemy::CEnemy(LAYER Layer) :
	CCharacter{ Layer },
	m_pBndCylinder{ DBG_NEW CBounding_Cylinder() }
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
	// �L�����N�^�[�N���X�̏����ݒ�
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
	// �����̕␳
	AdjustHeight();

	// �����蔻��̒��S�_��ݒ�
	m_pBndCylinder->SetCenterPos(GetPos());

	// �L�����N�^�[�N���X�̍X�V����
	CCharacter::Update();
}

//============================================================================
// �`�揈��
//============================================================================
void CEnemy::Draw()
{

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
// �~���o�E���f�B���O���擾
//============================================================================
const CBounding_Cylinder* const CEnemy::GetBndCylinder() const
{
	return m_pBndCylinder;
}

//============================================================================
// �_���[�W���󂯂�
//============================================================================
void CEnemy::SetDamage(int nDamage)
{
	// �_���[�W�ʕ��A�̗͂�ϓ�
	int nNewLife = GetLife();
	nNewLife += nDamage;
	SetLife(nNewLife);
}

//============================================================================
// �������ɑ΂����𐮂���
//============================================================================
void CEnemy::AdjustInitDirection(float fHeight)
{
	// ���p���R�s�[
	float fDirection = GetDirection();

	// �ڕW���p�𑵂���
	SetDirectionTarget(fDirection);

	// ��������p�ɍ��킹��
	Vec3 NewRot = { 0.0f, fDirection, 0.0f };
	SetRot(NewRot);
	SetRotTarget(NewRot);

	// ���W����p�E�t�B�[���h�͈͂ɍ��킹��
	Vec3 NewPos = utility::DirectionConvertVec3(fDirection, fHeight, CField_Manager::FIELD_RADIUS);
	SetPos(NewPos);
	SetPosTarget(NewPos);
}

//============================================================================
// 
// prtected�����o
// 
//============================================================================

//============================================================================
// �v���C���[������
//============================================================================
CPlayer* const CEnemy::FindPlayer() const
{
	// �v���C���[�^�C�v�̃I�u�W�F�N�g������
	CObject* const pObj = CObject::FindSpecificObject(CObject::TYPE::PLAYER);

	// �I�u�W�F�N�g�������炯��ΏI��
	if (pObj == nullptr)
		return nullptr;

	// �v���C���[�N���X�Ƀ_�E���L���X�g
	CPlayer* const pPlayer = utility::DownCast<CPlayer, CObject>(pObj);

	return pPlayer;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ������␳
//============================================================================
void CEnemy::AdjustHeight()
{
	// �ڕW���W��Y���̉����x���ϓ�����
	Vec3 NewPosTarget = GetPosTarget();
	NewPosTarget.y += GetVelY();
	SetPosTarget(NewPosTarget);

	// �����̖ڕW�ɉ�����ݒ�
	if (GetPosTarget().y < m_pBndCylinder->GetHeight())
	{
		// �ڕW���W�������ɌŒ�
		NewPosTarget.y = m_pBndCylinder->GetHeight();
		SetPosTarget(NewPosTarget);

		// Y�������̉����x��������
		SetVelY(0.0f);
	}
}

//============================================================================
// �o�E���f�B���O�T�C�Y�̐ݒ�
//============================================================================
void CEnemy::SetBndSize(float fRadius, float fHeight)
{
	// ���a�̐ݒ�
	m_pBndCylinder->SetRadius(fRadius);

	// �����̐ݒ�
	m_pBndCylinder->SetHeight(fHeight);
}

//============================================================================
// �Փˌ��o
//============================================================================
bool CEnemy::HitCheck()
{
	// �v���C���[���擾
	CPlayer* const pPlayer = FindPlayer();

	// �v���C���[�̎��~���͈͓��ɐN�����Ă�����Փ�
	if (collision::HitCylinderToCylinder(m_pBndCylinder, pPlayer->GetBndCylinder()))
	{
		// ���_����L�����N�^�[�̂���ꏊ�ւ̕����x�N�g�����쐬
		Vec3 CharacterPosVec = GetPos() - VEC3_INIT;
		D3DXVec3Normalize(&CharacterPosVec, &CharacterPosVec);

		// ���_����v���C���[�̂���ꏊ�ւ̃x�N�g�����쐬
		Vec3 PlayerVec = pPlayer->GetPos() - VEC3_INIT;
		D3DXVec3Normalize(&PlayerVec, &PlayerVec);

		// 2�{�̕����x�N�g���̊O�ς��쐬
		float fCross = (CharacterPosVec.x * PlayerVec.z) - (CharacterPosVec.z * PlayerVec.x);
		
		// ���a�ɉ����ďՌ��ʂ��쐬
		float fImpact = GetRadius() * 0.01f;

		// ���݂��̈ʒu�֌W�ɍ��킹�Đ�����΂����p��ύX
		if (fCross < 0.0f) fImpact *= -1.0f;

		// 1�_���[�W�ƏՌ��͂�^����
		pPlayer->SetDamage(-1, fImpact);

		// �Փ˂���
		return true;
	}

	// �Փ˂��Ă��Ȃ�
	return false;
}