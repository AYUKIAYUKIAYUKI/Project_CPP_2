//============================================================================
// 
// �����_���[�A�w�b�_�t�@�C�� [renderer.h]
// Author : ���c����
// 
//============================================================================

#ifndef _RENDERER_H_
#define _RENDERER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �����_���[�N���X
//****************************************************
class CRenderer final
{
public:

	// <function>
	HRESULT Init(HWND hWnd, BOOL bWindiw);	// �����ݒ�
	void	Release();						// ���
	void	Update();						// �X�V����
	void	Draw();							// �`�揈��
	void	PrintDebug();					// �f�o�b�O�\��

	// <static getter>
	static CRenderer*			GetInstance();	// �����_���[���擾
	static LPDIRECT3DDEVICE9	GetDeviece();	// �f�o�C�X���擾

	// <static setter>
	static void	SetDebugString(std::string str);			// �f�o�b�O������ɒǉ�
	static void	SetTimeString(std::string str, int nCnt);	// �������f�o�b�O������ɒǉ�

private:

	// <special function>
	CRenderer();	// �R���X�g���N�^
	~CRenderer();	// �f�X�g���N�^

	// <function>
	void Create();	// ����
	void Uninit();	// �I������

	// <data>
	LPDIRECT3D9									m_pD3D;			// Direct3D
	LPDIRECT3DDEVICE9							m_pD3DDevice;	// �f�o�C�X
	LPD3DXFONT									m_pFont;		// �t�H���g
	std::string									m_debugStr;		// �\���p������
	std::vector<std::pair<std::string, int>>	m_timeStr;		// ������������

	// <static data>
	static CRenderer* m_pRenderer;	// �����_���[�̖{��
};

#endif // _RENDERER_H_