////============================================================================
//// 
//// 旧フェード遷移処理、ヘッダファイル [fade.h]
//// Author : 福田歩希
//// 
////============================================================================
//
//#ifndef _FADE_H_
//#define _FADE_H_	// 二重インクルード防止
//
////****************************************************
//// インクルードファイル
////****************************************************
//#include "scene.h"
//
////****************************************************
//// フェードクラス
////****************************************************
//class CFade
//{
//public:
//
//	HRESULT Init();	// 初期設定
//	void Release();	// 解放
//	void Update();	// 更新処理
//	void Draw();	// 描画処理
//
//	static void SetFade(CScene::MODE Mode);	// フェード設定
//	static CFade* GetInstance();			// フェードを取得
//
//private:
//
//	CFade();	// コンストラクタ
//	~CFade();	// デストラクタ
//
//	void Create();	// 生成
//	void Uninit();	// 終了処理
//	void FadeIn();	// フェードイン
//	void FadeOut();	// フェードアウト
//	void SetVtx();	// 頂点情報の設定
//
//	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファのポインタ
//	CScene::MODE m_NextMode;			// 次のモード
//	bool m_bFadeIn;						// フェードイン判定
//	bool m_bFadeOut;					// フェードアウト判定
//	float m_fBrightness;				// 明度
//
//	static CFade* m_pInstnce;	// 自インスタンス
//};
//
//#endif // _FADE_H_