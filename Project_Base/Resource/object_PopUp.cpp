//============================================================================
// 
// �|�b�v�A�b�v�I�u�W�F�N�g [object_PopUp.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_PopUp.h"
#include "object_TextMesh.h"
#include "renderer.h"
#include "manager.h"

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
CObject_PopUp::CObject_PopUp(LAYER Priority) :
	CObject_3D{ Priority },
	m_fCorrectCoef{ 0.0f },
	m_PosTarget{ VEC3_INIT },
	m_RotTarget{ VEC3_INIT },
	m_SizeTarget{ VEC3_INIT },
	m_ColTarget{ XCOL_INIT },
	m_pTextMesh{ nullptr }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CObject_PopUp::~CObject_PopUp()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CObject_PopUp::Init()
{
	// 3D�I�u�W�F�N�g�̏����ݒ�
	if (FAILED(CObject_3D::Init()))
	{
		return E_FAIL;
	}

	// �^�C�v����
	SetType(TYPE::NONE);

	// �|�b�v�A�b�v�e�N�X�`��
	BindTex(CTexture_Manager::TYPE::POPUP);

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CObject_PopUp::Uninit()
{
	// 3D�I�u�W�F�N�g�̏I������
	CObject_3D::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CObject_PopUp::Update()
{
	if (CManager::GetKeyboard()->GetTrigger(DIK_M))
		Disappear();

	// �ڕW�l�ւ̕␳
	CorrectToTarget();

	// 3D�I�u�W�F�N�g�̍X�V����
	CObject_3D::Update();
}

//============================================================================
// �`�揈��
//============================================================================
void CObject_PopUp::Draw()
{
	// 3D�I�u�W�F�N�g�̕`�揈��
	CObject_3D::Draw();
}

//============================================================================
// ����
//============================================================================
void CObject_PopUp::Disappear()
{
	// ���̏�ŏ���
	m_SizeTarget = VEC3_INIT;
	m_ColTarget = XCOL_INIT;
}

//============================================================================
// �S�ڕW�l���Z�b�g
//============================================================================
void CObject_PopUp::AllTargetReset()
{
	// �S�Ă̖ڕW�l�����Z�b�g
	m_fCorrectCoef *= 0.5f;
	m_SizeTarget = VEC3_INIT;
	m_RotTarget = VEC3_INIT;
	m_PosTarget = VEC3_INIT;
	m_ColTarget = XCOL_INIT;
}

//============================================================================
// ��ԋ��x��ݒ�
//============================================================================
void CObject_PopUp::SetCorrectCoef(float fCoef)
{
	m_fCorrectCoef = fCoef;
}

//============================================================================
// �ڕW���W�擾
//============================================================================
const D3DXVECTOR3& CObject_PopUp::GetPosTarget() const
{
	return m_PosTarget;
}

//============================================================================
// �ڕW���W�ݒ�
//============================================================================
void CObject_PopUp::SetPosTarget(D3DXVECTOR3 PosTarget)
{
	m_PosTarget = PosTarget;
}

//============================================================================
// �ڕW�����擾
//============================================================================
const D3DXVECTOR3& CObject_PopUp::GetRotTarget() const
{
	return m_RotTarget;
}

//============================================================================
// �ڕW�����ݒ�
//============================================================================
void CObject_PopUp::SetRotTarget(D3DXVECTOR3 RotTarget)
{
	m_RotTarget = RotTarget;
}

//============================================================================
// �ڕW�T�C�Y�擾
//============================================================================
const D3DXVECTOR3& CObject_PopUp::GetSizeTarget() const
{
	return m_SizeTarget;
}

//============================================================================
// �ڕW�T�C�Y�ݒ�
//============================================================================
void CObject_PopUp::SetSizeTarget(D3DXVECTOR3 SizeTarget)
{
	m_SizeTarget = SizeTarget;
}

//============================================================================
// �ڕW�F�擾
//============================================================================
const D3DXCOLOR& CObject_PopUp::GetColTarget() const
{
	return m_ColTarget;
}

//============================================================================
// �ڕW�F�ݒ�
//============================================================================
void CObject_PopUp::SetColTarget(D3DXCOLOR ColTarget)
{
	m_ColTarget = ColTarget;
}

//============================================================================
// ����
//============================================================================
CObject_PopUp* CObject_PopUp::Create(JSON Json)
{
	// �C���X�^���X�𐶐�
	CObject_PopUp* pNewInstance = DBG_NEW CObject_PopUp();

	// �������s
	if (pNewInstance == nullptr)
	{
		assert(false && "�|�b�v�A�b�v�I�u�W�F�N�g�̐����Ɏ��s");
	}

	// �����ݒ�
	pNewInstance->Init();

	{ // �p�����[�^��ݒ�

		// �f�[�^���L���X�g
		float
			fCorrectCoef = static_cast<float>(Json["CorrectionCoef"]);
		Vec3
			Size = utility::JsonConvertToVec3(Json["Size"]),
			SizeTarget = utility::JsonConvertToVec3(Json["SizeTarget"]),
			Rot = utility::JsonConvertToVec3(Json["Rot"]),
			RotTarget = utility::JsonConvertToVec3(Json["RotTarget"]),
			Pos = utility::JsonConvertToVec3(Json["Pos"]),
			PosTarget = utility::JsonConvertToVec3(Json["PosTarget"]);
		XCol
			Col = utility::JsonConvertToXCol(Json["Col"]),
			ColTarget = utility::JsonConvertToXCol(Json["ColTarget"]);

		// �f�[�^���Z�b�g
		pNewInstance->SetCorrectCoef(fCorrectCoef);	// ��ԋ��x
		pNewInstance->SetSize(Size);				// �T�C�Y
		pNewInstance->SetSizeTarget(SizeTarget);	// �ڕW�T�C�Y
		pNewInstance->SetRot(Rot);					// ����
		pNewInstance->SetRotTarget(RotTarget);		// �ڕW����
		pNewInstance->SetPos(Pos);					// ���W
		pNewInstance->SetPosTarget(PosTarget);		// �ڕW���W
		pNewInstance->SetCol(Col);					// �F
		pNewInstance->SetColTarget(ColTarget);		// �ڕW�F
	}

	// �e�L�X�g���b�V���p�f�[�^�̃p�X���R�s�[
	std::string FileName = Json["TextMeshFile"];

	// �e�L�X�g���b�V�����쐬
	pNewInstance->m_pTextMesh = CObject_TextMesh::Create(utility::OpenJsonFile(FileName));

	return pNewInstance;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �ڕW�l�ւ̕��
//============================================================================
void CObject_PopUp::CorrectToTarget()
{
	// �ڕW�T�C�Y�֕␳
	Vec3 NowSize = GetSize();
	NowSize += (m_SizeTarget - NowSize) * m_fCorrectCoef;
	SetSize(NowSize);

	// �ڕW�����֕␳
	Vec3 NowRot = GetRot();
	utility::AdjustDirection(m_RotTarget.y, NowRot.y);	// �����͈̔͂̕␳
	NowRot += (m_RotTarget - NowRot) * m_fCorrectCoef;
	SetRot(NowRot);

	// �ڕW���W�ֈړ�
	Vec3 NowPos = GetPos();
	NowPos += (m_PosTarget - NowPos) * m_fCorrectCoef;
	SetPos(NowPos);

	// �ڕW�F�␳
	XCol NowCol = GetCol();
	NowCol += (m_ColTarget - NowCol) * m_fCorrectCoef;
	SetCol(NowCol);

	// ����ɂ��̎��A�e�L�X�g���b�V���ɂ����l�̃p�����[�^��^����
	m_pTextMesh->SetMeshSize(NowSize);
	m_pTextMesh->SetRot(NowRot);
	NowPos.x *= 0.975f;
	NowPos.z *= 0.975f;
	m_pTextMesh->SetPos(NowPos);
	m_pTextMesh->SetCol(NowCol);
}