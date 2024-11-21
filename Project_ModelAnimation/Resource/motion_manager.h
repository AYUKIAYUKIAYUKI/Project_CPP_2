//============================================================================
// 
// モーションマネージャー、ヘッダファイル [motion_manager.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _MOTION_MANAGER_H_
#define _MOTION_MANAGER_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "motion_set.h"

//****************************************************
// 前方宣言
//****************************************************
class CObject_Parts;

//****************************************************
// モーションマネージャークラス
//****************************************************
class CMotion_Manager final
{
public:

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
	static CMotion_Manager* GetInstance();	// モーションマネージャーを取得

private:

	// <special function>
	CMotion_Manager();	// コンストラクタ
	~CMotion_Manager();	// デストラクタ

	// <function>
	HRESULT	Init();			// 初期設定
	void	Uninit();		// 終了処理
	void	PrintDebug();	// デバッグ表示
	void	Edit();			// 編集
	void	EditParts();	// パーツ情報の編集
	void	EditMotion();	// モーション情報の編集
	void	EditKey();		// キー情報の編集
	void	EditFrame();	// フレーム情報の編集
	void	EditDest();		// 目標値情報の編集
	void	Export();		// エクスポート
	void	Reset();		// リセット

	// <getter>
	CObject_Parts* const GetSelectParts() const;		// 選択中のパーツのポインタを取得
	CMotion_Set::Motion* const GetSelectMotion() const;	// 選択中のモーションのポインタを取得
	CMotion_Set::Key* const GetSelectKey() const;		// 選択中のキーのポインタを取得

	// <data>
	JSON			m_Json;				// ジェイソンデータ
	CMotion_Set*	m_MotionSet;		// モーションセット
	WORD			m_wSelectParts;		// 選択パーツ
	WORD			m_wSelectMotion;	// 選択モーション
	WORD			m_wSelectKey;		// 選択キー
	bool			m_bPlay;			// 再生

	// <static data>
	static CMotion_Manager* m_pMotionManager;	// モーションマネージャーの本体
};

#endif // _X_MANAGER_H_