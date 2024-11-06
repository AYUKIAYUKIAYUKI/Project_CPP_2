//============================================================================
// 
// マネージャー、ヘッダファイル [managaer.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _MANAGER_H_
#define _MANAGER_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "camera.h"
#include "light.h"
#include "input_keyboard.h"
#include "input_pad.h"
#include "scene.h"

//****************************************************
// 前方宣言
//****************************************************
class CMask_Rectangle;

//****************************************************
// マネージャークラス
//****************************************************
class CManager final
{
public:

	// <function>
	HRESULT Create(HINSTANCE hInstance, HWND hWnd);	// 生成
	void	Release();								// 解放
	void	Update();								// 更新処理
	void	Draw();									// 描画処理

	// <getter>
	CMask_Rectangle*	GetMask_Rectangle() const;	// 四角形マスクを取得
	CCamera*			GetCamera() const;			// カメラを取得
	CLight*				GetLight() const;			// ライトを取得
	CScene*				GetScene() const;			// シーンを取得
	
	// <setter>
	void SetScene(CScene::MODE Mode);	// シーンの設定

	// <static getter>
	static CManager*		GetManager();	// マネージャーを取得
	static CInputKeyboard*	GetKeyboard();	// キーボードを取得
	static CInputPad*		GetPad();		// パッドを取得

private:

	// <special function>
	CManager();		// デフォルトコンストラクタ
	~CManager();	// デストラクタ

	// <function>
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期設定
	void	Uninit();								// 終了処理

	// <datas>
	CInputKeyboard*		m_pKeyboard;		// キーボード
	CInputPad*			m_pPad;				// パッド
	CMask_Rectangle*	m_pMask_Rectangle;	// 四角形マスクのポインタ
	CCamera*			m_pCamera;			// カメラ
	CLight*				m_pLight;			// ライト
	CScene*				m_pScene;			// シーン

	// <static data>
	static CManager* m_pManager;	// マネージャーの本体
};

#endif // _MANAGER_H_
