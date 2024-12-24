//============================================================================
// 
// �A�C�e�� [item.cpp]
// Author : ���c����
// 
//============================================================================

/* ����̓X�e���V���o�b�t�@�̕`��e�X�g�p */
namespace
{
	int
	ZFuncCmp = D3DCMP_LESSEQUAL,	// �f�v�X�X�e���V���l�̔�r���@
	StencilRefValue = 0,			// �X�e���V���Q�ƒl
	StencilCmp = D3DCMP_EQUAL,		// �X�e���V���l�̔�r���@
	Pass = D3DSTENCILOP_KEEP,		// Z�e�X�g�E�X�e���V���e�X�g�ɐ���
	ZFail = D3DSTENCILOP_INCRSAT,	// Z�e�X�g�̂ݎ��s
	Fail = D3DSTENCILOP_KEEP;		// Z�e�X�g�E�X�e���V���e�X�g�Ɏ��s
}

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "item.h"
#include "summoning.h"
#include "bounding_sphere.h"
#include "sound.h"
#include "field_manager.h"
#include "player.h"
#include "collision.h"

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
CItem::CItem() :
	CObject_X{ LAYER::STENCIL },
	m_pBndSphere{ DBG_NEW CBounding_Sphere() },
	m_fCorrectCoef{ 0.0f },
	m_fDirection{ 0.0f },
	m_RotTarget{ VEC3_INIT },
	m_PosTarget{ VEC3_INIT },
	m_pSummoning{ CSummoning::Create() }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CItem::~CItem()
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
HRESULT CItem::Init()
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
void CItem::Uninit()
{
	// X�I�u�W�F�N�g�N���X�̏I������
	CObject_X::Uninit();

	// ���@�w�����Ń��[�V�����ɕύX
	m_pSummoning->SetNowMotion(1);
}

//============================================================================
// �X�V����
//============================================================================
void CItem::Update()
{
	// ���邭����
	AppealRotate();

	// �ڕW�l�ւ̕��
	CorrectToTarget();

	// ���o�E���f�B���O�ɒ��S�_�Z�b�g
	m_pBndSphere->SetCenterPos(GetPos());

	// �v���C���[�Ƃ̐ڐG�����o
	HitChecklPlayer();

	// ���@�w���Z�b�g
	SetSummoning();

	// X�I�u�W�F�N�g�N���X�̍X�V����
	CObject_X::Update();

#if 1
#ifdef _DEBUG
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Item Param"))
	{
		ImGui::Text("Direction:%.1f", GetDirection());
		ImGui::Text("Size:X %.2f:Y %.2f:Z %.2f", GetSize().x, GetSize().y, GetSize().z);
		ImGui::Text("Rot:X %.2f:Y %.2f:Z %.2f", GetRot().x * (180 / D3DX_PI), GetRot().y * (180 / D3DX_PI), GetRot().z * (180 / D3DX_PI));
		ImGui::Text("Pos:X %.2f:Y %.2f:Z %.2f", GetPos().x, GetPos().y, GetPos().z);
		ImGui::End();
	}
#endif // _DEBUG
#endif

#ifdef _DEBUG
#if 0	/* �X�e���V���e�X�g�̐ݒ�ύX */
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Sstencil - Only Item"))
	{
		ImGui::InputInt("ZFuncCmp", &ZFuncCmp);
		ImGui::InputInt("StencilRefValue", &StencilRefValue);
		ImGui::InputInt("StencilCmp", &StencilCmp);
		ImGui::InputInt("Pass", &Pass);
		ImGui::InputInt("ZFail", &ZFail);
		ImGui::InputInt("Fail", &Fail);
		ImGui::End();
	}
#endif
#endif // _DEBUG
}

