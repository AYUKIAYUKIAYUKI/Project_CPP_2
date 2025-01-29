//============================================================================
// 
// �t�B�[���h�r���_�[�A�w�b�_�t�@�C�� [field_builder.h]
// Author : ���c����
// 
//============================================================================

#ifndef _FIELD_BUILDER_H_
#define _FIELD_BUILDER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �O���錾
//****************************************************
class CField_Type;
class CPlayer;
class CFan;

//****************************************************
// �t�B�[���h�r���_�[�N���X
//****************************************************
class CField_Builder
{
public:

	// <special function>
	CField_Builder(const CField_Builder&) = delete;				// �R�s�[�R���X�g���N�^
	CField_Builder& operator=(const CField_Builder&) = delete;	// �R�s�[������Z�q
	CField_Builder(CField_Builder&&) = delete;					// ���[�u�R���X�g���N�^
	CField_Builder& operator=(CField_Builder&&) = delete;		// ���[�u������Z�q

	// <function>
	void Release();						// ���
	void Update();						// �X�V����
	void Draw();						// �`�揈��
	void IncrementCntJump();			// �W�����v�����񐔂̃C���N�������g
	void IncrementCntDash();			// �_�b�V�������񐔂̃C���N�������g
	void IncrementCntSlash();			// �U�������񐔂̃C���N�������g
	bool DetectInFanRange(D3DXVECTOR3);	// ��`�͈͓��ɑ��݂��Ă��邩����

	// <getter>
	int GetCntDestroyBlock();	// �u���b�N�̔j�󐔂��擾

	// <setter>
	void SetSyncPlayer(const CPlayer* const pPlayer);	// �v���C���[���Z�b�g

	// <static function>
	static CField_Builder* Create();	// ����

private:

	//****************************************************
	// �A�N�V�����f�[�^�\���̂̒�`
	//****************************************************
	struct ActionData
	{
		int nCntJump;	// �W�����v������
		int nCntDash;	// �_�b�V��������
		int nCntSlash;	// �U��������
	};

	// <special function>
	CField_Builder();	// �R���X�g���N�^
	~CField_Builder();	// �f�X�g���N�^

	// <function>
	HRESULT Init();								// �����ݒ�
	void	Uninit();							// �I������
	void	UpdateFan();						// ��`�̍X�V
	void	UpdateBuilder();					// �r���_�[�̍X�V
	void	BranchFieldType();					// �t�B�[���h�^�C�v����
	void	GenerateItem();						// �A�C�e���̎�������
	bool	DetectNearBlock(D3DXVECTOR3 Pos);	// �אڂ������u���b�N�����o
	void	DestroyCheck();						// �j�󔻒�
	void	DestroyAllBlock();					// �S�u���b�N�̍폜
	void	PrintDebug();						// �f�o�b�O�\��

	// <data>
	std::unique_ptr<CField_Type> m_upFieldType;			// �t�B�[���h�^�C�v
	ActionData					 m_ActionData;			// �A�N�V�����f�[�^
	int							 m_nCntDestroyBlock;	// �u���b�N�̔j��
	const CPlayer*				 m_pSyncPlayer;			// �v���C���[�̃|�C���^
	CFan*						 m_pFan;				// ��`�͈�
};

#endif // _FIELD_BUILEDER_H_