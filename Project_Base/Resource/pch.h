//============================================================================
// 
// �v���R���p�C���w�b�_�[ [pch.h]
// Author : ���c����
// 
//============================================================================

#ifndef _PCH_H_
#define _PCH_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include <assert.h>
#include <stdio.h>
#include <windows.h>	// Windows�A�v���P�[�V��������p

// DirectX9
#include <d3dx9.h>					// DirectX9����p
#define DIRECTINPUT_VERSION	0x0800	// DirectInput�̃o�[�W����
#include <dinput.h>					// DirectInput����p
#include <xinput.h>					// �R���g���[���[����p
#include <xaudio2.h>				// �T�E���h����p

// JSON
#include <nlohmann/json.hpp>

// �W�����C�u����
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

// �֗��֐�
#include "utility.h"

//****************************************************
// ���C�u�����̃����N
//****************************************************
#pragma comment(lib, "winmm.lib")
#pragma	comment(lib, "d3d9.lib")	// �`�揈���p
#pragma	comment(lib, "d3dx9.lib")	// [d3d9.lib]�̊g�����C�u����
#pragma	comment(lib, "dxguid.lib")	// DirectX�̃R�[�|�l���g�g�p�̂���
#pragma	comment(lib, "dinput8.lib")	// DirectInput����p
#pragma comment(lib, "xinput.lib")	// �R���g���[������p

//****************************************************
// �}�N����`
//****************************************************

// ���������[�N���o�p
#define _CRTDBG_MAP_ALLOC

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

// ���_�t�H�[�}�b�g
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//****************************************************
// �萔��錾
//****************************************************

/// <summary> �X�N���[������ </summary>
static constexpr WORD SCREEN_WIDTH = 1980;

/// <summary> �X�N���[���c�� </summary>
static constexpr WORD SCREEN_HEIGHT = 1080;

//****************************************************
// ���O��Ԃ��`
//****************************************************
namespace abbr
{
	//****************************************************
	// using�錾
	//****************************************************
	using std::to_string;

	//****************************************************
	// �^�G�C���A�X���`
	//****************************************************
	using Vec2 = D3DXVECTOR2;
	using Vec3 = D3DXVECTOR3;
	using Mtx = D3DXMATRIX;

	/// <summary> D3DXVECTOR2�������p </summary>
	static const Vec2 VEC2_INIT = Vec2(0.0f, 0.0f);

	/// <summary> D3DXVECTOR3�������p </summary>
	static const Vec3 VEC3_INIT = Vec3(0.0f, 0.0f, 0.0f);
}

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using JSON = nlohmann::json;

//****************************************************
// 2D�|���S�����_�t�H�[�}�b�g�̒�`
//****************************************************
struct VERTEX_2D
{
	abbr::Vec3	pos;	// ���_���W
	float		rhw;	// ���Z��
	D3DCOLOR	col;	// ���_�J���[
	abbr::Vec2	tex;	// �e�N�X�`�����W
};

//****************************************************
// 3D�|���S�����_�t�H�[�}�b�g�̒�`
//****************************************************
struct VERTEX_3D
{
	abbr::Vec3	pos;	// ���_���W
	abbr::Vec3	nor;	// �@���x�N�g��
	D3DCOLOR	col;	// ���_�J���[
	abbr::Vec2	tex;	// �e�N�X�`�����W
};

#endif	_PCH_H_