//============================================================================
//
// �o�E���f�B���O�X�t�B�A [bouding_sphere.cpp]
// Author : ���c����
//
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "bounding_sphere.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace abbr;

//============================================================================
//
// public�����o
//
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CBounding_Sphere::CBounding_Sphere() :
	m_fRadius{ 0.0f }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CBounding_Sphere::~CBounding_Sphere()
{

}

//============================================================================
// ���a���擾
//============================================================================
const float& CBounding_Sphere::GetRadius() const
{
	return m_fRadius;
}

//============================================================================
// ���a��ݒ�
//============================================================================
void CBounding_Sphere::SetRadius(float fRad)
{
	m_fRadius = fRad;
}