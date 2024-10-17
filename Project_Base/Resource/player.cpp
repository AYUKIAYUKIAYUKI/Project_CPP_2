//============================================================================
// 
// �v���C���[ [player.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "player.h"

// �C���v�b�g�擾
#include "manager.h"

// �f�o�b�O�\���p
#include "renderer.h"

//****************************************************
// �G�C���A�X���`
//****************************************************
using Vec = D3DXVECTOR3;

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CPlayer::CPlayer() :
	m_PosTarget{ D3DXVECTOR3(0.0f, 0.0f, 0.0f) }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CPlayer::~CPlayer()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CPlayer::Init()
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
void CPlayer::Uninit()
{
	// ���N���X�̏I������
	CObject_X::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CPlayer::Update()
{
	// ����
	Control();

	// ���N���X�̍X�V����
	CObject_X::Update();

#ifdef _DEBUG
	CRenderer::GetInstance()->SetDebugString("�v���C���[���W : " + std::to_string(GetPos().x) + " :  " + std::to_string(GetPos().y) + " : " + std::to_string(GetPos().z));
#endif // _DEBUG
}

//============================================================================
// �`�揈��
//============================================================================
void CPlayer::Draw()
{
	// ���N���X�̕`�揈��
	CObject_X::Draw();
}

//============================================================================
// �ڕW���W���擾
//============================================================================
Vec CPlayer::GetPosTarget() const
{
	return m_PosTarget;
}

//============================================================================
// �ڕW���W��ݒ�
//============================================================================
void CPlayer::SetPosTarget(Vec PosTarget)
{
	m_PosTarget = PosTarget;
}

//============================================================================
// ����
//============================================================================
CPlayer* CPlayer::Create()
{
	// �C���X�^���X�𐶐�
	CPlayer* pNewInstance = DBG_NEW CPlayer();

	// �^�C�v��ݒ�
	pNewInstance->SetType(TYPE::NONE);
	
	// �����ݒ�
	pNewInstance->Init();

	// ���f����ݒ�
	pNewInstance->BindModel(CModel_X_Manager::TYPE::SAMUS);

	return pNewInstance;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ����
//============================================================================
void CPlayer::Control()
{
	// �C���v�b�g�n�擾
	CInputKeyboard* pKeyboard = CManager::GetKeyboard();	// �L�[�{�[�h
	CInputPad* pPad = CManager::GetPad();					// �p�b�h

	// �ړ������p
	bool bMove = false;			// ���͂����邩���o
	float X = 0.0f, Y = 0.0f;	// �P�ʃx�N�g���p

	// �X�e�B�b�N�̌X�����擾
	X = pPad->GetJoyStickL().X;
	Y = pPad->GetJoyStickL().Y;

	/* ������ */
	static float f�p�x = 0.0f;
	CRenderer::GetInstance()->SetDebugString("�v���C���[���W�̊p�x" + std::to_string(f�p�x));

	float f������ = 0.025f;

	// X���̓���
	if (pKeyboard->GetPress(DIK_A) || pPad->GetPress(CInputPad::JOYKEY::LEFT))
	{
		X = -1.0f;
		f�p�x += -f������;
	}
	else if (pKeyboard->GetPress(DIK_D) || pPad->GetPress(CInputPad::JOYKEY::RIGHT))
	{
		X = 1.0f;
		f�p�x += f������;
	}

	// Y���̓���
	if (pKeyboard->GetPress(DIK_W) || pPad->GetPress(CInputPad::JOYKEY::UP))
	{
		Y = 1.0f;
	}
	else if (pKeyboard->GetPress(DIK_S) || pPad->GetPress(CInputPad::JOYKEY::DOWN))
	{
		Y = -1.0f;
	}

	// �������͂��Ă���Έړ�������o��
	if (X != 0.0f || Y != 0.0f)
	{
		bMove = true;
	}

	/* �p�x���� -> ���� */
	if (f�p�x > D3DX_PI)
	{
		f�p�x += -D3DX_PI * 2.0f;
	}
	else if (f�p�x < -D3DX_PI)
	{
		f�p�x += D3DX_PI * 2.0f;
	}

	//if (bMove)
	//{
	//	float f���f�� = 50.0f;

	//	// ���W�𔽉f
	//	Vec NewPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//	NewPos.x = cosf(f�p�x) * f���f��;
	//	NewPos.z = sinf(f�p�x) * f���f��;
	//	SetPos(NewPos);

	//	// �����𔽉f
	//	Vec NewRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//	NewRot.y = f�p�x * 0.5f;
	//	SetRot(NewRot);
	//}

	float f���f�� = 150.0f;

	// ���W�𔽉f
	Vec NewPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	NewPos.x = cosf(f�p�x) * f���f��;
	NewPos.z = sinf(f�p�x) * f���f��;
	SetPos(NewPos);

	// �����𔽉f
	Vec NewRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	NewRot.y = -f�p�x;
	SetRot(NewRot);
}