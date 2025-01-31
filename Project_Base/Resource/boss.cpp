//============================================================================
// 
// �{�X [boss.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "boss.h"
#include "bounding_cylinder.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "field_manager.h"
#include "field_builder.h"
#include "object_parts.h"
#include "collision.h"

#include "player.h"
#include "sparks.h"
#include "scene.h"
#include "game.h"

#include "field_type.h"
#include "block.h"
#include "bright.h"
#include "enemy.h"
#include "monster.h"
#include "ghost.h"
#include "flyer.h"

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
CBoss::CBoss() :
	CCharacter{ LAYER::DEFAULT },
	m_ActionType{ ACTION::HOLDCENTER },
	m_nCntActionCast{ 0 },
	m_nDuration{ 0 },
	m_pBndCylinder{ DBG_NEW CBounding_Cylinder() }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CBoss::~CBoss()
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
HRESULT CBoss::Init()
{
	// �␳���x��ݒ�
	SetCorrectCoef(CORRECT_COEF);

	// �����̗͂�ݒ�
	SetLife(MAX_LIFE);

	// �L�����N�^�[�N���X�̏����ݒ�
	if (FAILED(CCharacter::Init()))
	{
		assert(false && "�{�X�N���X�̏����ݒ�Ɏ��s");
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CBoss::Uninit()
{
	// �L�����N�^�[�N���X�̏I������
	CCharacter::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CBoss::Update()
{
	// �o�ꃂ�[�V�������I��������
	if (GetNowMotion() == 0 && GetStopState())
	{
		// �ʏ탂�[�V������
		SetNowMotion(1);

		// �s���L���X�g�����Z�b�g
		m_nCntActionCast = 0;

		// �J�����̃v���C���[�Ǐ]�����̎��ɖ߂�
		CManager::GetManager()->GetCamera()->ChangeTrackPlayer(true);
	}

	// �΂̕���Ҕ���������
	CSparks::FuryGenerate();

	// ���̍s��������
	SetNextAction();

	// �s������
	BranchAction();

	// �~���o�E���f�B���O�̒��S�_�����f���̒��S��
	m_pBndCylinder->SetCenterPos(GetPos());

	// �L�����N�^�[�N���X�̍X�V����
	CCharacter::Update();

#ifdef _DEBUG
	// �E�B���h�E��\��
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Boss Param"))
	{
		ImGui::Text("ActionType:%d", m_ActionType);
		ImGui::Text("Direction:%.1f", GetDirection() * (180 / D3DX_PI));
		ImGui::Text("DirectionTarget:%.1f", GetDirectionTarget() * (180 / D3DX_PI));
		ImGui::Text("Rot:X %.1f:Y %.1f:Z %.1f", GetRot().x * (180 / D3DX_PI), GetRot().y * (180 / D3DX_PI), GetRot().z * (180 / D3DX_PI));
		ImGui::Text("RotTarget:X %.1f:Y %.1f:Z %.1f", GetRotTarget().x * (180 / D3DX_PI), GetRotTarget().y * (180 / D3DX_PI), GetRotTarget().z * (180 / D3DX_PI));
		ImGui::Text("Pos:X %.1f:Y %.1f:Z %.1f", GetPos().x, GetPos().y, GetPos().z);
		ImGui::Text("PosTarget:X %.1f:Y %.1f:Z %.1f", GetPosTarget().x, GetPosTarget().y, GetPosTarget().z);
		ImGui::Text("VelY:%.1f", GetVelY());
		ImGui::Text("Life:%d", GetLife());
		ImGui::End();
	}
#endif	// _DEBUG
}

//============================================================================
// �`�揈��
//============================================================================
void CBoss::Draw()
{

}

//============================================================================
// �_���[�W���󂯂�
//============================================================================
void CBoss::SetDamage(int nDamage)
{
	if (m_ActionType != ACTION::DAMAGEBACK && m_ActionType != ACTION::DEADEND)
	{
		// �_���[�W�ʕ��A�̗͂�ϓ�
		int nNewLife = GetLife();
		nNewLife += nDamage;
		SetLife(nNewLife);

		// �_���[�W��炢�s������������
		m_ActionType = ACTION::DAMAGEBACK;

		// �̗͂��[���ȉ��Ȃ玀�S�s������������
		if (nNewLife <= 0)
			m_ActionType = ACTION::DEADEND;
	}
}

//============================================================================
// ���a���擾
//============================================================================
float CBoss::GetRadius() const
{
	return m_pBndCylinder->GetRadius();
}

//============================================================================
// �������擾
//============================================================================
float CBoss::GetHeight() const
{
	return m_pBndCylinder->GetHeight();
}

//============================================================================
// �o�E���f�B���O�V�����_�[���擾
//============================================================================
const CBounding_Cylinder* const CBoss::GetBndCylinder() const
{
	return m_pBndCylinder;
}

//============================================================================
// ����
//============================================================================
CBoss* CBoss::Create()
{
	// �C���X�^���X�𐶐�
	CBoss* pNewInstance = DBG_NEW CBoss();

	// �^�C�v��ݒ�
	pNewInstance->SetType(TYPE::BOSS);

	// �����ݒ�
	pNewInstance->Init();

	// ���[�V�������Z�b�g
	pNewInstance->SetMotion(LAYER::DEFAULT, utility::OpenJsonFile("Data\\JSON\\CHARACTER\\BOSS\\pumpkin_motion.json"));

	// ���a��ݒ�
	pNewInstance->m_pBndCylinder->SetRadius(100.0f);

	// ������ݒ�
	pNewInstance->m_pBndCylinder->SetHeight(60.0f);

	return pNewInstance;
}

//============================================================================
// 
// private�����o
//  
//============================================================================

//============================================================================
// ���̍s��������
//============================================================================
void CBoss::SetNextAction()
{
	// �L���X�g�����l�ɒB�����
	if (m_nCntActionCast > 240)
	{
		// �s���L���X�g�����Z�b�g
		m_nCntActionCast = 0;

		// �����_���ɐ��l�𒊑I
		int nRand = rand() % 3;

		// �s�����Z�b�g
		switch (nRand)
		{
		case 0:
			m_ActionType = ACTION::DIRECTATTACK;
			break;

		case 1:
			m_ActionType = ACTION::SUMMONENEMY;
			break;

		case 2:
			m_ActionType = ACTION::DIRECTATTACK;
			break;

		default:	// ��O
#ifdef _DEBUG
			assert(false && "�{�X�̍s������ɗ�O");
#endif // _DEBUG
			break;
		}
	}
}

//============================================================================
// �s������
//============================================================================
void CBoss::BranchAction()
{
	if (m_ActionType == ACTION::HOLDCENTER)
	{ // �s�����ݒ肳��Ă��Ȃ���

		// �s���L���X�g���C���N�������g
		++m_nCntActionCast;

		// �����ɑҋ@
		HoldCenter();
	}
	else
	{ // �s���^�C�v���������Ă��鎞

		// �^�C�v�ɉ����ď�����ύX
		switch (m_ActionType)
		{
			// �̓�����
		case ACTION::DIRECTATTACK:
			DirectAttack();
			break;

			// �ǂÂ���
		case ACTION::WALLATTACK:
			WallAttack();
			break;

			// �G����
		case ACTION::SUMMONENEMY:
			SummonEnemy();
			break;

			// �_���[�W��炢
		case ACTION::DAMAGEBACK:
			DamageBack();
			break;

			// ���S
		case ACTION::DEADEND:
			DeadEnd();
			break;

			// ��O
		case ACTION::HOLDCENTER:
		default:
#ifdef _DEBUG
			assert(false && "�{�X�̍s���ɗ�O����");
#else
			m_ActionType = ACTION::HOLDCENTER;
#endif // _DEBUG
			break;
		}
	}
}

//============================================================================
// ���S�őҋ@
//============================================================================
void CBoss::HoldCenter()
{
	// �v���C���[���擾
	CPlayer* pPlayer = nullptr;
	pPlayer = utility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));

	// �v���C���[�̕���������
	Vec3 Rot = VEC3_INIT, Pos = pPlayer->GetPos();
	Rot.y = atan2f(-Pos.x, -Pos.z);
	SetRotTarget(Rot);	

	// �����ɕ�����
	Pos = VEC3_INIT;
	Pos.y = 100.0f;
	SetPosTarget(Pos);
}

//============================================================================
// �̓�����
//============================================================================
void CBoss::DirectAttack()
{
	if (GetNowMotion() != 2)
	{
		// �{�X���u���u�����[�V�����ɕύX
		SetNowMotion(2);

		// ���̏u�Ԃ̃v�����[�̕��p���R�s�[
		CPlayer* pPlayer = nullptr;
		pPlayer = utility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));
		SetDirectionTarget(pPlayer->GetDirection());

		// �G�l�~�[�o������炷
		CSound::GetInstance()->Play(CSound::LABEL::EAPPEAR);
	}

	// �p�����Ԃ��C���N�������g
	++m_nDuration;

	if (m_nDuration <= 120)
	{
		// �J���������E���Փx�����������ύX
		{
			CCamera* pCamera = CManager::GetManager()->GetCamera();
			float fDinstance = pCamera->GetDistance(), fUpAdjust = pCamera->GetUpAdjust();
			fDinstance += (450.0f - fDinstance) * 0.1f;
			fUpAdjust += (150.0f - fUpAdjust) * 0.05f;
			pCamera->SetDistance(fDinstance);
			pCamera->SetUpAdjust(fUpAdjust);
		}
	}
	else
	{
		// �J���������E���Փx�����������ύX
		{
			CCamera* pCamera = CManager::GetManager()->GetCamera();
			float fDinstance = pCamera->GetDistance(), fUpAdjust = pCamera->GetUpAdjust();
			fDinstance += (250.0f - fDinstance) * 0.1f;
			fUpAdjust += (75.0f - fUpAdjust) * 0.05f;
			pCamera->SetDistance(fDinstance);
			pCamera->SetUpAdjust(fUpAdjust);
		}
	}

	// 1.5�b���_��
	if (m_nDuration == 90)
	{
		// �ːi�ڕW�̍��W���쐬
		Vec3 PosTarget = {
			cosf(GetDirection()) * CField_Manager::FIELD_RADIUS,
			30.0f,
			sinf(GetDirection()) * CField_Manager::FIELD_RADIUS
		};

		// �ڕW���W�ɔ��f
		SetPosTarget(PosTarget);

		// �{�X�̍U������炷
		CSound::GetInstance()->Play(CSound::LABEL::BATK);
	}

	// 4�b���_��
	if (m_nDuration == 240)
	{
		// �p�����Ԃ����Z�b�g
		m_nDuration = 0;

		// �����ҋ@�ɖ߂�
		m_ActionType = ACTION::HOLDCENTER;

		// �ҋ@���[�V�����ɖ߂�
		SetNowMotion(1);
	}

	// �Փ˔���ɐ���������
	if (HitCheck())
	{
		// �v���C���[���擾
		CPlayer* pPlayer = nullptr;
		pPlayer = utility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));

		// ���_����L�����N�^�[�̂���ꏊ�ւ̕����x�N�g�����쐬
		Vec3 CharacterPosVec = GetPos() - VEC3_INIT;
		D3DXVec3Normalize(&CharacterPosVec, &CharacterPosVec);

		// ���_����v���C���[�̂���ꏊ�ւ̃x�N�g�����쐬
		Vec3 PlayerVec = pPlayer->GetPos() - VEC3_INIT;
		D3DXVec3Normalize(&PlayerVec, &PlayerVec);

		// 2�{�̕����x�N�g���̊O�ς��쐬
		float fCross = (CharacterPosVec.x * PlayerVec.z) - (CharacterPosVec.z * PlayerVec.x);

		// ���a�ɉ����ďՌ��ʂ��쐬
		float fImpact = GetRadius() * 0.001f;

		// ���݂��̈ʒu�֌W�ɍ��킹�Đ�����΂����p��ύX
		if (fCross < 0.0f) fImpact *= -1.0f;

		// 1�_���[�W�ƏՌ��͂�^����
		pPlayer->SetDamage(-1, fImpact);
	}
}

