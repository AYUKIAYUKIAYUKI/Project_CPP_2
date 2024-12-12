//============================================================================
// 
// フライヤー [flyer.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "flyer.h"

#include "player.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//****************************************************
// 静的メンバ変数の初期化
//****************************************************

// 基礎パラメータの展開
JSON CFlyer::m_InitParam = utility::OpenJsonFile("Data\\JSON\\CHARACTER\\ENEMY\\flyer_param.json");

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CFlyer::CFlyer() :
	CEnemy{ LAYER::DEFAULT },
	m_ActionType{ ACTION::UPDOWN },
	m_bAdder{ false },
	m_fDiffFromInitY{ 0.0f },
	m_fMinRangeDiff{ 0.0f },
	m_fMaxRangeDiff{ 0.0f }
{

}

//============================================================================
// デストラクタ
//============================================================================
CFlyer::~CFlyer()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CFlyer::Init()
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
void CFlyer::Uninit()
{
	// エネミークラスの終了処理
	CEnemy::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CFlyer::Update()
{
	// 行動分岐
	BranchAction();

	// 方角に座標を合わせる
	AutoSetPosTarget();

	// エネミークラスの更新処理
	CEnemy::Update();

#if 0
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("PosY")) {
		ImGui::Text("%f", GetPos().y);
		ImGui::Text("%f", GetPosTarget().y);
		ImGui::Text("%f", m_fDiffFromInitY);
		ImGui::End();
	}

	if (CManager::GetKeyboard()->GetRelease(DIK_RETURN))
		m_InitParam = utility::OpenJsonFile("Data\\JSON\\CHARACTER\\ENEMY\\flyer_param.json");
#endif
}

//============================================================================
// 描画処理
//============================================================================
void CFlyer::Draw()
{

}

//============================================================================
// ダメージを受ける
//============================================================================
void CFlyer::SetDamage(int nDamage)
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
CFlyer* CFlyer::Create()
{
	// インスタンスを生成
	CFlyer* pNewInstance = DBG_NEW CFlyer();

	// タイプを設定
	pNewInstance->CObject::SetType(TYPE::ENEMY);

	// 初期設定
	pNewInstance->Init();

	// モーションをセット
	pNewInstance->CCharacter::SetMotion(LAYER::DEFAULT, utility::OpenJsonFile("Data\\JSON\\CHARACTER\\ENEMY\\flyer_motion.json"));

	{ // パラメータ設定

		// データをキャスト
		int
			nLife = static_cast<int>(m_InitParam["Life"]);
		float
			fCoef = static_cast<float>(m_InitParam["Coef"]),
			fSpeed = static_cast<float>(m_InitParam["Speed"]),
			fRadius = static_cast<float>(m_InitParam["Radius"]),
			fHeight = static_cast<float>(m_InitParam["Height"]),
			fMin = static_cast<float>(m_InitParam["MinRangeDiff"]),
			fMax = static_cast<float>(m_InitParam["MaxRangeDiff"]);

		// データをセット
		pNewInstance->CCharacter::SetCorrectCoef(fCoef);	// 補間強度
		pNewInstance->CCharacter::SetMoveSpeed(fSpeed);		// 移動速度
		pNewInstance->CCharacter::SetLife(nLife);			// 体力
		pNewInstance->CEnemy::SetBndSize(fRadius, fHeight);	// バウンディングサイズ
		pNewInstance->m_fMinRangeDiff = fMin;				// 変化量の最低値
		pNewInstance->m_fMaxRangeDiff = fMax;				// 変化量の最大値
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
void CFlyer::BranchAction()
{
	// プレイヤーが存在しなければアクションをしない
	if (!FindPlayer())
		return;

	// タイプに応じて処理を変更
	switch (m_ActionType)
	{
		// 立ち止まる
	case ACTION::UPDOWN:
		UpDown();
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
		m_ActionType = ACTION::UPDOWN;
#endif // _DEBUG
		break;
	}
}

//============================================================================
// 昇降
//============================================================================
void CFlyer::UpDown()
{
	// 回転モーション中は昇降しない
	if (GetNowMotion() == 1 && !GetStopState())
	{
		return;
	}

	// 昇降モーションに変更
	if (GetNowMotion() != 0)
	{
		SetNowMotion(0);
	}

	// 移動速度をコピー
	float fSpeed = GetMoveSpeed();

	// 増加と減少を切り替えて上下に高さの補間を繰り返す
	if (m_bAdder)
	{
		m_fDiffFromInitY += (m_fMaxRangeDiff - m_fDiffFromInitY) * fSpeed;
	}
	else
	{
		m_fDiffFromInitY += (m_fMinRangeDiff - m_fDiffFromInitY) * fSpeed;
	}

	// 最大値・最低値にある程度近似で増減をスイッチ
	if (m_fDiffFromInitY > m_fMaxRangeDiff - 0.1f ||
		m_fDiffFromInitY < m_fMinRangeDiff + 0.1f)
	{
		// 回転モーションをセット
		SetNowMotion(1);

		// 高さの変化量をリセット
		m_fDiffFromInitY = 0.0f;

		// 昇降をスイッチ
		m_bAdder = !m_bAdder;
	}

	// 目標座標をコピー
	Vec3 PosTarget = GetPosTarget();
	PosTarget.y += m_fDiffFromInitY;
	SetPosTarget(PosTarget);

	// 衝突検出
	HitCheck();
}

//============================================================================
// 死亡
//============================================================================
void CFlyer::DeadEnd()
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