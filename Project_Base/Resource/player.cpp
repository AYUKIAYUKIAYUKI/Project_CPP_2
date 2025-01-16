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
#include "player_state_damage.h"
#include "object_Parts.h"

#include "block.h"
#include "collision.h"
#include "field_manager.h"

//****************************************************
// �v���v���Z�b�T�f�B���N�e�B�u
//****************************************************
#define SHOW_NORMALIZED_POSITION_HITCHECK 0	// ���K�����W�ł̓����蔻���\��

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
	CCharacter{ LAYER::DEFAULT },
	m_pBndCylinder{ DBG_NEW CBounding_Cylinder() },
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
	
	// �L�����N�^�[�N���X�̏����ݒ�
	if (FAILED(CCharacter::Init()))
	{
		assert(false && "�v���C���[�N���X�̏����ݒ�Ɏ��s");
	}

	// �␳���x��ݒ�
	SetCorrectCoef(CORRECT_COEF);
	
	// �������p��ݒ�
	SetDirection(D3DX_PI * -0.5f);
	SetDirectionTarget(D3DX_PI * -0.5f);

	// �����̌������Z�b�g
	SetRot({ 0.0f, D3DX_PI, 0.0f });
	SetRotTarget({ 0.0f, D3DX_PI, 0.0f });

	// �����̍��W���Z�b�g
	SetPos({ 0.0f, 60.0f, 0.0f });
	SetPosTarget({ 0.0f, 60.0f, 0.0f });

	// �����ړ����x��ݒ�
	SetMoveSpeed(DEFAULT_MOVE_SPEED);

	// �����̗͂�ݒ�
	SetLife(MAX_LIFE);

	// ��s���[�V�������Z�b�g
	SetNowMotion(8);

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

	// ���W����p�ɍ��킹��
	AutoSetPosTarget();

	// �����蔻�� (����菇�́A�ϓ������ڕW���p�E�ڕW���W���Z�[�t���ǂ���)
	HitCheck();

	// �~���o�E���f�B���O�̒��S�_���v���C���[�̍��W��
	m_pBndCylinder->SetCenterPos(GetPos());

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
		ImGui::Text("DirectionTarget:%.3f", GetDirectionTarget() * (180 / D3DX_PI));
		ImGui::Text("Speed:%.1f", GetMoveSpeed());
		ImGui::Text("Rot:X %.1f:Y %.1f:Z %.1f", GetRot().x * (180 / D3DX_PI), GetRot().y * (180 / D3DX_PI), GetRot().z * (180 / D3DX_PI));
		ImGui::Text("RotTarget:X %.1f:Y %.1f:Z %.1f", GetRotTarget().x * (180 / D3DX_PI), GetRotTarget().y * (180 / D3DX_PI), GetRotTarget().z * (180 / D3DX_PI));
		ImGui::Text("Pos:X %.1f:Y %.6f:Z %.1f", GetPos().x, GetPos().y, GetPos().z);
		ImGui::Text("PosTarget:X %.1f:Y %.6f:Z %.1f", GetPosTarget().x, GetPosTarget().y, GetPosTarget().z);
		ImGui::Text("VelY:%.1f", GetVelY());
		ImGui::Text("Life:%d", GetLife());
		ImGui::Text("Motion:%d", GetNowMotion());
		ImGui::End();
	}
#endif	// _DEBUG
#endif
}

//============================================================================
// �`�揈��
//============================================================================
void CPlayer::Draw()
{

}

//============================================================================
// �_���[�W���󂯂�
//============================================================================
void CPlayer::SetDamage(int nDamage, float fImpact)
{
	if (typeid(*m_pState) != typeid(CPlayer_State_Damage))
	{
		// �_���[�W�ʕ��A�̗͂�ϓ�
		int nNewLife = GetLife();
		nNewLife += nDamage;
		SetLife(nNewLife);

		// �y����ɐ������
		SetVelY(3.0f);

		// �Ռ��ʕ��A���p��ϓ�
		float fNewDirectionTarget = GetDirectionTarget();
		fNewDirectionTarget += fImpact;
		SetDirectionTarget(fNewDirectionTarget);

		// �_���[�W��Ԃ֕ύX����悤���߂���
		m_pState->To_Damage();
	}
}

