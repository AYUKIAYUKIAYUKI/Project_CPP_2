//============================================================================
// 
// �I�u�W�F�N�g�Ǘ� [object.cpp]
// Author : ���c����
// 
//============================================================================

/* ����̓X�e���V���o�b�t�@�̕`��e�X�g�p */
namespace
{
	int
	StencilRefValue = 0,			// �X�e���V���Q�ƒl
	StencilCmp = D3DCMP_ALWAYS,		// �X�e���V���l�̔�r���@
	Pass = D3DSTENCILOP_KEEP,		// Z�e�X�g�E�X�e���V���e�X�g�ɐ���
	ZFail = D3DSTENCILOP_KEEP,		// Z�e�X�g�̂ݎ��s
	Fail = D3DSTENCILOP_KEEP;		// Z�e�X�g�E�X�e���V���e�X�g�Ɏ��s
}

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.h"
#include "renderer.h"

// �X�e���V���e�X�g�p
#include "manager.h"
#include "mask_rectangle.h"

//****************************************************
// �ÓI�����o�̏�����
//****************************************************
int		 CObject::m_nNumAll = 0;								// �I�u�W�F�N�g����
CObject* CObject::m_pTop[static_cast<WORD>(LAYER::MAX)] = {};	// �擪�I�u�W�F�N�g�̃|�C���^
CObject* CObject::m_pCur[static_cast<WORD>(LAYER::MAX)] = {};	// �I�[�I�u�W�F�N�g�̃|�C���^

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CObject::CObject() :
	m_wPriority{ static_cast<WORD>(LAYER::BG) },
	m_pPrev{ nullptr },
	m_pNext{ nullptr },
	m_Type{ TYPE::NONE },
	m_bDeath{ false }
{
	// ���̃I�u�W�F�N�g�����X�g�ɓo�^
	if (m_pCur[m_wPriority] == nullptr)
	{ // �I�[�I�u�W�F�N�g������ (�I�u�W�F�N�g��1�����݂��Ȃ�)

		// ���̃I�u�W�F�N�g��擪�ƏI�[�ɓo�^
		m_pTop[m_wPriority] = this;
		m_pCur[m_wPriority] = this;
	}
	else
	{ // �I�[�I�u�W�F�N�g������

		// ���݂̏I�[�����̃I�u�W�F�N�g�̑O�Ƃ��ēo�^
		m_pPrev = m_pCur[m_wPriority];

		// �V���ȏI�[�Ƃ��Ă��̃I�u�W�F�N�g��o�^
		m_pCur[m_wPriority] = this;

		// �O�̃I�u�W�F�N�g�̎��ɂ��̃I�u�W�F�N�g��o�^
		m_pPrev->m_pNext = this;
	}

	// �������J�E���g�A�b�v
	m_nNumAll++;
}

