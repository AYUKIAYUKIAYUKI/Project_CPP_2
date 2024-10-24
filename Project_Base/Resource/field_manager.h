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
// ���O��Ԃ��`
//****************************************************
namespace field_manager
{
	//****************************************************
	// using�f�B���N�e�B�u
	//****************************************************
	using namespace abbr;

	//****************************************************
	// �t�B�[���h�}�l�[�W���[�N���X
	//****************************************************
	class CField_Manager final
	{
	public:

		/// <summary> �t�B�[���h���a </summary>
		static constexpr float FIELD_RADIUS = 300.0f;

		/// <summary> �����͈͂̔��a </summary>
		static constexpr float GENERATE_RANGE_RADIUS = 350.0f;

		/// <summary> �ő�u���b�N�� </summary>
		static constexpr int MAX_BLOCK = 100;

		/// <summary> �d�� </summary>
		static constexpr float FIELD_GRAVITY = -0.025f;

		HRESULT Init();	// �����ݒ�
		void Release();	// ���
		void Update();	// �X�V����

		static CField_Manager* GetInstance();	// ���C���X�^���X���擾

	private:

		CField_Manager();	// �f�t�H���g�R���X�g���N�^
		~CField_Manager();	// �f�X�g���N�^

		void Create();			// ����
		void Uninit();			// �I������
		void TestCreate();		// ���̐������\�b�h
		void TestDelete();		// ���̔j�����\�b�h
		void TestDeleteAll();	// ���̑S�j�����\�b�h

		float m_fCoeffRaondomRange;	// �����_���͈͂̋��x

		static CField_Manager* m_pInstance;	// ���C���X�^���X

		CObject_X* m_pCylinderCollider;	// cylinder collider
	};
}

#endif // _FIELD_MANAGER_H_