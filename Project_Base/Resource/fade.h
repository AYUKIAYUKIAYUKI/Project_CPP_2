//============================================================================
// 
// ���t�F�[�h�J�ڏ����A�w�b�_�t�@�C�� [fade.h]
// Author : ���c����
// 
//============================================================================

#ifndef _FADE_H_
#define _FADE_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "scene.h"

//****************************************************
// �t�F�[�h�N���X
//****************************************************
class CFade
{
public:

	HRESULT Init();	// �����ݒ�
	void Release();	// ���
	void Update();	// �X�V����
	void Draw();	// �`�揈��

	static void SetFade(CScene::MODE Mode);	// �t�F�[�h�ݒ�
	static CFade* GetInstance();			// �t�F�[�h���擾

private:

	CFade();	// �f�t�H���g�R���X�g���N�^
	~CFade();	// �f�X�g���N�^

	void Create();	// ����
	void Uninit();	// �I������
	void FadeIn();	// �t�F�[�h�C��
	void FadeOut();	// �t�F�[�h�A�E�g
	void SetVtx();	// ���_���̐ݒ�

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�̃|�C���^
	CScene::MODE m_NextMode;			// ���̃��[�h
	bool m_bFadeIn;						// �t�F�[�h�C������
	bool m_bFadeOut;					// �t�F�[�h�A�E�g����
	float m_fBrightness;				// ���x

	static CFade* m_pInstnce;	// ���C���X�^���X
};

#endif // _FADE_H_