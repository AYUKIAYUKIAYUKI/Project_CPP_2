//============================================================================
// 
// モーションマネージャー、ヘッダファイル [motion_manager.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _MOTION_MANAGER_H_
#define _MOTION_MANAGER_H_	// 二重インクルード防止

//****************************************************
// 前方宣言
//****************************************************
class CObject_X;

//****************************************************
// モーションマネージャークラス
//****************************************************
class CMotion_Manager final
{
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
	struct MotionKey
	{
		WORD		wMaxFrame;	// キーの総フレーム数
		KeyDest*	apDest;		// パーツごとの動作目標値
	};

	//****************************************************
	// モーション情報の構造体を定義
	//****************************************************
	struct ActorMotion
	{
		bool		bLoop;		// ループフラグ
		WORD		wMaxKey;	// モーションの総キー数
		MotionKey*	apKey;		// キー情報
	};

	//****************************************************
	// アクターの構造体を定義
	//****************************************************
	struct Actor
	{
		WORD					wNowMotion;		// 再生中のモーション
		WORD					wNowKey;		// 再生中のキー
		WORD					wNowFrame;		// 再生中のフレーム
		std::vector<CObject_X*>	vpModelParts;	// モデルパーツ
		WORD					wMaxMotion;		// 総モーション数
		ActorMotion*			apMotion;		// モーション情報
	};

	// <special function>
	CMotion_Manager(const CMotion_Manager&) = delete;				// コピーコンストラクタ
	CMotion_Manager& operator=(const CMotion_Manager&) = delete;	// コピー代入演算子
	CMotion_Manager(CMotion_Manager&&) = delete;					// ムーブコンストラクタ
	CMotion_Manager& operator=(CMotion_Manager&&) = delete;			// ムーブ代入演算子

	// <function>
	void Update();	// 更新処理

	// <static function>
	static HRESULT	Create();	// 生成
	static void		Release();	// 解放

	// <static getter>
	static CMotion_Manager* GetInstance();	// Xモデルマネージャーを取得

private:

	// <special function>
	CMotion_Manager();	// コンストラクタ
	~CMotion_Manager();	// デストラクタ

	// <function>
	HRESULT	Init();				// 初期設定
	void	Uninit();			// 終了処理
	void	Reset();			// リセット
	void	Animation();		// 動作
	void	CountFrame();		// フレームをカウント
	void	CorrectTarget();	// 目標値への補正

	// <data>
	Actor m_Actor;	// アクター

	// <static data>
	static CMotion_Manager* m_pMotionManager;	// Xモデルマネージャーの本体
};

#endif // _X_MANAGER_H_