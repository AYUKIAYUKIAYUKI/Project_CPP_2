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
// �f�t�H���g�R���X�g���N�^
//============================================================================
CBlock::CBlock() :
	CObject_X{ LAYER::MIDDLE },
	m_pBndBox{ DBG_NEW CBounding_Box(this) }
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

#if 1
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
	// �����ɂ���
	Vec3 NewRot = GetRot();
	NewRot.y += D3DX_PI * 0.005f;
	SetRot(NewRot);
#endif

	// ���N���X�̍X�V����
	CObject_X::Update();

#if 0
#ifdef _DEBUG

	CRenderer::SetDebugString("��������������������������������������������������������������");
	CRenderer::SetDebugString("��T�C�Y�@ : " + to_string(GetSize().x) + " :  " + to_string(GetSize().y) + " : " + to_string(GetSize().z));
	CRenderer::SetDebugString("������@�@ : " + to_string(GetRot().x * (180 / D3DX_PI)) + " :  " + to_string(GetRot().y * (180 / D3DX_PI)) + " : " + to_string(GetRot().z * (180 / D3DX_PI)));
	CRenderer::SetDebugString("��Ɓ@�@�@�@: " + to_string(GetRot().y));
	CRenderer::SetDebugString("����W�@�@ : " + to_string(GetPos().x) + " :  " + to_string(GetPos().y) + " : " + to_string(GetPos().z));
	CRenderer::SetDebugString("��������������������������������������������������������������");

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
	pNewInstance->BindModel(CModel_X_Manager::TYPE::TEST2);

	// �T�C�Y��ݒ�
	pNewInstance->m_pBndBox->SetSize(pNewInstance->GetModel()->Size);

	// �`��O��1�x�X�V
	pNewInstance->Update();

	return pNewInstance;
}