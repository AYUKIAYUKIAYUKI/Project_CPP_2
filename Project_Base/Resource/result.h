//============================================================================
// 
// ���U���g�A�w�b�_�t�@�C�� [result.h]
// Author : ���c����
// 
//============================================================================

#ifndef _RESULT_H_
#define _RESULT_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "scene.h"
#include "spline_test.h"

//****************************************************
// ���U���g�N���X
//****************************************************
class CResult : public CScene
{
public:

	CResult();				// �R���X�g���N�^
	~CResult() override;	// �f�X�g���N�^

	HRESULT Init() override;	// �����ݒ�
	void Uninit() override;		// �I������
	void Update() override;		// �X�V����
	void Draw() override;		// �`�揈��

private:

	CSpline_Test* m_pSpline;	// �X�v���C���e�X�g
};

#endif // _RESULT_H_