//============================================================================
// 
// フィールドマネージャー、ヘッダファイル [field_manager.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _FIELD_MANAGER_H_
#define _FIELD_MANAGER_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_X.h"

//****************************************************
// フィールドマネージャークラス
//****************************************************
class CField_Manager final
{
public:

	/// <summary> フィールド半径 </summary>
	static constexpr float FIELD_RADIUS = 300.0f;

	HRESULT Init();	// 初期設定
	void Release();	// 解放
	void Update();	// 更新処理

	static CField_Manager* GetInstance();	// 自インスタンスを取得

private:

	CField_Manager();	// デフォルトコンストラクタ
	~CField_Manager();	// デストラクタ

	void Create();			// 生成
	void Uninit();			// 終了処理
	void TestMethod();		// 仮の生成メソッド
	void TestDelete();		// 仮の破棄メソッド
	void TestDeleteAll();	// 仮の全破棄メソッド

	CObject_X* m_pCylinderCollider;	// cylinder collider

	static CField_Manager* m_pInstance;	// 自インスタンス
};

#endif // _FIELD_MANAGER_H_