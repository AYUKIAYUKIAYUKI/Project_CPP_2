//============================================================================
//
// フィールドタイプ-ノーマル [field_type_normal.cpp]
// Author : 福田歩希
//
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "field_type_normal.h"

#include "field_manager.h"
#include "field_builder.h"
#include "player.h"
#include "item.h"
#include "block.h"
#include "bright.h"
#include "enemy.h"
#include "monster.h"
#include "ghost.h"
#include "flyer.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

/* デバッグ用 - ブロックの密集度合 */
extern float fBlockDensity;

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
		NewPos.y = utility::RoundToAnyMultiple<float>(NewPos.y, 20, 9);

		{ // ③ブロックのタイプに応じさらに少し調整

			// 占有範囲をコピー
			float fRangeHeight = CField_Type::BLOCK_RANGE.z;

			// 縦長のブロック以外は範囲半減
			if (Type != CX_Manager::TYPE::BLOTALL)
			{
				fRangeHeight *= 0.5f;
			}

			// ブロックが地面に埋まってしまわないよう下限を設定
			if (NewPos.y < fRangeHeight)
			{
				NewPos.y = fRangeHeight;
			}
		}
	}

	// アイテムとの重複を検出
	if (DetectOverlapItem(NewPos))
	{
		return;
	}

	// ブロックとの重複を検出
	if (DetectOverlapBlock(Type, NewPos))
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

	// ブロックの破壊量が一定数に達していると
	if (CField_Manager::GetInstance()->GetFieldBuilder()->GetCntDestroyBlock() % 10 == 0)
	{
		// 隙間に敵を生成
		DetectGapForSetEnemy();
	}
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
// アイテムとの重複を検出
//============================================================================
bool CField_Type_Normal::DetectOverlapItem(D3DXVECTOR3 SelfPos)
{
	// アイテムタイプのオブジェクトを取得
	CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::ITEM);

	if (pObj)
	{
		// オブジェクトをアイテムクラスにダウンキャスト
		CItem* pItem = nullptr;
		pItem = utility::DownCast(pItem, pObj);

		// アイテムとの距離の差を作成
		const Vec3& Distance = pItem->GetPos() - SelfPos;

		// アイテムにブロックが接近してしまっていたら
		if (Distance.x * Distance.x + Distance.y * Distance.y + Distance.z * Distance.z <= CField_Type::GetAreaNorm() * 1.5f)
		{
			// 重複している、このブロックを生成しない
			return true;
		}
	}

	// 条件通過
	return false;
}

//============================================================================
// ブロックとの重複を検出
//============================================================================
bool CField_Type_Normal::DetectOverlapBlock(CX_Manager::TYPE SelfType, D3DXVECTOR3 SelfPos)
{
	// アイテムの下に足場が存在しているか判別
	bool bBlockUnderItem = false;

	// アイテムが扇形範囲内に存在しているか判別
	bool bInFanRange = false;

	// アイテムタイプのオブジェクトを取得
	CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::ITEM);

	// アイテムクラスのポインタを用意
	CItem* pItem = nullptr;

	if (pObj)
	{
		// オブジェクトをアイテムクラスにダウンキャスト
		pItem = utility::DownCast(pItem, pObj);
	}

	// ①アイテムの座標をコピー
	Vec3 ItemPos = pItem->GetPos();

	// ②アイテムが扇形範囲内に存在するか判定
	if (CField_Manager::GetInstance()->GetFieldBuilder()->DetectInFanRange(ItemPos))
	{
		// ③範囲内にアイテムがあることを記録
		bInFanRange = true;

		// ④キリの良い場所にアイテムの場所仮で固定する
		ItemPos.y += -40.0f;
		ItemPos.y = utility::RoundToAnyMultiple<float>(ItemPos.y, 20, 9);
	}

	// 通常優先度のオブジェクトを取得
	pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

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

			// 他のブロックとの距離がある程度近ければ条件を満たさない
			if (DistanceNorm <= CField_Type::GetAreaNorm() * fBlockDensity)
			{
				// 重複している、このブロックを生成しない
				return true;
			}

			// アイテムの足場となるブロックが存在しているなら
			if (ItemPos == pOther->GetPos())
			{
				bBlockUnderItem = true;
			}
		}

		// 次のオブジェクトへ
		pObj = pObj->GetNext();
	}

	// ⑤全てのブロックの中に足場となるものが存在せず、かつアイテムが扇形範囲内に収まっているとき
	if (!bBlockUnderItem && bInFanRange)
	{
		// 新しいを用意
		Vec3 NewRot = VEC3_INIT;

		// 向きを決定
		NewRot.y = atan2f(-ItemPos.x, -ItemPos.z);

		// ブロックを生成し、ブロックモデルを変更する
		CBlock* pBlock = CBlock::Create(ItemPos, NewRot);	// ブロックのインスタンス生成
		pBlock->BindModel(CX_Manager::TYPE::BLOSIDE);		// モデルを割り当て
		pBlock->SetSize(pBlock->GetModel()->Size);			// バウンディングサイズを揃える
	}

	// 条件通過
	return false;
}