//============================================================================
// �Ǎ��
//============================================================================
void CBoss::WallAttack()
{
	// �p�����Ԃ��C���N�������g
	++m_nDuration;

	// �J���������E���Փx�����������ύX
	{
		CCamera* pCamera = CManager::GetManager()->GetCamera();
		float fDinstance = pCamera->GetDistance(), fUpAdjust = pCamera->GetUpAdjust();
		fDinstance += (300.0f - fDinstance) * 0.1f;
		fUpAdjust += (100.0f - fUpAdjust) * 0.05f;
		pCamera->SetDistance(fDinstance);
		pCamera->SetUpAdjust(fUpAdjust);
	}

	// �u���b�N�̃��f���^�C�v�����O�ɑI�o
	CX_Manager::TYPE Type = CX_Manager::TYPE::BLONORMAL + (rand() % 3);

	{ // �����p�̍��W������

		// �v���C���[���擾
		CPlayer* pPlayer = nullptr;
		pPlayer = utility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));

		// �v���C���[�̖ڕW���W�ւ̃x�N�g�����쐬
		Vec3 Norm = pPlayer->GetPosTarget() - pPlayer->GetPos();

		// �v���C���[��������x�ړ����Ă��邱�Ƃ����m�������̂�
		if (Norm.x * Norm.x + Norm.z * Norm.z > 0.1f)
		{
			// �������W�v�Z�p ((���p + ��`���̊p�x)�̏ꏊ�������|�C���g)
			float fDirection = pPlayer->GetDirection();		// �v���C���[�̌��݂̕��p���R�s�[
			//float fRange = m_pFan->GetRange();			// ��`�͈͂̕����R�s�[
			float fRange = 0.5f;
			Vec3  NewPos = VEC3_INIT, NewRot = VEC3_INIT;	// �u���b�N�p�̍��W�E�������쐬

			// ���ݍ��W�ƖڕW���W�ɑ΂����_����̕����x�N�g�����쐬
			Vec3 OldVec = pPlayer->GetPos() - VEC3_INIT, NewVec = pPlayer->GetPosTarget() - VEC3_INIT;
			D3DXVec3Normalize(&OldVec, &OldVec);
			D3DXVec3Normalize(&NewVec, &NewVec);

			// 2�{�̕����x�N�g���̊O�ς��쐬
			float fCross = (OldVec.x * NewVec.z) - (OldVec.z * NewVec.x);

			// ���Ɉړ����Ă���ꍇ�p�x�𔽓]������
			if (fCross < 0.0f)
				fRange = -fRange;

			// �@���E�̏ꏊ�͋��E���̈ʒu��
			NewPos.x = cosf(fRange + fDirection) * CField_Manager::FIELD_RADIUS;
			NewPos.z = sinf(fRange + fDirection) * CField_Manager::FIELD_RADIUS;

			// �A�����̓����_��
			NewPos.y = fabsf(utility::GetRandomValue<float>());

			/**/
			bool bSafe = true;
#if 1
			{
				// �ʏ�D��x�̃I�u�W�F�N�g���擾
				CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

				while (pObj != nullptr)
				{
					// �u���b�N�^�C�v�̃I�u�W�F�N�g�̂Ƃ�
					if (pObj->GetType() == CObject::TYPE::BLOCK)
					{
						// �I�u�W�F�N�g���u���b�N�N���X�Ƀ_�E���L���X�g
						CBlock* pOther = nullptr;
						pOther = utility::DownCast(pOther, pObj);

						// �K�v�ȏ���p��
						const CX_Manager::TYPE&
							OtherType = pOther->GetModelType();	// �Ώۂ̃��f���^�C�v���擾
						const Vec3&
							Distance = pOther->GetPos() - NewPos,	// ���݂��̋����̍�
							OtherSize = pOther->GetSize();			// �Ώۂ̃T�C�Y
						float
							DistanceNorm = (Distance.x * Distance.x + Distance.y * Distance.y + Distance.z * Distance.z);	// �����̍��̑傫��

						// ���̃u���b�N�Ƃ̋�����������x�߂���Ώ����𖞂����Ȃ�
						if (DistanceNorm <= CField_Type::GetAreaNorm() * 0.1f)
						{
							// �d�����Ă���A���̃u���b�N�𐶐����Ȃ�
							bSafe = false;

							break;
						}
					}

					// ���̃I�u�W�F�N�g��
					pObj = pObj->GetNext();
				}
			}
#endif

			if (bSafe)
			{
				// ����������
				NewRot.y = atan2f(-NewPos.x, -NewPos.z);

				// �u���b�N�𐶐����A�u���b�N���f����ύX����
				CBlock* pBlock = CBlock::Create(NewPos, NewRot);	// �u���b�N�̃C���X�^���X����
				pBlock->BindModel(Type);							// ���f�������蓖��
				pBlock->SetSize(pBlock->GetModel()->Size);			// �o�E���f�B���O�T�C�Y�𑵂���
			}
		}
	}

	// �ʏ탌�C���[�̃I�u�W�F�N�g���擾
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	while (pObj != nullptr)
	{
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// �I�u�W�F�N�g���u���b�N�^�O�Ƀ_�E���L���X�g
			CBlock* pBlock = utility::DownCast<CBlock, CObject>(pObj);

			// �Ώە��ւ̃x�N�g�����o��
			Vec3 OtherVec = pBlock->GetPos() - VEC3_INIT;

			// �Ώە��ւ̃x�N�g���𐳋K��
			D3DXVec3Normalize(&OtherVec, &OtherVec);

			// �v���C���[���擾
			CPlayer* pPlayer = nullptr;
			pPlayer = utility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));

			// �v���C���[�̖ڕW���W�ւ̃x�N�g�����쐬
			Vec3 Norm = pPlayer->GetPosTarget() - pPlayer->GetPos();

			// �������W�v�Z�p ((���p + ��`���̊p�x)�̏ꏊ�������|�C���g)
			float fDirection = pPlayer->GetDirection();		// �v���C���[�̌��݂̕��p���R�s�[
			float fRange = 0.45f;
			Vec3  NewPos = VEC3_INIT, NewRot = VEC3_INIT;	// �u���b�N�p�̍��W�E�������쐬
			
			// ���ݍ��W�ƖڕW���W�ɑ΂����_����̕����x�N�g�����쐬
			Vec3 OldVec = pPlayer->GetPos() - VEC3_INIT, NewVec = pPlayer->GetPosTarget() - VEC3_INIT;
			D3DXVec3Normalize(&OldVec, &OldVec);
			D3DXVec3Normalize(&NewVec, &NewVec);

			// 2�{�̕����x�N�g���̊O�ς��쐬
			float fCross = (OldVec.x * NewVec.z) - (OldVec.z * NewVec.x);

			// ���Ɉړ����Ă���ꍇ�p�x�𔽓]������
			if (fCross < 0.0f)
				fRange = -fRange;

			// ���p�ɍ��킹�Ĕ͈͕��̕����x�N�g����2�{�쐬
			Vec3 DirVec[2] = {};
			DirVec[0] = { cosf(fDirection + fRange), 0, sinf(fDirection + fRange) };
			DirVec[1] = { cosf(fDirection - fRange), 0, sinf(fDirection - fRange) };

			// �Е��̐�`�����̃x�N�g���ƑΏە��ւ̕����x�N�g���̊O�ς��쐬
			float fCross1 = DirVec[0].x * OtherVec.z - DirVec[0].z * OtherVec.x;

			// ���̎��_��1�ڂ̕����x�N�g���Ƃ̊O�ό��ʂ����̊p�x�Ȃ�͈͊O
			if (fCross1 > 0.0f)
			{
				continue;
			}

			// �����Е��̊O�ς��쐬
			float fCross2 = DirVec[1].x * OtherVec.z - DirVec[1].z * OtherVec.x;

			// 2�ڂ̕����x�N�g���Ƃ̊O�ό��ʂ����̊p�x�Ȃ�͈͊O
			if (fCross2 < 0.0f)
			{
				continue;
			}

			// �����𖞂����Ă���ΐ�`�͈͓̔��A���̃u���b�N�͔j��
			pBlock->SetRelease();
		}

		// ���̃I�u�W�F�N�g���擾
		pObj = pObj->GetNext();
	}
}

