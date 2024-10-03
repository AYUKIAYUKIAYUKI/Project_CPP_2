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

	// 乱数生成
	template <typename T> static T GetRandomValue();

	// ダウンキャスト
	template <typename T1, typename T2> static T1* DownCast(T2* pBase);
};

//============================================================================
// 乱数生成
//============================================================================
template <typename T> T CUtility::GetRandomValue()
{
	// 数値の取る範囲を設定
	const int nRange = 180;

	// 倍の範囲からランダムな整数を生成
	int random{ rand() % (nRange * 2) };

	// もとの範囲を超えていた場合正負を反転
	if (random > nRange)
	{
		random %= nRange;
		random *= -1;
	}

	// 整数を指定された型にキャスト
	return static_cast<T>(random);
}

//============================================================================
// ダウンキャスト
//============================================================================
template <typename T1, typename T2> T1* CUtility::DownCast(T2* pBase)
{
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