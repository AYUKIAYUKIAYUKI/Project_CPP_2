//============================================================================
// 
// ���@�w [summoning.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "summoning.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;

//****************************************************
// �ÓI�����o�ϐ��̏�����
//****************************************************

// ���[�V�����f�[�^�̓W�J
const JSON CSummoning::m_MotionData = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\SUMMONING\\summoning_motion.json");

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CSummoning::CSummoning(LAYER Layer) :
	CMotion_Set{ Layer }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CSummoning::~CSummoning()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CSummoning::Init()
{
	// ���[�V�����Z�b�g�N���X�̏����ݒ�
	if (FAILED(CMotion_Set::Init()))
	{
		assert(false && "���@�w�N���X�̏����ݒ�Ɏ��s");
	}

	// �o�����[�V�������Z�b�g
	SetNowMotion(2);

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CSummoning::Uninit()
{
	// ���[�V�����Z�b�g�N���X�̏I������
	CMotion_Set::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CSummoning::Update()
{
	// ���Ŕ���
	if (Disappear())
		return;	// ���Ŏ��͍X�V���Ȃ�

	// �o�����[�V�������I�����Ă�����
	if (GetStopState() && GetNowMotion() == 2)
	{
		// �ʏ탂�[�V�����ɕύX
		SetNowMotion(0);
	}

	// ���[�V�����Z�b�g�N���X�̍X�V
	CMotion_Set::Update();
}

//============================================================================
// �`�揈��
//============================================================================
void CSummoning::Draw()
{

}

//============================================================================
// ����
//============================================================================
CSummoning* CSummoning::Create()
{
	// �C���X�^���X�𐶐�
	CSummoning* pNewInstance = DBG_NEW CSummoning(LAYER::DEFAULT);

	// �^�C�v��ݒ�
	pNewInstance->CObject::SetType(TYPE::NONE);

	// �����ݒ�
	pNewInstance->Init();

	// ���[�V�������Z�b�g
	pNewInstance->CMotion_Set::SetMotion(LAYER::DEFAULT, m_MotionData);

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
bool CSummoning::Disappear()
{
	// ���Ń��[�V�����ŋ�����Ώ������Ȃ�
	if (GetNowMotion() != 1)
		return false;

	// ���Ń��[�V�����Đ����I��������
	if (GetStopState())
	{
		// �j���\��
		SetRelease();

		return true;
	}

	return false;
}