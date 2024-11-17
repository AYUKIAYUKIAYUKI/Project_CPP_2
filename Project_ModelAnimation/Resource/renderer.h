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

	// <special function>
	CRenderer(const CRenderer&) = delete;				// �R�s�[�R���X�g���N�^
	CRenderer& operator=(const CRenderer&) = delete;	// �R�s�[������Z�q
	CRenderer(CRenderer&&) = delete;					// ���[�u�R���X�g���N�^
	CRenderer& operator=(CRenderer&&) = delete;			// ���[�u������Z�q

	// <function>
	void Update();		// �X�V����
	void Draw();		// �`�揈��
	void PrintDebug();	// �f�o�b�O�\��

	// <static function>
	static HRESULT	Create(HWND hWnd, BOOL bWindiw);	// ����
	static void		Release();							// �j��

	// <static getter>
	static CRenderer*			GetRenderer();	// �����_���[���擾
	static LPDIRECT3DDEVICE9	GetDeviece();	// �f�o�C�X���擾

	// <static setter>
	static void	SetDebugString(std::string Str);			// �f�o�b�O������ɒǉ�
	static void	SetTimeString(std::string Str, int nCnt);	// �������f�o�b�O������ɒǉ�

private:

	// <special function>
	CRenderer();	// �R���X�g���N�^
	~CRenderer();	// �f�X�g���N�^

	// <function>
	HRESULT Init(HWND hWnd, BOOL bWindiw);	// �����ݒ�
	void	Uninit();						// �I������

	// <data>
	LPDIRECT3D9									m_pD3D;			// Direct3D
	LPDIRECT3DDEVICE9							m_pD3DDevice;	// �f�o�C�X
	LPD3DXFONT									m_pFont;		// �t�H���g
	std::string									m_DebugStr;		// �\���p������
	std::vector<std::pair<std::string, int>>	m_TimeStr;		// ������������

	// <static data>
	static CRenderer* m_pRenderer;	// �����_���[�̖{��
};

#endif // _RENDERER_H_