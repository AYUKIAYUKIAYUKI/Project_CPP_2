//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================

#ifndef _SOUND_H_
#define _SOUND_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �T�E���h�N���X
//****************************************************
class CSound final
{
public:

	//****************************************************
	// �T�E���h�ꗗ
	//****************************************************
	enum class LABEL
	{
		TEST = 0,	// �e�X�g
		MAX,
	};

	HRESULT Init(HWND hWnd);		// �����ݒ�
	void Release();					// ���
	HRESULT Play(LABEL label);		// �Đ�
	void Stop(LABEL label);			// ��~ (�I��)
	void Stop();					// ��~ (�S��)
	void SetVol(LABEL label);		// ���ʂ�ݒ�
	float GetVol(LABEL label);		// ���ʂ��擾

	static CSound* GetInstance();	// �T�E���h���擾

private:

	//****************************************************
	// �T�E���h���̍\����
	//****************************************************
	struct SOUNDINFO
	{
		const char* pFilename;	// �t�@�C����
		int nCntLoop;			// ���[�v�J�E���g
		float fVolume;			// ����
	};

	CSound();	// �R���X�g���N�^
	~CSound();	// �f�X�g���N�^

	void Create();																					// ����
	void Uninit();																					// �I������
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);	// �`�����N�̃`�F�b�N
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);	// �`�����N�f�[�^�̓ǂݍ���

	IXAudio2* m_pXAudio2;												// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice* m_pMasteringVoice;							// �}�X�^�[�{�C�X
	IXAudio2SourceVoice* m_apSourceVoice[static_cast<int>(LABEL::MAX)];	// �\�[�X�{�C�X
	BYTE* m_apDataAudio[static_cast<int>(LABEL::MAX)];					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[static_cast<int>(LABEL::MAX)];					// �I�[�f�B�I�f�[�^�T�C�Y
	SOUNDINFO m_aSoundInfo[static_cast<int>(LABEL::MAX)];				// �T�E���h��� (��)

	static CSound* m_pSound;	// �T�E���h
};

#endif	// _SOUND_H_