//============================================================================
// �`�揈��
//============================================================================
void CItem::Draw()
{
	// �f�o�C�X���擾
	auto pDev = CRenderer::GetDeviece();

	// �[�x�o�b�t�@�ւ̏������݂�L����
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// �X�e���V���Q�ƒl��ݒ�
	pDev->SetRenderState(D3DRS_STENCILREF, StencilRefValue);

	// �X�e���V���o�b�t�@�̔�r���@��ύX
	pDev->SetRenderState(D3DRS_STENCILFUNC, StencilCmp);

	// �X�e���V���e�X�g�̌��ʂɑ΂��Ă̂ӂ�܂���ݒ肷��
	pDev->SetRenderState(D3DRS_STENCILPASS, Pass);		// Z�e�X�g�E�X�e���V���e�X�g�ɐ���
	pDev->SetRenderState(D3DRS_STENCILZFAIL, ZFail);	// Z�e�X�g�̂ݎ��s
	pDev->SetRenderState(D3DRS_STENCILFAIL, Fail);		// Z�e�X�g�E�X�e���V���e�X�g�Ɏ��s

	// X�I�u�W�F�N�g�N���X�̕`�揈��
	CObject_X::Draw();

	// �[�x�o�b�t�@�ւ̏������݂�L����
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//============================================================================
// ���a���擾
//============================================================================
float CItem::GetRadius() const
{
	return m_pBndSphere->GetRadius();
}

//============================================================================
// ���a��ݒ�
//============================================================================
void CItem::SetRadius(float fRadius)
{
	m_pBndSphere->SetRadius(fRadius);
}

//============================================================================
// ��ԋ��x��ݒ�
//============================================================================
void CItem::SetCorrectCoef(float fCoef)
{
	m_fCorrectCoef = fCoef;
}

//============================================================================
// ���p���擾
//============================================================================
const float& CItem::GetDirection() const
{
	return m_fDirection;
}

//============================================================================
// ���p��ݒ�
//============================================================================
void CItem::SetDirection(float fDirection)
{
	m_fDirection = fDirection;

	// ���p������W��ݒ肵
	Vec3 NewPos = GetPos();
	NewPos.x = cosf(fDirection) * CField_Manager::FIELD_RADIUS;
	NewPos.z = sinf(fDirection) * CField_Manager::FIELD_RADIUS;
	SetPos(NewPos);

	// �ڕW���W�������ɐݒ�
	m_PosTarget = NewPos;
}

//============================================================================
// �ڕW�������擾
//============================================================================
const D3DXVECTOR3& CItem::GetRotTarget() const
{
	return m_RotTarget;
}

//============================================================================
// �ڕW������ݒ�
//============================================================================
void CItem::SetRotTarget(D3DXVECTOR3 RotTarget)
{
	m_RotTarget = RotTarget;
}

//============================================================================
// �ڕW���W���擾
//============================================================================
const D3DXVECTOR3& CItem::GetPosTarget() const
{
	return m_PosTarget;
}

//============================================================================
// �ڕW���W��ݒ�
//============================================================================
void CItem::SetPosTarget(D3DXVECTOR3 PosTarget)
{
	m_PosTarget = PosTarget;
}

//============================================================================
// Y���W��ݒ�
//============================================================================
void CItem::SetPosY(float fPosY)
{
	// ���W��ݒ肵
	Vec3 NewPos = GetPos();
	NewPos.y = fPosY;
	SetPos(NewPos);

	// �ڕW���W�������ɐݒ�
	m_PosTarget.y = fPosY;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ���@�w���Z�b�g
//============================================================================
void CItem::SetSummoning()
{
	// ���W���R�s�[
	Vec3 Pos = GetPos();

	// ���W�ɍ��킹�Č������Z�b�g
	Vec3 Rot = { 0.0f, atan2f(Pos.z, -Pos.x) + D3DX_PI * 0.5f, 0.0f };
	m_pSummoning->SetRot(Rot);

#if 0
	// ���W���A�C�e���̔w���
	Pos.x *= 0.95f;
	Pos.z *= 0.95f;
#endif

	// ���W���Z�b�g
	m_pSummoning->SetPos(Pos);
}

//============================================================================
// �v���C���[�Ƃ̐ڐG�����o
//============================================================================
void CItem::HitChecklPlayer()
{
	// �v���C���[�^�C�v�̃I�u�W�F�N�g��T��
	CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::PLAYER);

	// ������Ȃ���Ώ������s��Ȃ�
	if (!pObj)
		return;

	// �I�u�W�F�N�g���v���C���[�N���X�ɃL���X�g
	CPlayer* pPlayer = utility::DownCast<CPlayer, CObject>(pObj);

	// �v���C���[�̉~���o�E���f�B���O�Ƃ̓����蔻��
	if (collision::HitCylinderToSphere(pPlayer->GetBndCylinder(), m_pBndSphere))
	{ // �ڐG���Ă�����

		// �擾���̌��ʂ𔭓�
		GainEffect();

		// ���g��j���\��
		SetRelease();

		// �A�C�e���擾����炷
		CSound::GetInstance()->Play(CSound::LABEL::HEAL);
	}
}

//============================================================================
// ���邭����
//============================================================================
void CItem::AppealRotate()
{
	// ������Y����]
	m_RotTarget.y += 0.05f;

	// �ȈՓI�Ȍ�������
	if (m_RotTarget.y > D3DX_PI * 2.0f)
		m_RotTarget.y += D3DX_PI * -2.0f;

	// �኱�㉺�ɗh��ĕ��V�����o��
	Vec3 NewPos = GetPos();
	NewPos.y += cosf(m_RotTarget.y) * 0.5f;
	SetPos(NewPos);
}

//============================================================================
// �ڕW�l�ւ̕��
//============================================================================
void CItem::CorrectToTarget()
{
	// �ڕW�����֌�����
	Vec3 NewRot = GetRot();
	utility::AdjustAngle(NewRot.y, m_RotTarget.y);	// �p�x�̍���␳
	NewRot += (m_RotTarget - NewRot) * 0.1f;
	SetRot(NewRot);

	// �ڕW���W�ֈړ�
	Vec3 NewPos = GetPos();
	NewPos += (m_PosTarget - NewPos) * m_fCorrectCoef;
	SetPos(NewPos);
}