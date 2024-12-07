//============================================================================
// 
// �u���b�N [block.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "block.h"
#include "bounding_box.h"
#include "rubble.h"

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
CBlock::CBlock() :
	CObject_X{ LAYER::DEFAULT },
	m_pBndBox{ DBG_NEW CBounding_Box() }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CBlock::~CBlock()
{
	// �o�E���f�B���O�{�b�N�X�̔j��
	if (m_pBndBox != nullptr)
	{
		// �����������
		delete m_pBndBox;

		// �|�C���^��������
		m_pBndBox = nullptr;
	}

#if 0
	for (int i = 0; i < 2; i++)
	{
		// �����_���ȉ����x��ݒ� ( ���Y���ɕ΂���)
		D3DXVECTOR3 RandomVelocity{ utility::GetRandomValue<float>() * 0.01f, fabsf(utility::GetRandomValue<float>()) * 0.03f, -0.5f };

		// ���ꂫ�𐶐�
		CRubble::Create(0,						// �ʏ�
			GetPos() + RandomVelocity * 2.0f,	// ���W
			RandomVelocity * 0.5f);				// �����x
	}
#endif
}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CBlock::Init()
{
	// ���N���X�̏����ݒ�
	HRESULT hr = CObject_X::Init();

	if (FAILED(hr))
	{
		assert(false && "�v���C���[�N���X�̏����ݒ�Ɏ��s");
	}

	return hr;
}

//============================================================================
// �I������
//============================================================================
void CBlock::Uninit()
{
	// ���N���X�̏I������
	CObject_X::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CBlock::Update()
{
#if 0
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Block Param"))
	{
		static float fRotY = 0.0f;
		if (ImGui::Button("--"))
			fRotY += D3DX_PI * -0.1f;
		ImGui::SameLine();
		if (ImGui::Button("++"))
			fRotY += D3DX_PI * 0.1f;
		ImGui::SameLine();
		ImGui::SliderFloat("Add RotY", &fRotY, -D3DX_PI, D3DX_PI);
		Vec3 NewRot = GetRot();
		NewRot.y = fRotY;
		SetRot(NewRot);
		ImGui::End();
	}
#endif

	// ���o�E���f�B���O�ɒ��S�_�E�T�C�Y�E�������Z�b�g
	m_pBndBox->SetCenterPos(GetPos());
	m_pBndBox->SetSize(GetSize());
	m_pBndBox->SetRot(GetRot());

	// ���N���X�̍X�V����
	CObject_X::Update();

#if 0
#ifdef _DEBUG
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Block Param"))
	{
		ImGui::Text("Size::X %.2f:Y %.2f:Z %.2f", GetSize().x, GetSize().y, GetSize().z);
		ImGui::Text("Rot:X %.2f:Y %.2f:Z %.2f", GetRot().x * (180 / D3DX_PI), GetRot().y * (180 / D3DX_PI), GetRot().z * (180 / D3DX_PI));
		ImGui::Text("Pos:X %.2f:Y %.2f:Z %.2f", GetPos().x, GetPos().y, GetPos().z);
		ImGui::End();
	}
#endif // _DEBUG
#endif
}

//============================================================================
// �`�揈��
//============================================================================
void CBlock::Draw()
{
	// ���N���X�̕`�揈��
	CObject_X::Draw();
}

//============================================================================
// �T�C�Y���擾
//============================================================================
D3DXVECTOR3 CBlock::GetSize() const
{
	return m_pBndBox->GetSize();
}

//============================================================================
// ����
//============================================================================
CBlock* CBlock::Create(const D3DXVECTOR3& Pos, const D3DXVECTOR3& Rot)
{
	// �C���X�^���X�𐶐�
	CBlock* pNewInstance = DBG_NEW CBlock();

	// �^�C�v��ݒ�
	pNewInstance->SetType(TYPE::BLOCK);

	// �����ݒ�
	pNewInstance->Init();

	// ���W��ݒ�
	pNewInstance->SetPos(Pos);

	// ������ݒ�
	pNewInstance->SetRot(Rot);

	// ���f����ݒ�
	pNewInstance->BindModel(CX_Manager::TYPE::BLONORMAL + rand() % 3);

	// �T�C�Y��ݒ�
	pNewInstance->m_pBndBox->SetSize(pNewInstance->GetModel()->Size);

	// �`��O��1�x�X�V
	pNewInstance->Update();

	return pNewInstance;
}