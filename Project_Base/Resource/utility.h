//============================================================================
// 
// 便利関数、ヘッダファイル [utility.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _UTILITY_H_
#define _UTILITY_H_	// 二重インクルード防止

//****************************************************
// 名前空間utilityを定義
//****************************************************
namespace utility
{
	// 象限に応じて、単位立方体の頂点の座標を作成
	D3DXVECTOR3 SetCubeVtxFromQuadrant(const WORD& wIdx);

	// 点をY軸で回転させる
	D3DXVECTOR3 RotatePointAroundY(const float& fDirection, const D3DXVECTOR3& Pos);

	// 箱をY軸で回転させる
	D3DXVECTOR3 RotateBoxAroundY(const WORD& wIdx, const float& fDirection, const D3DXVECTOR3& Size);

	// 箱のY軸の回転を打ち消す
	D3DXVECTOR3 InverseRotateBoxAroundY(const float& fDirection, const D3DXVECTOR3& VtxPos);

	// 角度の差の補正(ラジアン)
	void AdjustAngle(float& fAngle, const float& fDest);

	// 角度の差の補正(ラジアン)
	void AdjustAngle(float& fAngle, const float& fDest, const float& fRadius);

	// 向きの範囲の補正(ラジアン)
	void AdjustDirection(float& fAngle);

	// 向きの範囲の補正(ラジアン)
	void AdjustDirection(float& fAngle1, float& fAngle2);

	// Jsonファイルの展開
	nlohmann::json OpenJsonFile(std::string FilePath);

	// JsonデータをD3DXVECTOR2に変換
	D3DXVECTOR2 JsonConvertToVec2(nlohmann::json Json);

	// JsonデータをD3DXVECTOR3に変換
	D3DXVECTOR3 JsonConvertToVec3(nlohmann::json Json);

	// JsonデータをD3DXCOLRに変換
	D3DXCOLOR JsonConvertToXCol(nlohmann::json Json);

	// UTF8のJsonデータをSJISに変換
	std::string JsonConvertToSJIS(nlohmann::json Json);

	// 乱数生成
	template <typename T> static T GetRandomValue();

	// ダウンキャスト
	template <typename T1, typename T2> static T1* DownCast(T2* pBase);
	template <typename T1, typename T2> static T1* DownCast(T1* pDest, T2* pBase);
};

//============================================================================
// 乱数生成
//============================================================================
template <typename T> T utility::GetRandomValue()
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
template <typename T1, typename T2> T1* utility::DownCast(T2* pBase)
{
	// 引数のオブジェクトを指定されたクラスにダウンキャスト
	T1* pObj = dynamic_cast<T1*>(pBase);

	// ダウンキャスト失敗
	if (!pObj) throw std::bad_cast();

	return pObj;
}

//============================================================================
// ダウンキャスト
//============================================================================
template <typename T1, typename T2> T1* utility::DownCast(T1* pDest, T2* pBase)
{
	/* 警告回避 */
	pDest;

	// 引数のオブジェクトを指定されたクラスにダウンキャスト
	T1* pObj = dynamic_cast<T1*>(pBase);

	// ダウンキャスト失敗
	if (!pObj) throw std::bad_cast();

	return pObj;
}

#endif // _UTILITY_H_