//============================================================================
// �G����
//============================================================================
void CBoss::SummonEnemy()
{
	if (GetNowMotion() != 2)
	{
		// �{�X���u���u�����[�V�����ɕύX
		SetNowMotion(2);

		// �G���ʐ���
		for (WORD wCntLoop = 0; wCntLoop < 3; ++wCntLoop)
		{
			// �����ʒu
			Vec3 InitPos = VEC3_INIT;

#if 1
			do
			{
				// ��`�͈͓��Ɏ��܂�悤�ɕ��p�������_������
				float fRand = fabsf(utility::GetRandomValue<float>()) * 0.01f;
				InitPos = utility::DirectionConvertVec3(D3DX_PI * fRand, 10.0f + fabsf(utility::GetRandomValue<float>()) * 0.25f, CField_Manager::FIELD_RADIUS);

				// �͈͊O�ōĒ��I
			} while (!CField_Manager::GetInstance()->GetFieldBuilder()->DetectInFanRange(InitPos));
#else
			// �v���C���[�̕��p���擾
			CPlayer* pPlayer = nullptr;
			pPlayer = utility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));
			if (pPlayer)
			{
				float fRand = utility::GetRandomValue<float>() * 0.005f;
				InitPos = utility::DirectionConvertVec3(pPlayer->GetDirection() + fRand, 10.0f + (45.0f * fabsf(fRand)), CField_Manager::FIELD_RADIUS);
			}
#endif

			// �����_���ɐ��l�𒊑I
			CBright::CREATETYPE Type = CBright::CREATETYPE::NONE;
			int nRand = rand() % 2;

			if (nRand == 0)
			{
				Type = CBright::CREATETYPE::MONSTER;
			}
			else
			{
				Type = CBright::CREATETYPE::GHOST;
			}

			// ���̏ꏊ�ɁA�����X�^�[���S�[�X�g�𐶐�
			CBright::Generate(InitPos, Type);
		}
	}

	// �v���C���[�̕�������������
	CPlayer* pPlayer = nullptr;
	pPlayer = utility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));
	if (pPlayer)
	{
		Vec3 Rot = VEC3_INIT, Pos = pPlayer->GetPos();
		Rot.y = atan2f(-Pos.x, -Pos.z);
		SetRotTarget(Rot);
	}

	// �J���������E���Փx�����������ύX
	{
		CCamera* pCamera = CManager::GetManager()->GetCamera();
		float fDinstance = pCamera->GetDistance(), fUpAdjust = pCamera->GetUpAdjust();
		fDinstance += (450.0f - fDinstance) * 0.1f;
		fUpAdjust += (150.0f - fUpAdjust) * 0.05f;
		pCamera->SetDistance(fDinstance);
		pCamera->SetUpAdjust(fUpAdjust);
	}

	// �΂̕���Ҕ���������
	for (WORD wCntLoop = 0; wCntLoop < 2; ++wCntLoop)
	{
		CSparks::FuryGenerate();
	}

	// �G�l�~�[�^�C�v�̃I�u�W�F�N�g���擾
	CObject* pObjEnemy = CObject::FindSpecificObject(CObject::TYPE::ENEMY);
	CObject* pObjBright = CObject::FindSpecificObject(CObject::TYPE::BRIGHT);

	// �G�l�~�[�̑|�����m�F������
	if (!pObjEnemy && !pObjBright)
	{
		// �p�����Ԃ����Z�b�g
		m_nDuration = 0;

		// �����ҋ@�ɖ߂�
		m_ActionType = ACTION::HOLDCENTER;

		// �ҋ@���[�V�����ɖ߂�
		SetNowMotion(1);
	}
}

