//============================================================================
// 
// モーションセット、ヘッダファイル [motion_set.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _MOTION_SET_H_
#define _MOTION_SET_H_	// 二重インクルード防止

//****************************************************
// 前方宣言
//****************************************************
class CObject_Parts;

//****************************************************
// モーションセットクラス
//****************************************************
class CMotion_Set
{
	friend class CMotion_Manager;

public:

	//****************************************************
	// 動作目標値の構造体を定義
	//****************************************************
	struct KeyDest
	{
		D3DXVECTOR3 ScaleTarget;	// 目標縮尺
		D3DXVECTOR3 RotTarget;		// 目標向き
		D3DXVECTOR3 PosTarget;		// 目標座標
	};

	//****************************************************
	// キー情報の構造体を定義
	//****************************************************
	struct Key
	{
		int			nMaxFrame;	// キーの総フレーム数
		KeyDest*	apDest;		// パーツごとの動作目標値
	};

	//****************************************************
	// モーション情報の構造体を定義
	//****************************************************
	struct Motion
	{
		bool bLoop;				// ループフラグ
		WORD wMaxKey;			// モーションの総キー数
		std::vector<Key> vpKey;	// キー情報
	};

	// <special function>
	CMotion_Set();	// コンストラクタ
	~CMotion_Set();	// デストラクタ

	// <function>
	void Release();			// 破棄
	void Update();			// 更新処理
	void Animation();		// 動作
	void CountFrame();		// フレームをカウント
	void CorrectTarget();	// 目標値への補正

	// <static function>
	static CMotion_Set* Create(JSON Json);

private:

	// <getter>
	CMotion_Set::Motion* const GetNowMotion();	// 現在のモーションのポインタを取得
	CMotion_Set::Key* const GetNowKey();		// 現在のキーのポインタを取得

	// <data>
	WORD						m_wNowMotion;	// 再生中のモーション
	WORD						m_wNowKey;		// 再生中のキー
	WORD						m_wNowFrame;	// 再生中のフレーム
	std::vector<CObject_Parts*>	m_vpModelParts;	// モデルパーツ
	WORD						m_wMaxParts;	// モデルの総パーツ数
	WORD						m_wMaxMotion;	// 総モーション数
	std::vector<Motion>			m_vpMotion;		// モーション情報
};

#endif	// _MOTION_SET_