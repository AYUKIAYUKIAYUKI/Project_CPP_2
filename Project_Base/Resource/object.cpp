//============================================================================
// 
// �I�u�W�F�N�g�Ǘ� [object.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.h"

// �f�o�b�O�\���p
#include "renderer.h"

//****************************************************
// �ÓI�����o�̏�����
//****************************************************
int CObject::m_nNumAll = 0;										// �I�u�W�F�N�g����
CObject* CObject::m_pTop[static_cast<int>(LAYER::MAX)] = {};	// �擪�I�u�W�F�N�g�̃|�C���^
CObject* CObject::m_pCur[static_cast<int>(LAYER::MAX)] = {};	// �I�[�I�u�W�F�N�g�̃|�C���^

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CObject::CObject() :
	m_nPriority{ static_cast<int>(LAYER::MAX) - 1 },	// �`��D��x
	m_pPrev{ nullptr },									// �O�̃I�u�W�F�N�g�̃|�C���^
	m_pNext{ nullptr },									// ���̃I�u�W�F�N�g�̃|�C���^
	m_type(TYPE::NONE),									// �^�C�v����
	m_bDeath{ false }									// ���S�t���O
{
	// ���̃I�u�W�F�N�g�����X�g�ɓo�^
	if (m_pCur[m_nPriority] == nullptr)
	{ // �I�[�I�u�W�F�N�g������ (�I�u�W�F�N�g��1�����݂��Ȃ�)

		// ���̃I�u�W�F�N�g��擪�ƏI�[�ɓo�^
		m_pTop[m_nPriority] = this;
		m_pCur[m_nPriority] = this;
	}
	else
	{ // �I�[�I�u�W�F�N�g������

		// ���݂̏I�[�����̃I�u�W�F�N�g�̑O�Ƃ��ēo�^
		m_pPrev = m_pCur[m_nPriority];

		// �V���ȏI�[�Ƃ��Ă��̃I�u�W�F�N�g��o�^
		m_pCur[m_nPriority] = this;

		// �O�̃I�u�W�F�N�g�̎��ɂ��̃I�u�W�F�N�g��o�^
		m_pPrev->m_pNext = this;
	}

	// �������J�E���g�A�b�v
	m_nNumAll++;

	//// �펯�I�ȃI�u�W�F�N�g�̐������x�𒴂�����
	//if (m_nNumAll > MAX_OBJ)
	//{
	//	assert(false);
	//}
}

//============================================================================
// �v���C�I���e�B�w��R���X�g���N�^
//============================================================================
CObject::CObject(int nPriority) :
	m_nPriority{ nPriority },	// �`��D��x
	m_pPrev{ nullptr },			// �O�̃I�u�W�F�N�g�̃|�C���^
	m_pNext{ nullptr },			// ���̃I�u�W�F�N�g�̃|�C���^
	m_type(TYPE::NONE),			// �^�C�v����
	m_bDeath{ false }			// ���S�t���O
{
	// ���̃I�u�W�F�N�g�����X�g�ɓo�^
	if (m_pCur[nPriority] == nullptr)
	{ // �I�[�I�u�W�F�N�g������ (�I�u�W�F�N�g��1�����݂��Ȃ�)
		
		// ���̃I�u�W�F�N�g��擪�ƏI�[�ɓo�^
		m_pTop[nPriority] = this;
		m_pCur[nPriority] = this;
	}
	else
	{ // �I�[�I�u�W�F�N�g������

		// ���݂̏I�[�����̃I�u�W�F�N�g�̑O�Ƃ��ēo�^
		m_pPrev = m_pCur[nPriority];

		// �V���ȏI�[�Ƃ��Ă��̃I�u�W�F�N�g��o�^
		m_pCur[nPriority] = this;

		// �O�̃I�u�W�F�N�g�̎��ɂ��̃I�u�W�F�N�g��o�^
		m_pPrev->m_pNext = this;
	}

	// �������J�E���g�A�b�v
	m_nNumAll++;

	// �I�u�W�F�N�g�̐������x�𒴂�����
	if (m_nNumAll > 99999999)
	{
		assert(false);
	}
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
	int nPriority = m_nPriority;	// �v���C�I���e�B���R�s�[���Ă���

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
CObject::TYPE CObject::GetType()
{
	return m_type;
}

//============================================================================
// �^�C�v��ݒ�
//============================================================================
void CObject::SetType(TYPE type)
{
	m_type = type;
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
	for (int nCntPriority = 0; nCntPriority < static_cast<int>(LAYER::MAX); nCntPriority++)
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
#ifdef _DEBUG
	// �I�u�W�F�N�g����\��
	CRenderer::GetInstance()->SetDebugString("���݂̃I�u�W�F�N�g��:" + std::to_string(m_nNumAll));
#endif	// _DEBUG

	for (int nCntPriority = 0; nCntPriority < static_cast<int>(LAYER::MAX); nCntPriority++)
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
	for (int nCntPriority = 0; nCntPriority < static_cast<int>(LAYER::MAX); nCntPriority++)
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
}

//============================================================================
// �S�`�揈��
//============================================================================
void CObject::DrawAll()
{
	for (int nCntPriority = 0; nCntPriority < static_cast<int>(LAYER::MAX); nCntPriority++)
	{
		// �擪�I�u�W�F�N�g�̃|�C���^���R�s�[
		CObject* pObj = m_pTop[nCntPriority];

		// ���̃I�u�W�F�N�g�������Ȃ�܂�
		while (pObj != nullptr)
		{
			// ���̃I�u�W�F�N�g�̃|�C���^���R�s�[
			CObject* pNext = pObj->m_pNext;

			// �`�揈��
			pObj->Draw();

			// ���̃I�u�W�F�N�g�̃|�C���^���R�s�[
			pObj = pNext;
		}
	}
}

//============================================================================
// �擪�I�u�W�F�N�g�̃|�C���^�擾
//============================================================================
CObject* CObject::GetObject(int nPriority)
{
	return m_pTop[nPriority];
}

//============================================================================
// ����^�C�v�̃I�u�W�F�N�g��T��
//============================================================================
CObject* CObject::FindObject(TYPE type)
{
	for (int nCntPriority = 0; nCntPriority < static_cast<int>(LAYER::MAX); nCntPriority++)
	{
		// �擪�I�u�W�F�N�g�̃|�C���^���R�s�[
		CObject* pObj = m_pTop[nCntPriority];

		// ���̃I�u�W�F�N�g�������Ȃ�܂�
		while (pObj != nullptr)
		{
			if (pObj->GetType() == type)
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