//============================================================================
// �`��D��x�w��R���X�g���N�^
//============================================================================
CObject::CObject(LAYER Priority) :
	m_wPriority{ static_cast<WORD>(Priority) },
	m_pPrev{ nullptr },
	m_pNext{ nullptr },
	m_Type{ TYPE::NONE },
	m_bDeath{ false }
{
	// ���̃I�u�W�F�N�g�����X�g�ɓo�^
	if (m_pCur[m_wPriority] == nullptr)
	{ // �I�[�I�u�W�F�N�g������ (�I�u�W�F�N�g��1�����݂��Ȃ�)
		
		// ���̃I�u�W�F�N�g��擪�ƏI�[�ɓo�^
		m_pTop[m_wPriority] = this;
		m_pCur[m_wPriority] = this;
	}
	else
	{ // �I�[�I�u�W�F�N�g������

		// ���݂̏I�[�����̃I�u�W�F�N�g�̑O�Ƃ��ēo�^
		m_pPrev = m_pCur[m_wPriority];

		// �V���ȏI�[�Ƃ��Ă��̃I�u�W�F�N�g��o�^
		m_pCur[m_wPriority] = this;

		// �O�̃I�u�W�F�N�g�̎��ɂ��̃I�u�W�F�N�g��o�^
		m_pPrev->m_pNext = this;
	}

	// �������J�E���g�A�b�v
	m_nNumAll++;
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CObject::~CObject()
{
	
}

//============================================================================
// ����ݒ�
//============================================================================
void CObject::SetRelease()
{
	if (!m_bDeath)
	{
		m_bDeath = true;
	}
}

//============================================================================
// �������
//============================================================================
void CObject::Release()
{
	int nPriority = m_wPriority;	// �v���C�I���e�B���R�s�[���Ă���

	if (m_pPrev == nullptr)
	{ // �O�̃I�u�W�F�N�g������ (���̃I�u�W�F�N�g���擪)

		if (m_pNext == nullptr)
		{ // ���̃I�u�W�F�N�g������ (�I�u�W�F�N�g���S�Ė����Ȃ�)
			
			m_pTop[nPriority] = nullptr;	// �擪�I�u�W�F�N�g�̃|�C���^��������
			m_pCur[nPriority] = nullptr;	// �I�[�I�u�W�F�N�g�̃|�C���^��������
		}
		else
		{ // ���̃I�u�W�F�N�g�͂���

			// �V���Ȑ擪�Ƃ��Ď��̃I�u�W�F�N�g��o�^
			m_pTop[nPriority] = m_pNext;

			// ���̃I�u�W�F�N�g�̑O�̃|�C���^��������
			m_pNext->m_pPrev = nullptr;
		}
	}
	else
	{ // �O�̃I�u�W�F�N�g������

		if (m_pNext == nullptr)
		{ // ���̃I�u�W�F�N�g������ (���̃I�u�W�F�N�g���I�[)

			// �V���ȏI�[�Ƃ��đO�̃I�u�W�F�N�g�o�^
			m_pCur[nPriority] = m_pPrev;

			// �O�̃I�u�W�F�N�g�̎��̃|�C���^��������
			/* ���������u���̃I�u�W�F�N�g�̎��̃|�C���^(m_pNext)��n���v�Ƃ����s�ׂȂ�A���g�̗L���ɂ�����炸�Ȃ���̂ł� */
			m_pPrev->m_pNext = nullptr;
		}
		else
		{ // ���̃I�u�W�F�N�g������

			// �O�̃I�u�W�F�N�g�̎��ɂ��̃I�u�W�F�N�g�̎���o�^
			m_pPrev->m_pNext = m_pNext;

			// ���̃I�u�W�F�N�g�̑O��
			m_pNext->m_pPrev = m_pPrev;
		}
	}

	Uninit();		// �I������
	delete this;	// �����������
	m_nNumAll--;	// �������J�E���g�_�E��
}

//============================================================================
// �^�C�v���擾
//============================================================================
const CObject::TYPE& CObject::GetType() const
{
	return m_Type;
}

//============================================================================
// �^�C�v��ݒ�
//============================================================================
void CObject::SetType(TYPE Type)
{
	m_Type = Type;
}

//============================================================================
// �O�̃|�C���^���擾
//============================================================================
CObject* CObject::GetPrev()
{
	return m_pPrev;
}

//============================================================================
// ���̃|�C���^���擾
//============================================================================
CObject* CObject::GetNext()
{
	return m_pNext;
}

//============================================================================
// �S�������
//============================================================================
void CObject::ReleaseAll()
{
	for (int nCntPriority = 0; nCntPriority < static_cast<WORD>(LAYER::MAX); nCntPriority++)
	{
		// �擪�I�u�W�F�N�g�̃|�C���^���R�s�[
		CObject* pObj = m_pTop[nCntPriority];

		// ���̃I�u�W�F�N�g�������Ȃ�܂�
		while (pObj != nullptr)
		{
			// ���̃I�u�W�F�N�g�̃|�C���^���R�s�[
			CObject* pNext = pObj->m_pNext;

			// �������
			pObj->Release();

			// ���̃I�u�W�F�N�g�����̃|�C���^�ŏ㏑��
			pObj = pNext;
		}
	}
}

//============================================================================
// �S�X�V����
//============================================================================
void CObject::UpdateAll()
{
#ifdef _DEBUG	// �I�u�W�F�N�g����\��

	CRenderer::SetDebugString("���݂̃I�u�W�F�N�g��:" + std::to_string(m_nNumAll));

#endif	// _DEBUG

	for (int nCntPriority = 0; nCntPriority < static_cast<WORD>(LAYER::MAX); nCntPriority++)
	{
		// �擪�I�u�W�F�N�g�̃|�C���^���R�s�[
		CObject* pObj = m_pTop[nCntPriority];

		// ���̃I�u�W�F�N�g�������Ȃ�܂�
		while (pObj != nullptr)
		{
			// ���̃I�u�W�F�N�g�̃|�C���^���R�s�[
			CObject* pNext = pObj->m_pNext;

			// �X�V����
			pObj->Update();

			// ���̃I�u�W�F�N�g�̃|�C���^���R�s�[
			pObj = pNext;
		}
	}
}

//============================================================================
// �S��X�V����
//============================================================================
void CObject::LateUpdateAll()
{
	for (int nCntPriority = 0; nCntPriority < static_cast<WORD>(LAYER::MAX); nCntPriority++)
	{
		// �擪�I�u�W�F�N�g�̃|�C���^���R�s�[
		CObject* pObj = m_pTop[nCntPriority];

		// ���̃I�u�W�F�N�g�������Ȃ�܂�
		while (pObj != nullptr)
		{
			// ���̃I�u�W�F�N�g�̃|�C���^���R�s�[
			CObject* pNext = pObj->m_pNext;

			// ���S�t���O���o�Ă�����
			if (pObj->m_bDeath)
			{
				// �������
				pObj->Release();
			}

			// ���̃I�u�W�F�N�g�̃|�C���^���R�s�[
			pObj = pNext;
		}
	}

#ifdef _DEBUG
#if 0	/* �X�e���V���e�X�g�̐ݒ�ύX */
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Stencil - All Obect"))
	{
		D3DCMP_GREATEREQUAL;
		ImGui::InputInt("StencilRefValue", &StencilRefValue);
		ImGui::InputInt("StencilCmp", &StencilCmp);
		ImGui::InputInt("Pass", &Pass);
		ImGui::InputInt("ZFail", &ZFail);
		ImGui::InputInt("Fail", &Fail);
		ImGui::End();
	}
#endif
#endif // _DEBUG
}

//============================================================================
// �S�`�揈��
//============================================================================
void CObject::DrawAll()
{
	// �f�o�C�X���擾
	auto pDev = CRenderer::GetDeviece();

	// �X�e���V���o�b�t�@�ւ̏������݂�L����
	pDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	for (int nCntPriority = 0; nCntPriority < static_cast<WORD>(LAYER::MAX); nCntPriority++)
	{
		// �擪�I�u�W�F�N�g�̃|�C���^���R�s�[
		CObject* pObj = m_pTop[nCntPriority];

		// ���̃I�u�W�F�N�g�������Ȃ�܂�
		while (pObj != nullptr)
		{
			// ���̃I�u�W�F�N�g�̃|�C���^���R�s�[
			CObject* pNext = pObj->m_pNext;

			// �X�e���V���Q�ƒl��ݒ�
			pDev->SetRenderState(D3DRS_STENCILREF, StencilRefValue);

			// �X�e���V���o�b�t�@�̔�r���@��ύX
			pDev->SetRenderState(D3DRS_STENCILFUNC, StencilCmp);

			// �X�e���V���e�X�g�̌��ʂɑ΂��Ă̂ӂ�܂���ݒ肷��
			pDev->SetRenderState(D3DRS_STENCILPASS, Pass);		// Z�e�X�g�E�X�e���V���e�X�g�ɐ���
			pDev->SetRenderState(D3DRS_STENCILZFAIL, ZFail);	// Z�e�X�g�̂ݎ��s
			pDev->SetRenderState(D3DRS_STENCILFAIL, Fail);		// Z�e�X�g�E�X�e���V���e�X�g�Ɏ��s

			// ����I�u�W�F�N�g�ȊO�`�揈��
			if (pObj->GetType() != TYPE::LATEDRAW)
			{
				// �`�揈��
				pObj->Draw();
			}

			// ���̃I�u�W�F�N�g�̃|�C���^���R�s�[
			pObj = pNext;
		}
	}
	
#if 1	// ��ʂ𕢂��|���S���̕`��

	// Z�o�b�t�@�ɏ������܂Ȃ�
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// �X�e���V���Q�ƒl��ݒ�
	pDev->SetRenderState(D3DRS_STENCILREF, 1);

	// �X�e���V���o�b�t�@�̔�r���@��ύX
	pDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	// �X�e���V���e�X�g�̌��ʂɑ΂��Ă̂ӂ�܂���ݒ肷�� (Z�e�X�g�͂��Ȃ�)
	pDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);	// (Z�e�X�g�E)�X�e���V���e�X�g�ɐ���
	pDev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);	// (Z�e�X�g�E)�X�e���V���e�X�g�Ɏ��s

	// �}�X�N�|���S���̕`��
	CManager::GetManager()->GetMask_Rectangle()->Draw();

	// Z�o�b�t�@�̏������݂�L����
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// �X�e���V���o�b�t�@�ւ̏������݂𖳌���
	pDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);
