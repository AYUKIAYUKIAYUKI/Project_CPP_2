//============================================================================
// 
// HUD�I�u�W�F�N�g [object_HUD.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_HUD.h"

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
// �`��D��x�w��R���X�g���N�^
//============================================================================
CObject_HUD::CObject_HUD(int nPriority) :
	CObject_2D{ nPriority },
	m_PosTarget{ VEC3_INIT },
	m_RotTarget{ VEC3_INIT },
	m_SizeTarget{ VEC3_INIT },
	m_ColTarget{ XCOL_INIT }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CObject_HUD::~CObject_HUD()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CObject_HUD::Init()
{
	// 2D�I�u�W�F�N�g�̏����ݒ�
	if (FAILED(CObject_2D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CObject_HUD::Uninit()
{
	// 2D�I�u�W�F�N�g�̏I������
	CObject_2D::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CObject_HUD::Update()
{
	// �ڕW�l�ւ̕␳
	AdjustToTarget();

	// 2D�I�u�W�F�N�g�̍X�V����
	CObject_2D::Update();
}

//============================================================================
// �`�揈��
//============================================================================
void CObject_HUD::Draw()
{
	// 2D�I�u�W�F�N�g�̕`�揈��
	CObject_2D::Draw();
}

//============================================================================
// �ڕW���W�擾
//============================================================================
const D3DXVECTOR3& CObject_HUD::GetPosTarget() const
{
	return m_PosTarget;
}

//============================================================================
// �ڕW���W�ݒ�
//============================================================================
void CObject_HUD::SetPosTarget(D3DXVECTOR3 PosTarget)
{
	m_PosTarget = PosTarget;
}

//============================================================================
// �ڕW�����擾
//============================================================================
const D3DXVECTOR3& CObject_HUD::GetRotTarget() const
{
	return m_RotTarget;
}

//============================================================================
// �ڕW�����ݒ�
//============================================================================
void CObject_HUD::SetRotTarget(D3DXVECTOR3 RotTarget)
{
	m_RotTarget = RotTarget;
}

//============================================================================
// �ڕW�T�C�Y�擾
//============================================================================
const D3DXVECTOR3& CObject_HUD::GetSizeTarget() const
{
	return m_SizeTarget;
}

//============================================================================
// �ڕW�T�C�Y�ݒ�
//============================================================================
void CObject_HUD::SetSizeTarget(D3DXVECTOR3 SizeTarget)
{
	m_SizeTarget = SizeTarget;
}

//============================================================================
// �ڕW�F�擾
//============================================================================
const D3DXCOLOR& CObject_HUD::GetColTarget() const
{
	return m_ColTarget;
}

//============================================================================
// �ڕW�F�ݒ�
//============================================================================
void CObject_HUD::SetColTarget(D3DXCOLOR ColTarget)
{
	m_ColTarget = ColTarget;
}

//============================================================================
// ����
//============================================================================
CObject_HUD* CObject_HUD::Create()
{
	// �C���X�^���X�𐶐�
	CObject_HUD* pNewInstance = DBG_NEW CObject_HUD();

	// �����o���Ă����珉���ݒ�
	if (pNewInstance == nullptr)
	{
		assert(false && "HUD�I�u�W�F�N�g�̐����Ɏ��s");
	}

	// �����ݒ�
	pNewInstance->Init();

	return pNewInstance;
}

//============================================================================
// ����
//============================================================================
CObject_HUD* CObject_HUD::Create(std::string FilePath)
{
	// �C���X�^���X�𐶐�
	CObject_HUD* pNewInstance = DBG_NEW CObject_HUD();

	// �����o���Ă����珉���ݒ�
	if (pNewInstance == nullptr)
	{
		assert(false && "HUD�I�u�W�F�N�g�̐����Ɏ��s");
	}

	// �����ݒ�
	pNewInstance->Init();

	// JSON�t�@�C����ǂݎ��W�J
	std::ifstream ifs(FilePath);

	// �t�@�C�����W�J�o���Ă�����
	if (ifs.good())
	{
		// JSON�f�[�^���p�[�X
		JSON Json;
		ifs >> Json;

		// �������W��ݒ�
		auto Pos = Json["Pos"];
		pNewInstance->SetPos(Vec3(Pos[0], Pos[1], Pos[2]));

		// �ڕW���W��ݒ�
		auto PosTarget = Json["PosTarget"];
		pNewInstance->SetPosTarget(Vec3(PosTarget[0], PosTarget[1], PosTarget[2]));

		// ����������ݒ�
		auto Rot = Json["Rot"];
		pNewInstance->SetRot(Vec3(Rot[0], Rot[1], Rot[2]));

		// �ڕW������ݒ�
		auto RotTarget = Json["RotTarget"];
		pNewInstance->SetRotTarget(Vec3(RotTarget[0], RotTarget[1], RotTarget[2]));

		// �����T�C�Y��ݒ�
		auto Size = Json["Size"];
		pNewInstance->SetSize(Vec3(Size[0], Size[1], Size[2]));

		// �ڕW�T�C�Y��ݒ�
		auto SizeTarget = Json["SizeTarget"];
		pNewInstance->SetSizeTarget(Vec3(SizeTarget[0], SizeTarget[1], SizeTarget[2]));

		// �����F��ݒ�
		auto Col = Json["Col"];
		pNewInstance->SetCol(XCol(Col[0], Col[1], Col[2], Col[3]));

		// �ڕW�F��ݒ�
		auto ColTarget = Json["ColTarget"];
		pNewInstance->SetColTarget(XCol(ColTarget[0], ColTarget[1], ColTarget[2], ColTarget[3]));
	}
	else
	{
		assert(false && (*FilePath.c_str() + "�̓ǂݎ��Ɏ��s���܂���"));
	}

	return pNewInstance;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �ڕW�l�ւ̕␳
//============================================================================
void CObject_HUD::AdjustToTarget()
{
	// �ڕW���W�ֈړ�
	Vec3 NowPos = GetPos();
	NowPos += (m_PosTarget - NowPos) * COEF_ADJUST;
	SetPos(NowPos);

	// �ڕW�����֕␳
	Vec3 NowRot = GetRot();
	CUtility::AdjustDirection(m_RotTarget.y, NowRot.y);	// �����͈̔͂̕␳
	NowRot += (m_RotTarget - NowRot) * COEF_ADJUST;
	SetRot(NowRot);

	// �ڕW�T�C�Y�֕␳
	Vec3 NowSize = GetSize();
	NowSize += (m_SizeTarget - NowSize) * COEF_ADJUST;
	SetSize(NowSize);

	// �ڕW�F�␳
	XCol NowCol = GetCol();
	NowCol += (m_ColTarget - NowCol) * COEF_ADJUST;
	SetCol(NowCol);
}