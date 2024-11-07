//============================================================================
// 
// 便利関数、ヘッダファイル [utility.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _UTILITY_H_
#define _UTILITY_H_	// 二重インクルード防止

//****************************************************
// 便利関数クラス
//****************************************************
class CUtility final
{
public:

	// 角度の差の補正(ラジアン)
	static void AdjustAngle(float& fAngle, const float& fDest);

	// 角度の差の補正(ラジアン)
	static void AdjustAngle(float& fAngle, const float& fDest, const float& fRadius);

	// 向きの範囲の補正(ラジアン)
	static void AdjustDirection(float& fAngle);

	// 向きの範囲の補正(ラジアン)
	static void AdjustDirection(float& fAngle1, float& fAngle2);

	// 球どうしの衝突判定
	static bool OnlySphere(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const D3DXVECTOR3& PosTarget, const float& fRadiusTarget);

	// 円柱と点の衝突判定
	static bool CylinderAndPoint(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget);

	// 円柱と球の衝突判定
	static bool CylinderAndSphere(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget, const float& fRadiusTarget);

	// 円柱とAABBの衝突判定
	static bool CylinderAndAABB(const D3DXVECTOR3& PosSelf, const float& fRadiusSelf, const float& fHeight, const D3DXVECTOR3& PosTarget, const D3DXVECTOR3& SizeTarget);

	// Jsonファイルの展開
	static nlohmann::json OpenJsonFile(std::string FilePath);

	// 乱数生成
	template <typename T> static T GetRandomValue();

	// ダウンキャスト
	template <typename T1, typename T2> static T1* DownCast(T1* pDest, T2* pBase);
};

//============================================================================
// 乱数生成
//============================================================================
template <typename T> T CUtility::GetRandomValue()
{
	// 数値の取る範囲を設定
	const int nRange = 180;

	// 倍の範囲からランダムな整数を生成
	int nRandom{ rand() % (nRange * 2) };

	// もとの範囲を超えていた場合正負を反転
	if (nRandom > nRange)
	{
		nRandom %= nRange;
		nRandom *= -1;
	}

	// 整数を指定された型にキャスト
	return static_cast<T>(nRandom);
}

//============================================================================
// ダウンキャスト
//============================================================================
template <typename T1, typename T2> T1* CUtility::DownCast(T1* pDest, T2* pBase)
{
	pDest;

	// 引数のオブジェクトを指定されたクラスにダウンキャスト
	T1* pObj{ dynamic_cast<T1*>(pBase) };

	// ダウンキャスト失敗
	if (!pObj)
	{
		assert(false);
	}

	return pObj;
}

#endif // _UTILITY_H_