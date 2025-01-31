//============================================================================
// 
// Xオブジェクト、ヘッダファイル [object_X.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _OBJECT_X_H_
#define _OBJECT_X_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object.h"

// Xモデル情報用
#include "X_manager.h"

//****************************************************
// 前方宣言
//****************************************************
class CRender_Collision;

//****************************************************
// Xオブジェクトクラス
//****************************************************
class CObject_X : public CObject
{
public:

	// <special function>
	CObject_X(LAYER Priority = LAYER::BG);	// 描画優先度指定コンストラクタ
	~CObject_X() override;					// デストラクタ

	// <function>
	HRESULT	Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <getter/setter>
	CX_Manager::MODEL* GetModel();				// モデル取得
	CX_Manager::TYPE GetModelType();			// モデルタイプ取得
	void BindModel(CX_Manager::MODEL* pModel);	// モデル割当
	void BindModel(CX_Manager::TYPE Type);		// もっとモデル割当

	// <getter/setter>
	const D3DXVECTOR3& GetRot() const;	// 向き取得
	void SetRot(D3DXVECTOR3 Rot);		// 向き設定

	// <getter/setter>
	const D3DXVECTOR3& GetPos() const;	// 座標取得
	void SetPos(D3DXVECTOR3 Pos);		// 座標設定

	// <getter/setter>
	const D3DXVECTOR3& GetScale() const;	// 縮尺取得
	void SetScale(D3DXVECTOR3 Scale);		// 縮尺設定

	// <getter/setter>
	const D3DXCOLOR& GetCol() const;	// 色を取得
	void SetCol(D3DXCOLOR Col);			// 色を設定

	// <getter/setter>
	const bool& GetUseCol() const;	// 色反映を取得
	void SetUseCol(bool Col);		// 色反映を設定

	// <virtual getter>
	virtual D3DXVECTOR3 GetSize() const;	// サイズを取得
	virtual float GetRadius() const;		// 半径を取得
	virtual float GetHeight() const;		// 高さを取得

	// <static function>
	static CObject_X* Create(CX_Manager::TYPE Type);					// 生成
	static CObject_X* Create(LAYER Priority, CX_Manager::TYPE Type);	// 生成
	static CObject_X* Create(JSON Json);								// 生成

private:

	/// <summary> アルファ値初期化用 </summary>
	static constexpr float ALPHA_INIT = 1.0f;

	// <function>
	void AdjustRotAngle();	// 向きを調整する
	void SetMtxWorld();		// ワールド行列設定

	// <data>
	CX_Manager::MODEL*	m_pModel;	// モデル情報
	D3DXVECTOR3			m_Rot;		// 向き
	D3DXVECTOR3			m_Pos;		// 座標
	D3DXVECTOR3			m_Scale;	// 縮尺
	D3DXCOLOR			m_Col;		// 色
	bool				m_bUseCol;	// 色反映
	D3DXMATRIX			m_MtxWorld;	// ワールド行列
};

#endif // _OBJECT_X_H_