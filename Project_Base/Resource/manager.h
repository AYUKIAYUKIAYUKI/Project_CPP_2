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

	// <special function>
	CManager();		// デフォルトコンストラクタ
	~CManager();	// デストラクタ

	// <function>
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);	// 初期設定
	void	Uninit();											// 終了処理
	void	Update();											// 更新処理
	void	Draw();												// 描画処理

	// 四角形マスクの取得
	static CMask_Rectangle* GetMask_Rectangle()
	{ return m_pMask_Rectangle; }
	
	// <static getter>
	static CCamera*			GetCamera();	// カメラ取得
	static CLight*			GetLight();		// ライト取得
	static CInputKeyboard*	GetKeyboard();	// キーボード取得
	static CInputPad*		GetPad();		// パッド取得
	static CScene*			GetScene();		// シーン取得
	
	// <static setter>
	static void SetScene(CScene::MODE mode);	// シーン設定

private:

	// <static datas>
	static CMask_Rectangle*	m_pMask_Rectangle;	// 四角形マスクのポインタ
	static CCamera*			m_pCamera;			// カメラ管理
	static CLight*			m_pLight;			// ライト管理
	static CInputKeyboard*	m_pKeyboard;		// キーボード管理
	static CInputPad*		m_pPad;				// パッド管理
	static CScene*			m_pScene;			// シーン管理
};

#endif // _MANAGER_H_
