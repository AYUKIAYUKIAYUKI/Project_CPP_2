//============================================================================
// 
// ���ꂫ�G�t�F�N�g [rubble.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "rubble.h"
#include "utility.h"

// �f�o�C�X�擾�p
#include "renderer.h"

//============================================================================
// �R���X�g���N�^
//============================================================================
CRubble::CRubble() :
	CObject_X{ static_cast<int>(LAYER::MIDDLE) },
	m_Velocity{ 0.0f, 0.0f, 0.0f }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CRubble::~CRubble()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CRubble::Init()
{
	// ���N���X�̏����ݒ�
	if (FAILED(CObject_X::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CRubble::Uninit()
{
	// ���N���X�̏I������
	CObject_X::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CRubble::Update()
{
	// ���N���X�̍X�V����
	CObject_X::Update();

	// ��]
	D3DXVECTOR3 NewRot{ GetRot() };
	NewRot = m_Velocity;
	SetRot(NewRot);

	// �d�͉���
	m_Velocity.y += -0.15f;

	// �ړ�
	D3DXVECTOR3 NewPos = GetPos();
	NewPos += m_Velocity;
	SetPos(NewPos);

	// ���ł��邩�`�F�b�N
	CheckDisappear();
}

//============================================================================
// �`�揈��
//============================================================================
void CRubble::Draw()
{
	// ���N���X�̕`�揈��
	CObject_X::Draw();
}

//============================================================================
// �����x���擾
//============================================================================
D3DXVECTOR3& CRubble::GetVelocity()
{
	return m_Velocity;
}

//============================================================================
// �����x��ݒ�
//============================================================================
void CRubble::SetVelocity(D3DXVECTOR3 Velocity)
{
	m_Velocity = Velocity;
}

//============================================================================
// ����
//============================================================================
CRubble* CRubble::Create(int nType, D3DXVECTOR3 Pos, D3DXVECTOR3 Velocity)
{
	// �x���}���p
	nType;

	CRubble* pRubble = DBG_NEW CRubble{};

	// �������s
	if (!pRubble)
	{
		assert(false);
	}

	// �^�C�v��ݒ�
	pRubble->SetType(CObject::TYPE::NONE);

	// �����ݒ�
	pRubble->Init();

	// ���W�̐ݒ�
	pRubble->SetPos(Pos);

	// �����x��ݒ�
	pRubble->SetVelocity(Velocity);

	// ���f���p�|�C���^
	CModel_X_Manager::MODEL* Model{ nullptr };
	
	// ���f�����擾
	switch (rand() % 2)
	{
	case 0:
		Model = CModel_X_Manager::GetInstance()->GetModel(CModel_X_Manager::TYPE::RUBBLE_00);
		break;

	case 1:
		Model = CModel_X_Manager::GetInstance()->GetModel(CModel_X_Manager::TYPE::RUBBLE_01);
		break;

	default:		
		Model = CModel_X_Manager::GetInstance()->GetModel(CModel_X_Manager::TYPE::RUBBLE_00);
		break;
	}

	// ���f����ݒ�
	pRubble->BindModel(Model);

	// �T�C�Y��ݒ�
	pRubble->SetSize(Model->Size);

	// �`�悳���O�Ɉ�x�X�V���Ă���
	pRubble->Update();

	return pRubble;
}

//============================================================================
// ���ł��邩�`�F�b�N
//============================================================================
void CRubble::CheckDisappear() 
{
	// �o���A�̍ł��Ⴂ���W�����������
	if (GetPos().y < -30.0f)
	{
		// �j��
		SetRelease();
	}
}