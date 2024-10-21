//============================================================================
// 
// �t�B�[���h�}�l�[�W���[�A�w�b�_�t�@�C�� [field_manager.h]
// Author : ���c����
// 
//============================================================================

#ifndef _FIELD_MANAGER_H_
#define _FIELD_MANAGER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_X.h"

//****************************************************
// �t�B�[���h�}�l�[�W���[�N���X
//****************************************************
class CField_Manager final
{
public:

	/// <summary> �t�B�[���h���a </summary>
	static constexpr float FIELD_RADIUS = 300.0f;

	HRESULT Init();	// �����ݒ�
	void Release();	// ���
	void Update();	// �X�V����

	static CField_Manager* GetInstance();	// ���C���X�^���X���擾

private:

	CField_Manager();	// �f�t�H���g�R���X�g���N�^
	~CField_Manager();	// �f�X�g���N�^

	void Create();			// ����
	void Uninit();			// �I������
	void TestMethod();		// ���̐������\�b�h
	void TestDelete();		// ���̔j�����\�b�h
	void TestDeleteAll();	// ���̑S�j�����\�b�h

	CObject_X* m_pCylinderCollider;	// cylinder collider

	static CField_Manager* m_pInstance;	// ���C���X�^���X
};

#endif // _FIELD_MANAGER_H_