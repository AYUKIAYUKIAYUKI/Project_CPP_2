//============================================================================
// 
// �V�[���Ǘ��A�w�b�_�t�@�C�� [scene.h]
// Author : ���c����
// 
//============================================================================

#ifndef _SCENE_H_
#define _SCENE_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �V�[���Ǘ��N���X
//****************************************************
class CScene
{
public:

	//****************************************************
	// �V�[���̎��
	//****************************************************
	enum class MODE
	{
		NONE = 0,	// ����
		TITLE,		// �^�C�g��
		GAME,		// �Q�[��
		RESULT,		// ���U���g
		MAX,
	};

	CScene();			// �R���X�g���N�^
	virtual ~CScene();	// �f�X�g���N�^

	virtual HRESULT Init();	// �����ݒ�
	virtual void Uninit();	// �I������
	virtual void Update();	// �X�V����
	virtual void Draw();	// �`�揈��

	MODE GetMode();	// ���݂̃��[�h���擾����

	static CScene* Create(MODE mode);	// ����

private:

	MODE m_mode;	// ���݂̃��[�h
};

#endif // _SCENE_H_