//============================================================================
// �_���[�W��炢
//============================================================================
void CBoss::DamageBack()
{
	if (GetNowMotion() != 3)
	{ // �_���[�W���[�V�������Đ����Ă��Ȃ����

		// �{�X���_���[�W���[�V�����ɕύX
		SetNowMotion(3);

		// �{�X�̃_���[�W����炷
		CSound::GetInstance()->Play(CSound::LABEL::BDAMAGE);
	}
	else if (GetStopState() == true)
	{ // �_���[�W���[�V�������Đ��I��������

		// �s���L���X�g�J�E���g�����Z�b�g
		m_nCntActionCast = 0;

		// �p���J�E���g�����ɖ߂�
		m_nDuration = 0;

		// �ڕW���W�𒆉��֖߂�
		Vec3 Pos = VEC3_INIT;
		Pos.y = 150.0f;
		SetPosTarget(Pos);

		// �����ҋ@�ɖ߂�
		m_ActionType = ACTION::HOLDCENTER;

		// �ҋ@���[�V�����ɖ߂�
		SetNowMotion(1);
	}
}

//============================================================================
// �Փˌ��o
//============================================================================
bool CBoss::HitCheck()
{
	// �v���C���[�̃p�����[�^���R�s�[
	CPlayer* pPlayer = nullptr;
	pPlayer = utility::DownCast(pPlayer, CObject::FindSpecificObject(CObject::TYPE::PLAYER));
	const CBounding_Cylinder* Other = pPlayer->GetBndCylinder();

	// �����ƃv���C���[�̉~���o�E���f�B���O�Ŕ�ׂ�
	if (collision::HitCylinderToCylinder(m_pBndCylinder, Other))
		return true;

	return false;
}

