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
#include "bounding_cylinder.h"
#include "player_state.h"
#include "player_state_default.h"
#include "player_state_dash.h"

#include "block.h"
#include "collision.h"
#include "field_manager.h"
#include "manager.h"
#include "mask_rectangle.h"
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
CPlayer::CPlayer() :
	CCharacter{},
	m_pBndCylinder{ DBG_NEW CBounding_Cylinder(this) },
	m_pState{ nullptr }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CPlayer::~CPlayer()
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
HRESULT CPlayer::Init()
{
	// �����X�e�[�g�̐���
	if (!m_pState)
	{
		m_pState = DBG_NEW CPlayer_State_Default();
	}
	
	// �␳���x��ݒ�
	SetCorrectCoef(CORRECT_COEF);

	// �������p��ݒ�
	SetDirection(D3DX_PI * -0.5f);

	// �����ړ����x��ݒ�
	SetMoveSpeed(DEFAULT_MOVE_SPEED);

	// �����̗͂�ݒ�
	SetLife(MAX_LIFE);

	// �L�����N�^�[�N���X�̏����ݒ�
	if (FAILED(CCharacter::Init()))
	{
		assert(false && "�v���C���[�N���X�̏����ݒ�Ɏ��s");
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CPlayer::Uninit()
{
	// �X�e�[�g�j��
	if (m_pState)
	{
		// �X�e�[�g�̔j��
		delete m_pState;
		
		// �|�C���^��������
		m_pState = nullptr;
	}

	// �L�����N�^�[�N���X�̏I������
	CCharacter::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CPlayer::Update()
{
	// �X�e�[�g�̕ύX�m�F
	m_pState = m_pState->CheckChangeState<CPlayer_State>();

	// �X�e�[�g�̍X�V
	m_pState->Update();

	// �����̕␳
	AdjustHeight();

	// �����ŖڕW�������ړ������Ɍ�����
	AutoSetRotTarget();

	// �����ŖڕW���W��ϓ��������p�ɍ��킹��
	AutoSetPosTarget();

	// �����蔻��
	HitCheck();

	// �L�����N�^�[�N���X�̍X�V����
	CCharacter::Update();

#if 1
#ifdef _DEBUG
	// �E�B���h�E��\��
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Player Param"))
	{
		ImGui::Text("State:%s", typeid(*m_pState).name());
		ImGui::Text("Direction:%.3f", GetDirection() * (180 / D3DX_PI));
		ImGui::Text("Speed:%.1f", GetMoveSpeed());
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
void CPlayer::Draw()
{
	// �L�����N�^�[�N���X�̕`�揈��
	//CCharacter::Draw();

#if 1	// �X�e���V���o�b�t�@�Ƀv���C���[�̃V���G�b�g��`�悷��

	// �f�o�C�X���擾
	auto pDev = CRenderer::GetDeviece();

	// �X�e���V���}�X�N��ݒ�
	pDev->SetRenderState(D3DRS_STENCILMASK, 0x000000ff);

	// �X�e���V���Q�ƒl��ݒ�
	pDev->SetRenderState(D3DRS_STENCILREF, 0x01);

	// �X�e���V���o�b�t�@�̔�r���@��ύX
	pDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);

	// �X�e���V���e�X�g�̌��ʂɑ΂��Ă̂ӂ�܂���ݒ肷��
	pDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILCAPS_REPLACE);	// Z�e�X�g�E�X�e���V���e�X�g�ɐ���
	pDev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILCAPS_KEEP);		// Z�e�X�g�̂ݎ��s
	pDev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILCAPS_KEEP);		// Z�e�X�g�E�X�e���V���e�X�g�Ɏ��s

	// �L�����N�^�[�N���X�̕`�揈�� (�X�e���V���o�b�t�@)
	CCharacter::Draw();

	// �X�e���V���}�X�N��ݒ�
	pDev->SetRenderState(D3DRS_STENCILMASK, 0x000000ff);

	// �X�e���V���Q�ƒl��ݒ�
	pDev->SetRenderState(D3DRS_STENCILREF, 0x02);

	// �X�e���V���o�b�t�@�̔�r���@��ύX
	pDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);

	// �l�p�`�}�X�N�̕`��
	CManager::GetManager()->GetMask_Rectangle()->Draw();

#endif

#if 0
	for (DWORD i = 0; i < 10; ++i)
	{
		// �X�e�[�g�}�l�[�W���[�̍X�V
		CCharacter::Update();

		// �L�����N�^�[�N���X�̃N���X�̕`�揈��
		CCharacter::Draw();
	}
#endif
}