//============================================================================
// ���V��L����
//============================================================================
void CPlayer::EnableWings()
{
	
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

	// ���[�V�������Z�b�g
	pNewInstance->SetMotion(LAYER::DEFAULT, utility::OpenJsonFile("Data\\JSON\\CHARACTER\\PLAYER\\player_motion.json"));

	// ���a��ݒ�
	pNewInstance->m_pBndCylinder->SetRadius(3.0f);

	// ������ݒ�
	pNewInstance->m_pBndCylinder->SetHeight(8.5f);

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
	// �Փ˂̗L�����L�^
	bool bDetect = false;

	// �ʏ�D��x�̃I�u�W�F�N�g���擾
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	while (pObj != nullptr)
	{
		// �u���b�N�N���X�Ƀ_�E���L���X�g
		CBlock* pBlock = CObject::DownCast<CBlock>(pObj);

		// �L���X�g���s��
		if (!pBlock)
		{
			pObj = pObj->GetNext();
			continue;
		}

		// �v���C���[�̃p�����[�^���R�s�[
		const Vec3& CylinderPosTarget = GetPosTarget();	// �ڕW���W
		const Vec3& CylinderOldPos = GetPos();			// ���ݍ��W
		const float& CylinderRadius = GetRadius();		// ���a
		const float& CylinderHeight = GetHeight();		// ����

		// �o�E���f�B���O�{�b�N�X�̃p�����[�^���R�s�[
		const Vec3& BoxSize = pBlock->GetSize();			// �u���b�N�̍��W
		const Vec3& BoxPos = pBlock->GetPos();				// �u���b�N�̍��W
		const float& fBoxDirection = pBlock->GetRot().y;	// �u���b�N�̉�]����
#if SHOW_NORMALIZED_POSITION_HITCHECK
		{
			TESTTEST->SetScale(BoxSize);
		}
#endif	// _SHOW_NORMALIZED_POSITION_HITCHECK_

		// �@�{�b�N�X�̒��S�_����V�����_�[�̍��W�ւ̋����x�N�g�����v�Z
		const Vec3& RelativePos = CylinderPosTarget - BoxPos;
		const Vec3& RelativeOldPos = CylinderOldPos - BoxPos;

		// �A���W�n�𐳋K�����邽�߁A�����x�N�g���Ƀ{�b�N�X�̉�]�ʂ�ł������悤�ɉ�]�s���K�p
		const Vec3& ResultPos = utility::RotatePointAroundY(-fBoxDirection, RelativePos);
		const Vec3& ResultOldPos = utility::RotatePointAroundY(-fBoxDirection, RelativeOldPos);
#if SHOW_NORMALIZED_POSITION_HITCHECK
		{
			�ĂĂ�->SetScale({ GetBndCylinder()->GetRadius(), GetBndCylinder()->GetHeight(), GetBndCylinder()->GetRadius() });
			�ĂĂ�->SetPos(ResultPos);
		}
#endif	// _SHOW_NORMALIZED_POSITION_HITCHECK_

		// ���W�n�𐳋K���������肵�A�{�b�N�X����V�����_�[�ւ̋����x�N�g����p���ďՓ˔���
		// (�{�b�N�X�̍��W�͌��_�ŁA��]�͍l�������AAABB�Ɖ��肷��)
		if (collision::HitCylinderToAABB(ResultPos, CylinderRadius, CylinderHeight, VEC3_INIT, BoxSize))
		{
#ifdef _DEBUG
			// ����\����ԐF��
			//m_pBndCylinder->SetCol({ 1.0f, 0.0f, 0.0f, 0.5f });
#endif // _DEBUG
				
			// �Փ˂����������Ƃ��L�^
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
				//// �ύX���ꂽ�ڕW���p�̌��݂̕��p�֖߂�	/* �p�^�[��1 */
				//SetDirectionTarget(GetDirection());

				//// �ύX���ꂽ�ڕW���W�����݂̍��W�ɖ߂�
				//AutoSetPosTarget();
#endif
				// �ՓˑO�̍��W����A�����钼�O�ƂȂ���p���o��	/* �p�^�[��2 */
				SetDirectionTarget(atan2f(CylinderOldPos.z, CylinderOldPos.x));

				// �Փː��O�̕��p����ڕW���W������o��
				AutoSetPosTarget();

				break;
			}
			case 4:	// �E
			{
				/* �p�^�[��2 */

				// �ՓˑO�̍��W����A�����钼�O�ƂȂ���p���o��	/* �p�^�[��2 */
				SetDirectionTarget(atan2f(CylinderOldPos.z, CylinderOldPos.x));

				// �Փː��O�̕��p����ڕW���W������o��
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
#ifdef _DEBUG
		// ����\����ʏ�F�ɖ߂�
		//m_pBndCylinder->SetCol({ 1.0f, 1.0f, 1.0f, 0.5f });
#endif // _DEBUG
	}
}