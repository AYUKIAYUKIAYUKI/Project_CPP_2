//============================================================================
// 
// �e�N�X�`���}�l�[�W���[�A�w�b�_�t�@�C�� [texture_manager.h]
// Author : ���c����
// 
//============================================================================

#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �e�N�X�`���}�l�[�W���[�N���X
//****************************************************
class CTexture_Manager final
{
public:

	//****************************************************
	// �e�N�X�`������
	//****************************************************
	enum class TYPE : WORD
	{
		TEST0 = 0,			// �e�X�g
		TEST1,				// �e�X�g
		MAP,				// ���}�b�v
		CIRCLE,				// �T�[�N��
		PLAYERGAUGE,		// �v���C���[�̃Q�[�W
		PLAYERGAUGEWINDOW,	// �v���C���[�̃Q�[�W�E�B���h�E
		MAX,
	};

	// <special function>
	CTexture_Manager(const CTexture_Manager&) = delete;				// �R�s�[�R���X�g���N�^
	CTexture_Manager& operator=(const CTexture_Manager&) = delete;	// �R�s�[������Z�q
	CTexture_Manager(CTexture_Manager&&) = delete;					// ���[�u�R���X�g���N�^
	CTexture_Manager& operator=(CTexture_Manager&&) = delete;		// ���[�u������Z�q

	// <getter>
	LPDIRECT3DTEXTURE9 GetTexture(TYPE Type);	// �e�N�X�`�����擾

	// <static function>
	static HRESULT	Create();	// ����
	static void		Release();	// ���

	// <static getter>
	static CTexture_Manager* GetInstance();	// �e�N�X�`���}�l�[�W���[���擾

private:

	// <special function>
	CTexture_Manager();		// �R���X�g���N�^
	~CTexture_Manager();	// �f�X�g���N�^

	// <function>
	HRESULT	Init();		// �����ݒ�
	void	Uninit();	// �I������

	// <data>
	LPDIRECT3DTEXTURE9 m_apTexture[static_cast<WORD>(TYPE::MAX)];	// �e�N�X�`�����

	// <static data>
	static CTexture_Manager* m_pTextureManager;	// �e�N�X�`���}�l�[�W���[�̖{��
};

#endif // _TEXTURE_MANAGER_H_