//============================================================================
// ���S
//============================================================================
void CBoss::DeadEnd()
{
	if (GetNowMotion() != 4)
	{ // ���S���[�V�������Đ����Ă��Ȃ����

		// �{�X�����S���[�V�����ɕύX
		SetNowMotion(4);

		// �����ֈړ�
		Vec3 Pos = VEC3_INIT;
		Pos.y = 150.0f;
		SetPosTarget(Pos);
	}
	else
	{ // ���S���[�V�����Đ���

		// �΂̕���Ҕ���
		for (int nCnt = 0; nCnt < 2; ++nCnt)
			CSparks::FuryGenerate();

		// �J���������E���Փx�����������ύX
		{
			CCamera* pCamera = CManager::GetManager()->GetCamera();
			float fDinstance = pCamera->GetDistance(), fUpAdjust = pCamera->GetUpAdjust();
			fDinstance += (800.0f - fDinstance) * 0.1f;
			fUpAdjust += (300.0f - fUpAdjust) * 0.05f;
			pCamera->SetDistance(fDinstance);
			pCamera->SetUpAdjust(fUpAdjust);

			// �J������U��
			pCamera->SetVibration(0.05f);
		}

		// ���[�V�����̍Đ����I��������Q�[���V�[���ɃQ�[���I����ʒm����
		if (GetStopState())
		{
			// �Q�[���V�[���擾
			CGame* const pScene = dynamic_cast<CGame*>(CScene_Manager::GetInstance()->GetScene());

			// �V�[���J�ڊJ�n
			pScene->SetTransition();
		}
	}
}