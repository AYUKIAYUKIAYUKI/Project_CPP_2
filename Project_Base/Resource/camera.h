//============================================================================
//
// カメラ、ヘッダファイル [camera.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _CAMERA_H_
#define _CAMERA_H_	// 二重インクルード防止

//****************************************************
// 名前空間を定義
//****************************************************
namespace camera
{
	//****************************************************
	// usingディレクティブ
	//****************************************************
	using namespace abbr;

	//****************************************************
	// using宣言
	//****************************************************
	using std::to_string;

	//****************************************************
	// カメラクラス
	//****************************************************
	class CCamera
	{
	public:

		CCamera();	// デフォルトコンストラクタ
		~CCamera();	// デストラクタ

		HRESULT	Init();		// 初期設定
		void Update();		// 更新処理
		void SetCamera();	// カメラをセット

		const Vec3& GetPos() const;	// 座標を取得
		void SetPos(Vec3 Pos);		// 座標を設定

		const Vec3& GetPosTarget() const;	// 目標座標を取得
		void SetPosTarget(Vec3 PosTarget);	// 目標座標を設定

		const Vec3& GetRot() const;	// 向きを取得
		void SetRot(Vec3 Rot);		// 向きを設定

		const Vec3& GetRotTarget() const;	// 目標向きを取得
		void SetRotTarget(Vec3 RotTarget);	// 目標向きを設定

		const float& GetDistance() const;	// 間距離を取得
		void SetDistance(float fDistance);	// 間距離を設定

	private:

		/// <summary> 目標値への補間強度 </summary>
		static constexpr float COEF_ADJUST = 0.1f;

		void BranchMode();			// ビューモード分岐
		void Control();				// 操作
		void Rotation();			// 回転
		void RestrictPitch();		// ピッチ角の範囲制限
		void Translation();			// 移動
		void CalcPosV();			// 視点座標を計算
		void CalcPosR();			// 注視点座標を計算
		void CalcMtxProjection();	// プロジェクション行列計算
		void CalcMtxView();			// ビュー行列計算

		Vec3	m_Pos;				// 座標
		Vec3	m_PosTarget;		// 目標座標
		Vec3	m_PosV;				// 視点座標
		Vec3	m_PosTargetV;		// 目標視点座標
		Vec3	m_PosR;				// 注視点座標
		Vec3	m_PosTargetR;		// 目標注視点座標
		Vec3	m_Rot;				// 向き
		Vec3	m_RotTarget;		// 目標向き
		float	m_fDistance;		// 視点 -> 注視点間の距離
		Vec3	m_VecU;				// 上方向ベクトル
		Mtx		m_MtxProjection;	// プロジェクション行列
		Mtx		m_MtxView;			// ビュー行列
		float	m_fAdjust;			// 俯瞰度合い
		bool	m_bTrack;			// カメラの追従切り替え
	};
}

#endif // _CAMERA_H_