#endif
}

//============================================================================
// �S��`�揈��
//============================================================================
void CObject::LateDrawAll()
{
	for (int nCntPriority = 0; nCntPriority < static_cast<WORD>(LAYER::MAX); nCntPriority++)
	{
		// �擪�I�u�W�F�N�g�̃|�C���^���R�s�[
		CObject* pObj = m_pTop[nCntPriority];

		// ���̃I�u�W�F�N�g�������Ȃ�܂�
		while (pObj != nullptr)
		{
			// ���̃I�u�W�F�N�g�̃|�C���^���R�s�[
			CObject* pNext = pObj->m_pNext;

			// ����I�u�W�F�N�g�̂ݕ`�揈��
			if (pObj->GetType() == TYPE::LATEDRAW)
			{
				pObj->Draw();
			}

			// ���̃I�u�W�F�N�g�̃|�C���^���R�s�[
			pObj = pNext;
		}
	}
}

//============================================================================
// �擪�I�u�W�F�N�g�̃|�C���^�擾
//============================================================================
CObject* CObject::GetTopObject()
{
	return *m_pTop;
}

//============================================================================
// �擪�I�u�W�F�N�g�̃|�C���^�擾
//============================================================================
CObject* CObject::GetTopObject(WORD wPriority)
{
	return m_pTop[wPriority];
}