//============================================================================
// エネミーが生成出来そうな隙間を検出
//============================================================================
void CField_Type_Normal::DetectGapForSetEnemy()
{
	/*
		エネミータイプを検索してモンスタークラスが無いときのみ横長ブロックの真下に一体生成
	*/

	// 通常優先度のオブジェクトを取得
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	while (pObj != nullptr)
	{
		// このオブジェクトのタイプをコピー
		const CObject::TYPE Type = pObj->GetType();

		// 閃光タイプのオブジェクトが存在していたら
		if (Type == CObject::TYPE::BRIGHT)
		{
			// 敵を生成せず終了
			return;
		}

		// エネミータイプのオブジェクトが存在していたら
		if (Type == CObject::TYPE::ENEMY)
		{
			// クラスを確認し、モンスターだった場合
			if (typeid(*pObj) == typeid(CMonster))
			{
				// 敵を生成せず終了
				return;
			}
		}

		// 次のオブジェクトへ
		pObj = pObj->GetNext();
	}

	// プレイヤータイプのオブジェクトを取得
	pObj = CObject::FindSpecificObject(CObject::TYPE::PLAYER);

	// 取得失敗で終了
	if (!pObj)
	{
		return;
	}

	// プレイヤークラスにダウンキャスト
	const CPlayer* pPlayer = utility::DownCast<CPlayer, CObject>(pObj);

	// 適所の座標を保持しておく
	Vec3 HoldBestPos = VEC3_INIT;

	// 距離の差の大きさを保持しておく
	float fDistanceNorm = 0.0f;

	// 通常優先度のオブジェクトを取得
	pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	while (pObj != nullptr)
	{
		// ブロックタイプのオブジェクトのとき
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// オブジェクトをブロッククラスにダウンキャスト
			CBlock* pAnyBlock = nullptr;
			pAnyBlock = utility::DownCast(pAnyBlock, pObj);

			// このブロックのモデルタイプを取得
			const CX_Manager::TYPE& Type = pAnyBlock->GetModelType();

			// 横長ブロックだった場合
			if (Type == CX_Manager::TYPE::BLOSIDE)
			{
				// ブロックの高さをコピー
				float fHeight = pAnyBlock->GetPos().y;

				// このブロックの高さが適しているか剪定
				if (fHeight >= 50.0f &&
					fHeight <= 70.0f)
				{
					// 距離の差を割り出す
					Vec3 Distance = pAnyBlock->GetPos() - pPlayer->GetPos();

					// 大きさを割り出す
					float fNorm = Distance.x * Distance.x + Distance.z * Distance.z;

					// より離れた場所にある場合
					if (fNorm > fDistanceNorm)
					{
						// その大きさを保持
						fDistanceNorm = fNorm;

						// 適所の座標を更新
						HoldBestPos = { pAnyBlock->GetPos() };
						HoldBestPos.y += -20.0f;
					}
				}
			}
		}

		// 次のオブジェクトへ
		pObj = pObj->GetNext();
	}

	// モンスターを生成
	CBright::Generate(HoldBestPos, CBright::CREATETYPE::MONSTER);
}