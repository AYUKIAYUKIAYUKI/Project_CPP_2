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
// 名前空間を定義
//****************************************************
namespace field_manager
{
	//****************************************************
	// usingディレクティブ
	//****************************************************
	using namespace abbr;

	//****************************************************
	// フィールドマネージャークラス
	//****************************************************
	class CField_Manager final
	{
	public:

		/// <summary> フィールド半径 </summary>
		static constexpr float FIELD_RADIUS = 300.0f;

		/// <summary> 生成範囲の半径 </summary>
		static constexpr float GENERATE_RANGE_RADIUS = 350.0f;

		/// <summary> 最大ブロック数 </summary>
		static constexpr int MAX_BLOCK = 100;

		/// <summary> 重力 </summary>
		static constexpr float FIELD_GRAVITY = -0.025f;

		HRESULT Init();	// 初期設定
		void Release();	// 解放
		void Update();	// 更新処理

		static CField_Manager* GetInstance();	// 自インスタンスを取得

	private:

		CField_Manager();	// デフォルトコンストラクタ
		~CField_Manager();	// デストラクタ

		void Create();			// 生成
		void Uninit();			// 終了処理
		void TestCreate();		// 仮の生成メソッド
		void TestDelete();		// 仮の破棄メソッド
		void TestDeleteAll();	// 仮の全破棄メソッド

		float m_fCoeffRaondomRange;	// ランダム範囲の強度

		static CField_Manager* m_pInstance;	// 自インスタンス

		CObject_X* m_pCylinderCollider;	// cylinder collider
	};
}

#endif // _FIELD_MANAGER_H_