//============================================================================
// �擪�I�u�W�F�N�g�̃|�C���^�擾
//============================================================================
CObject* CObject::GetTopObject(LAYER Priority)
{
	return m_pTop[static_cast<WORD>(Priority)];
}

//============================================================================
// ����^�C�v�̃I�u�W�F�N�g��T��
//============================================================================
CObject* CObject::FindSpecificObject(TYPE Type)
{
	for (int nCntPriority = 0; nCntPriority < static_cast<WORD>(LAYER::MAX); nCntPriority++)
	{
		// �擪�I�u�W�F�N�g�̃|�C���^���R�s�[
		CObject* pObj = m_pTop[nCntPriority];

		// ���̃I�u�W�F�N�g�������Ȃ�܂�
		while (pObj != nullptr)
		{
			if (pObj->GetType() == Type)
			{
				// �^�C�v��v�Ȃ�Ԃ�
				return pObj;
			}

			// ���̃I�u�W�F�N�g�̃|�C���^���R�s�[
			pObj = pObj->m_pNext;
		}
	}

	return nullptr;
}

//============================================================================
// ����^�C�v�̃I�u�W�F�N�g�����擾
//============================================================================
int CObject::CountSpecificObject(TYPE Type)
{
	// ���J�E���g�p
	int nCount = 0;

	for (int nCntPriority = 0; nCntPriority < static_cast<WORD>(LAYER::MAX); nCntPriority++)
	{
		// �擪�I�u�W�F�N�g�̃|�C���^���R�s�[
		CObject* pObj = m_pTop[nCntPriority];

		// ���̃I�u�W�F�N�g�������Ȃ�܂�
		while (pObj != nullptr)
		{
			if (pObj->GetType() == Type)
			{
				nCount++;
			}

			pObj = pObj->GetNext();
		}
	}

	return nCount;
}