//============================================================================
//
// フィールドタイプ-ノーマル [field_type_normal.cpp]
// Author : 福田歩希
//
//============================================================================

/* デバッグ用 */
extern float fTest;

//****************************************************
// インクルードファイル
//****************************************************
#include "field_type_normal.h"

#include "field_manager.h"
#include "block.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CField_Type_Normal::CField_Type_Normal() :
	m_LastModel{ CX_Manager::TYPE::BLONORMAL }
{

}

//============================================================================
// デストラクタ
//============================================================================
CField_Type_Normal::~CField_Type_Normal()
{

}

//============================================================================
// 更新処理
//============================================================================
void CField_Type_Normal::GenerateBlock(float fEdgeDirection)
{
	// ブロックのモデルタイプを事前に選出
	CX_Manager::TYPE Type = CX_Manager::TYPE::BLONORMAL + (rand() % 3);

	// 生成に使用する座標・向きを用意
	Vec3 NewPos = VEC3_INIT, NewRot = VEC3_INIT;

	{ // 生成用の座標を決定

		// ①左右の場所は境界線の位置に
		NewPos.x = cosf(fEdgeDirection) * CField_Manager::FIELD_RADIUS;
		NewPos.z = sinf(fEdgeDirection) * CField_Manager::FIELD_RADIUS;

		// ②高さはキリの良い数値になるよう調整
		NewPos.y = fabsf(utility::GetRandomValue<float>());
		NewPos.y = utility::RoundToAnyMultiple<float>(NewPos.y, 10, 5);
	}

	// 配置の安全性を検出、条件に反しているなら生成しない
	if (!DetectLayoutSafety(Type, NewPos))
	{
		return;
	}

	// 向きを決定
	NewRot.y = atan2f(-NewPos.x, -NewPos.z);

	// ブロックを生成し、ブロックモデルを変更する
	CBlock* pBlock = CBlock::Create(NewPos, NewRot);	// ブロックのインスタンス生成
	pBlock->BindModel(Type);							// モデルを割り当て
	pBlock->SetSize(pBlock->GetModel()->Size);			// バウンディングサイズを揃える

	// 最後に生成したモデルタイプを保持
	m_LastModel = Type;
}

//============================================================================
// 生成
//============================================================================
CField_Type_Normal* CField_Type_Normal::Create()
{
	CField_Type_Normal* pNewInstance = DBG_NEW CField_Type_Normal();

	// 生成失敗
	if (!pNewInstance)
	{
		assert(false);
	}

	return pNewInstance;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 配置の安全性を検出
//============================================================================
bool CField_Type_Normal::DetectLayoutSafety(CX_Manager::TYPE SelfType, D3DXVECTOR3 SelfPos)
{
	// 通常優先度のオブジェクトを取得
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	while (pObj != nullptr)
	{
		// ブロックタイプのオブジェクトのとき
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// オブジェクトをブロッククラスにダウンキャスト
			CBlock* pOther = nullptr;
			pOther = utility::DownCast(pOther, pObj);

			// 必要な情報を用意
			const CX_Manager::TYPE&
				OtherType = pOther->GetModelType();	// 対象のモデルタイプを取得
			const Vec3&
				Distance = pOther->GetPos() - SelfPos,	// お互いの距離の差
				OtherSize = pOther->GetSize();			// 対象のサイズ
			float
				DistanceNorm = (Distance.x * Distance.x + Distance.y * Distance.y + Distance.z * Distance.z);	// 距離の差の大きさ

			if (DistanceNorm <= CField_Type::GetAreaNorm() * fTest)
			{
				return false;
			}
		}

		// 次のオブジェクトへ
		pObj = pObj->GetNext();
	}

	return true;
}