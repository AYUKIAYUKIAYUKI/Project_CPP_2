//============================================================================
// 
// �{�X [boss.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "boss.h"
#include "bounding_cylinder.h"
#include "object_parts.h"

#include "field_manager.h"
#include "manager.h"
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
CBoss::CBoss() :
	CCharacter{},
	m_pBndCylinder{ DBG_NEW CBounding_Cylinder() }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CBoss::~CBoss()
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
HRESULT CBoss::Init()
{
	// �␳���x��ݒ�
	SetCorrectCoef(CORRECT_COEF);

	// �������p��ݒ�
	SetDirection(D3DX_PI * -0.5f);
	SetDirectionTarget(D3DX_PI * -0.5f);

	// �����̗͂�ݒ�
	SetLife(MAX_LIFE);

	// �L�����N�^�[�N���X�̏����ݒ�
	if (FAILED(CCharacter::Init()))
	{
		assert(false && "�{�X�N���X�̏����ݒ�Ɏ��s");
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CBoss::Uninit()
{
	// �L�����N�^�[�N���X�̏I������
	CCharacter::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CBoss::Update()
{
	// �~���o�E���f�B���O�̒��S�_�����f���̒��S��
	m_pBndCylinder->SetCenterPos(GetParentParts()->GetPos());

	// �L�����N�^�[�N���X�̍X�V����
	CCharacter::Update();

#if 1
#ifdef _DEBUG
	// �E�B���h�E��\��
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Boss Param"))
	{
		ImGui::Text("Direction:%.3f", GetDirection() * (180 / D3DX_PI));
		ImGui::Text("DirectionTarget:%.3f", GetDirectionTarget() * (180 / D3DX_PI));
		ImGui::Text("Rot:X %.1f:Y %.1f:Z %.1f", GetRot().x * (180 / D3DX_PI), GetRot().y * (180 / D3DX_PI), GetRot().z * (180 / D3DX_PI));
		ImGui::Text("RotTarget:X %.1f:Y %.1f:Z %.1f", GetRotTarget().x * (180 / D3DX_PI), GetRotTarget().y * (180 / D3DX_PI), GetRotTarget().z * (180 / D3DX_PI));
		ImGui::Text("Pos:X %.1f:Y %.1f:Z %.1f", GetPos().x, GetPos().y, GetPos().z);
		ImGui::Text("PosTarget:X %.1f:Y %.1f:Z %.1f", GetPosTarget().x, GetPosTarget().y, GetPosTarget().z);
		ImGui::Text("VelY:%.1f", GetVelY());
		ImGui::Text("Life:%df", GetLife());
	}
	ImGui::End();
#endif	// _DEBUG
#endif
}

//============================================================================
// �`�揈��
//============================================================================
void CBoss::Draw()
{
	// �L�����N�^�[�N���X�̕`�揈��
	CCharacter::Draw();
}

//============================================================================
// �_���[�W���󂯂�
//============================================================================
void CBoss::SetDamage(int nDamage)
{
	// �_���[�W�ʕ��A�̗͂�ϓ�
	int nNewLife = GetLife();
	nNewLife += nDamage;
	SetLife(nNewLife);
}

//============================================================================
// ���a���擾
//============================================================================
float CBoss::GetRadius() const
{
	return m_pBndCylinder->GetRadius();
}

//============================================================================
// �������擾
//============================================================================
float CBoss::GetHeight() const
{
	return m_pBndCylinder->GetHeight();
}

//============================================================================
// �o�E���f�B���O�V�����_�[���擾
//============================================================================
const CBounding_Cylinder* const CBoss::GetBndCylinder() const
{
	return m_pBndCylinder;
}

//============================================================================
// ����
//============================================================================
CBoss* CBoss::Create()
{
	// �C���X�^���X�𐶐�
	CBoss* pNewInstance = DBG_NEW CBoss();

	// �^�C�v��ݒ�
	pNewInstance->SetType(TYPE::BOSS);

	// �����ݒ�
	pNewInstance->Init();

	// ���[�V�������Z�b�g
	pNewInstance->SetMotion(utility::OpenJsonFile("Data\\JSON\\CHARACTER\\pumpkin_motion.json"));

	// ���a��ݒ�
	pNewInstance->m_pBndCylinder->SetRadius(50.0f);

	// ������ݒ�
	pNewInstance->m_pBndCylinder->SetHeight(50.0f);

	return pNewInstance;
}