//============================================================================
// �_���[�W���󂯂�
//============================================================================
void CPlayer::SetDamage(int nDamage)
{
	// �_���[�W�ʕ��A�̗͂�ϓ�
	 int nNewLife = GetLife();
	 nNewLife += nDamage;
	 SetLife(nNewLife);

	// �_���[�W��Ԃ֕ύX����悤���߂���
	m_pState->To_Damage();
}

//============================================================================
// ���a���擾
//============================================================================
float CPlayer::GetRadius() const
{
	return m_pBndCylinder->GetRadius();
}

//============================================================================
// �������擾
//============================================================================
float CPlayer::GetHeight() const
{
	return m_pBndCylinder->GetHeight();
}

//============================================================================
// �o�E���f�B���O�V�����_�[���擾
//============================================================================
const CBounding_Cylinder* const CPlayer::GetBndCylinder() const
{
	return m_pBndCylinder;
}

//============================================================================
// ���݂̃X�e�[�g���擾
//============================================================================
const CPlayer_State* const CPlayer::GetNowState() const
{
	// �X�e�[�g�̃|�C���^��Ԃ�
	return m_pState;
}

//============================================================================
// ����
//============================================================================
CPlayer* CPlayer::Create()
{
	// �C���X�^���X�𐶐�
	CPlayer* pNewInstance = DBG_NEW CPlayer();

	// �^�C�v��ݒ�
	pNewInstance->SetType(TYPE::PLAYER);
	
	// �����ݒ�
	pNewInstance->Init();

	// ���f����ݒ�
	pNewInstance->BindModel(CX_Manager::TYPE::SAMUS);

	// ���a��ݒ�
	float fRad = 0.0f;
	pNewInstance->GetModel()->Size.x > pNewInstance->GetModel()->Size.z ? fRad = pNewInstance->GetModel()->Size.x : fRad = pNewInstance->GetModel()->Size.z;
	pNewInstance->m_pBndCylinder->SetRadius(fRad);

	// ������ݒ�
	pNewInstance->m_pBndCylinder->SetHeight(pNewInstance->GetModel()->Size.y);

	return pNewInstance;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ������␳
//============================================================================
void CPlayer::AdjustHeight()
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
// �����蔻��
//============================================================================
void CPlayer::HitCheck()
{
	static CObject_X* TESTTEST = CObject_X::Create(utility::OpenJsonFile("Data\\JSON\\testtest.json"));
	static CObject_X* �ĂĂ� = CObject_X::Create(utility::OpenJsonFile("Data\\JSON\\�ĂĂ�.json"));

	// �Փ˂̗L�������o
	bool bDetect = false;

	// �����蔻��̒��S�_��ݒ�
	m_pBndCylinder->SetCenterPos(GetPos());

	// �~�h���I�u�W�F�N�g���擾
	CObject* pObj = CObject::GetTopObject(static_cast<int>(CObject::LAYER::MIDDLE));

	while (pObj != nullptr)
	{
		// �u���b�N�N���X�Ƀ_�E���L���X�g
		CBlock* pBlock = CObject::DownCast<CBlock>(pObj);

		// �L���X�g���s��
		if (!pBlock)
		{
			continue;
		}

		// �o�E���f�B���O�V�����_�[�̃p�����[�^���R�s�[
		const Vec3& CylinderPosTarget = GetPosTarget();
		const Vec3& CylinderOldPos = GetPos();
		const float& CylinderRadius = GetRadius();
		const float& CylinderHeight = GetHeight();

		// �o�E���f�B���O�{�b�N�X�̃p�����[�^���R�s�[
		const Vec3& BoxSize = pBlock->GetSize();
		const Vec3& BoxPos = pBlock->GetPos();
		const float& fBoxDirection = pBlock->GetRot().y;
		{
			TESTTEST->SetScale(BoxSize);
		}

		// �@�{�b�N�X�̒��S�_����V�����_�[�̍��W�ւ̋����x�N�g�����v�Z
		const Vec3& RelativePos = CylinderPosTarget - BoxPos;
		const Vec3& RelativeOldPos = CylinderOldPos - BoxPos;

		// �A���W�n�𐳋K�����邽�߁A�����x�N�g���Ƀ{�b�N�X�̉�]�ʂ�ł������悤�ɉ�]�s���K�p
		const Vec3& ResultPos = utility::RotatePointAroundY(-fBoxDirection, RelativePos);
		const Vec3& ResultOldPos = utility::RotatePointAroundY(-fBoxDirection, RelativeOldPos);
		{
			�ĂĂ�->SetScale({ GetBndCylinder()->GetRadius(), GetBndCylinder()->GetHeight(), GetBndCylinder()->GetRadius() });
			�ĂĂ�->SetPos(ResultPos);
		}

		// ���W�n�𐳋K���������肵�A�{�b�N�X����V�����_�[�ւ̋����x�N�g����p���ďՓ˔���
		// (�{�b�N�X�̍��W�͌��_�ŁA��]�͍l�������AAABB�Ɖ��肷��)
		if (collision::HitCylinderToAABB(ResultPos, CylinderRadius, CylinderHeight, VEC3_INIT, BoxSize))
		{
			// ����\����ԐF��
			//m_pBndCylinder->ChangeModel(CX_Manager::TYPE::RENDER_CYLINDER_HIT);
				
			// �Փ˂����������Ƃ����o
			bDetect = 1;
			
			// �Փ˖ʂ𔻒�
			int nIdx = collision::GetCylinderToAABB(ResultOldPos, ResultPos, CylinderRadius, CylinderHeight, VEC3_INIT, BoxSize);

			// �㉺�̔����2D��Y������ƕς��Ȃ��̂ŕ��ʂɃu���b�N�̏㉺���ŉ����o��
			// ���E�̔���͐��K���������W�ŉ����o�����s������ɉ�]�����Ė{���̂���ׂ����W��ݒ�
			switch (nIdx)
			{
			case 0:	// ���ޕs�\
				break;

			case 1: // ��
			{
				// Y�������̉����x��������
				SetVelY(0.0f);

				// �V�����ڕW���W���쐬
				const Vec3& NewPosTarget = { CylinderPosTarget.x,
					BoxPos.y + BoxSize.y + CylinderHeight,
					CylinderPosTarget.z };
			 
				// �ڕW���W�𔽉f
				SetPosTarget(NewPosTarget);
			
				break;
			}
			case 2: // ��
			{
				// Y�������̉����x���d�͕����֌Œ�
				SetVelY(0.0f);

				// �V�����ڕW���W���쐬
				const Vec3& NewPosTarget = {
					CylinderPosTarget.x,
					BoxPos.y - BoxSize.y - CylinderHeight,
					CylinderPosTarget.z };

				// �ڕW���W�𔽉f
				SetPosTarget(NewPosTarget);

				break;
			}
			case 3:	// ��
			{
#if 0	// �����o�����@�̐؂�ւ� (������)
				// ���K�����ꂽ���W�n���ŐV�����ڕW���W���쐬
				Vec3 NewPosTarget = {
					/* BoxPos.x <- 0.0f */ + BoxSize.x + CylinderRadius,
					CylinderPosTarget.y,
					//(ResultOldPos.z/* - BoxPos.z <- 0.0f */) * (ResultPos.x / ResultOldPos.x)};
					//ResultOldPos.z};
					CylinderOldPos.z * (ResultPos.x / ResultOldPos.x) };
				/* ��Z���W�ɂ��āA�~�ʂňړ����邽�߁A�c��ȃX�C�[�v������s�����ƂɂȂ��Ă��܂��c
				�{�b�N�X�̒��S���猳��Z���W�ւ̕���������X�̃T�C�Y���Ƌϓ��Ȋ����ɂȂ�悤�ɒ������� */

				// �ł���������]���ēx�������킹�Ė{���̌����ɖ߂�
				NewPosTarget = utility::RotatePointAroundY(fBoxDirection + D3DX_PI * 0.5f, NewPosTarget);

				CRenderer::SetTimeString("���K�����W�n�ł̍��W:x " + to_string(NewPosTarget.x) + ":y " + to_string(NewPosTarget.y) + ":z " + to_string(NewPosTarget.z), 600);
				CRenderer::SetTimeString("Z����" + to_string((ResultPos.x / ResultOldPos.x)), 600);

				// �������������炢��ׂ����p��␳
				SetDirection(atan2f(NewPosTarget.x, NewPosTarget.z));

				// �␳���p����ڕW���W��ݒ�
				AutoSetPosTarget();
				
				CRenderer::SetTimeString("�ʏ���W�n�ł̍��W:x " + to_string(GetPosTarget().x) + ":y " + to_string(GetPosTarget().y) + ":z " + to_string(GetPosTarget().z), 600);
#else
				// �ߋ��̕��p�֖߂�
				SetDirection(GetOldDirection());

				// �ڕW���W���Đݒ�
				AutoSetPosTarget();
#endif
				break;
			}
			case 4:	// �E
			{
				// �ߋ��̕��p�֖߂�
				SetDirection(GetOldDirection());

				// �ڕW���W���Đݒ�
				AutoSetPosTarget();

				break;
			}
			default:
				break;
			}

			break;
		}

		pObj = pObj->GetNext();
	}

	// ���ɂ��Փ˂��Ă��Ȃ����
	if (!bDetect)
	{
		// ����\����ʏ�F�ɖ߂�
		m_pBndCylinder->ChangeModel(CX_Manager::TYPE::RENDER_CYLINDER);
	}
}