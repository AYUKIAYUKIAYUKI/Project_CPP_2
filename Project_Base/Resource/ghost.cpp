//============================================================================
// 
// ゴースト [ghost.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "ghost.h"

#include "player.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//****************************************************
// 静的メンバ変数の初期化
//****************************************************

// 基礎パラメータの展開
const JSON CGhost::m_InitParam = utility::OpenJsonFile("Data\\JSON\\CHARACTER\\ENEMY\\ghost_param.json");

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CGhost::CGhost() :
	CEnemy{ LAYER::DEFAULT },
	m_ActionType{ ACTION::HOLD }
{

}

//============================================================================
// デストラクタ
//============================================================================
CGhost::~CGhost()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CGhost::Init()
{
	// エネミークラスの初期設定
	if (FAILED(CEnemy::Init()))
	{
		assert(false && "ゴーストクラスの初期設定に失敗");
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CGhost::Uninit()
{
	// エネミークラスの終了処理
	CEnemy::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CGhost::Update()
{
	// 行動分岐
	BranchAction();

	// エネミークラスの更新処理
	CEnemy::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CGhost::Draw()
{

}

//============================================================================
// ダメージを受ける
//============================================================================
void CGhost::SetDamage(int nDamage)
{
	// ダメージ量分、体力を変動
	int nNewLife = GetLife();
	nNewLife += nDamage;
	SetLife(nNewLife);

	// ライフが無くなったら
	if (nNewLife < 0)
	{
		// アクションタイプを死亡に変更
		m_ActionType = ACTION::DEADEND;
	}
}

//============================================================================
// 生成
//============================================================================
CGhost* CGhost::Create()
{
	// インスタンスを生成
	CGhost* pNewInstance = DBG_NEW CGhost();

	// タイプを設定
	pNewInstance->CObject::SetType(TYPE::ENEMY);

	// 初期設定
	pNewInstance->Init();

	// モーションをセット
	pNewInstance->CCharacter::SetMotion(LAYER::STENCIL, utility::OpenJsonFile("Data\\JSON\\CHARACTER\\ENEMY\\ghost_motion.json"));

	{ // パラメータ設定

		// データをキャスト
		int
			nLife = static_cast<int>(m_InitParam["Life"]);
		float
			fCoef = static_cast<float>(m_InitParam["Coef"]),
			fSpeed = static_cast<float>(m_InitParam["Speed"]),
			fRadius = static_cast<float>(m_InitParam["Radius"]),
			fHeight = static_cast<float>(m_InitParam["Height"]);

		// データをセット
		pNewInstance->CCharacter::SetCorrectCoef(fCoef);	// 補間強度
		pNewInstance->CCharacter::SetMoveSpeed(fSpeed);		// 移動速度
		pNewInstance->CCharacter::SetLife(nLife);			// 体力
		pNewInstance->CEnemy::SetBndSize(fRadius, fHeight);	// バウンディングサイズ
	}

	return pNewInstance;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 行動分岐
//============================================================================
void CGhost::BranchAction()
{
	// プレイヤーが存在しなければアクションをしない
	if (!FindPlayer())
		return;

	// タイプに応じて処理を変更
	switch (m_ActionType)
	{
		// 漂う
	case ACTION::HOLD:
		Hold();
		break;

		// 飛んでくる
	case ACTION::FLY:
		Fly();
		break;

		// 死亡
	case ACTION::DEADEND:
		DeadEnd();
		break;

		// 例外
	default:
#ifdef _DEBUG
		assert(false && "エネミーの行動に例外発生");
#else
		m_ActionType = ACTION::HOLD;
#endif // _DEBUG
		break;
	}
}

//============================================================================
// 漂う
//============================================================================
void CGhost::Hold()
{
	// 待機モーションに変更
	if (GetNowMotion() != 0)
	{
		SetNowMotion(0);
	}

	// プレイヤーを取得
	CPlayer* const pPlayer = FindPlayer();

	// プレイヤーとの距離の差を出す
	Vec3 Norm = pPlayer->GetPos() - GetPos();

	// プレイヤーに近ければ飛んでくる
	if (Norm.x * Norm.x + Norm.y * Norm.y + Norm.z * Norm.z < 5000.0f)
	{
		m_ActionType = ACTION::FLY;
	}

	// 衝突検出
	HitCheck();
}

//============================================================================
// 飛んでくる
//============================================================================
void CGhost::Fly()
{
	// 飛んでくるモーションに変更
	if (GetNowMotion() != 1)
	{
		SetNowMotion(1);
	}

	// 衝突検出
	HitCheck();

	// プレイヤーを取得
	CPlayer* const pPlayer = FindPlayer();

	// プレイヤーとの距離の差を出す
	Vec3 Norm = pPlayer->GetPos() - GetPos();

	// プレイヤーから遠ければ
	if (Norm.x * Norm.x + Norm.y * Norm.y + Norm.z * Norm.z > 8000.0f)
	{
		// アクションタイプを漂うに変更
		m_ActionType = ACTION::HOLD;

		// 以降の処理は行わない
		return;
	}

	// 自身の座標をコピー
	Vec3 Pos = GetPos();

	// プレイヤーとの座標の差を出す
	Vec3 fDifference = pPlayer->GetPos() - Pos;

	// 差を埋めていく
	Pos += fDifference * 0.5f;

	// 目標座標を設定
	SetPosTarget(Pos);

	// 移動方向に向きを合わせる
	AutoSetRotTarget();
}

//============================================================================
// 死亡
//============================================================================
void CGhost::DeadEnd()
{
	// 死亡モーションに変更
	if (GetNowMotion() != 2)
	{
		SetNowMotion(2);
	}

	// 死亡モーションの再生が終了したら
	if (GetStopState())
	{
		// 破棄
		SetRelease();
	}
}