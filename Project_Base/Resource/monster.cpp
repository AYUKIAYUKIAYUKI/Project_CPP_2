//============================================================================
// 
// モンスター [monster.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "monster.h"

#include "sound.h"
#include "field_manager.h"
#include "player.h"
#include "block.h"
#include "sparks.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//****************************************************
// 静的メンバ変数の初期化
//****************************************************

// 基礎パラメータの展開
const JSON CMonster::m_InitParam = utility::OpenJsonFile("Data\\JSON\\CHARACTER\\ENEMY\\monster_param.json");

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CMonster::CMonster() :
	CEnemy{ LAYER::DEFAULT },
	m_ActionType{ ACTION::HOLD },
	m_nCntActionCast{ 0 },
	m_fFakeVelY{ 0.0f }
{

}

//============================================================================
// デストラクタ
//============================================================================
CMonster::~CMonster()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CMonster::Init()
{
	// エネミークラスの初期設定
	if (FAILED(CEnemy::Init()))
	{
		assert(false && "モンスタークラスの初期設定に失敗");
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CMonster::Uninit()
{
	// エネミークラスの終了処理
	CEnemy::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CMonster::Update()
{
	// 行動分岐
	BranchAction();

	// 疑似重力
	FakeGravity();

	// 方角に座標を合わせる
	AutoSetPosTarget();

	// エネミークラスの更新処理
	CEnemy::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CMonster::Draw()
{

}

//============================================================================
// ダメージを受ける
//============================================================================
void CMonster::SetDamage(int nDamage)
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
CMonster* CMonster::Create()
{
	// インスタンスを生成
	CMonster* pNewInstance = DBG_NEW CMonster();

	// タイプを設定
	pNewInstance->CObject::SetType(TYPE::ENEMY);

	// 初期設定
	pNewInstance->Init();

	// モーションをセット
	pNewInstance->CCharacter::SetMotion(LAYER::DEFAULT, utility::OpenJsonFile("Data\\JSON\\CHARACTER\\ENEMY\\monster_motion.json"));

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
// 疑似重力
//============================================================================
void CMonster::FakeGravity()
{
	// 目標座標をコピー
	Vec3 Postarget = GetPosTarget();

	if (Postarget.y + GetHeight() > 0.0f)
	{ // 空中にいるとき

		m_fFakeVelY += CField_Manager::FIELD_GRAVITY;	// 重力加速
		Postarget.y += m_fFakeVelY;						// 高さが下がっていく
		SetPosTarget(Postarget);						// 目標座標をセット
	}
	else if(Postarget.y + GetHeight() < 0.0f)
	{ // 地面にめり込んでいるとき

		m_fFakeVelY = 0.0f;			// 加速度リセット
		Postarget.y = GetHeight();	// 高さを固定
		SetPosTarget(Postarget);	// 目標座標をセット
	}
}

//============================================================================
// 行動分岐
//============================================================================
void CMonster::BranchAction()
{
	// プレイヤーが存在しなければアクションをしない
	if (!FindPlayer())
		return;

	// タイプに応じて処理を変更
	switch (m_ActionType)
	{
		// 立ち止まる
	case ACTION::HOLD:
		Hold();
		break;

		// 歩いてくる
	case ACTION::COMING:
		Coming();
		break;

		// 引き下がる
	case ACTION::GOBACK:
		GoBack();
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
// 立ち止まる
//============================================================================
void CMonster::Hold()
{
	// 待機モーションに変更
	if (GetNowMotion() != 0)
	{
		SetNowMotion(0);
	}

	// プレイヤーを取得
	CPlayer* const pPlayer = FindPlayer();

	// プレイヤーへの距離を出す
	Vec3 Norm = pPlayer->GetPos() - GetPos();

	// プレイヤーに近ければ歩いてくる
	if (Norm.y * Norm.y <  GetHeight() * GetHeight() &&
		Norm.x * Norm.x + Norm.z * Norm.z < 2000.0f)
	{
		m_ActionType = ACTION::COMING;
	}

	// 衝突検出
	HitCheck();
}

//============================================================================
// 歩いてくる
//============================================================================
void CMonster::Coming()
{
	// 歩いてくるモーションに変更
	if (GetNowMotion() != 1)
	{
		SetNowMotion(1);
	}

	// 衝突検出
	HitCheck();

	// ブロックに接近していたら終了
	if (StopBlockSide())
		return;

	// プレイヤーを取得
	CPlayer* const pPlayer = FindPlayer();

	// プレイヤーとの距離の差を出す
	Vec3 Norm = pPlayer->GetPos() - GetPos();

	// プレイヤーから遠ければ
	if (Norm.x * Norm.x + Norm.z * Norm.z > 4500.0f)
	{
		// アクションタイプを立ち止まるに変更
		m_ActionType = ACTION::HOLD;

		// 以降の処理は行わない
		return;
	}

	// 自身の目標方角をコピー
	float fDirectionTarget = GetDirectionTarget();

	// 方角の差を出す
	float fDifference = pPlayer->GetDirection() - fDirectionTarget;

	// 差を埋めるように目標方角を変動
	if (fDifference > 0.0f)
	{
		SetDirectionTarget(fDirectionTarget + static_cast<float>(m_InitParam["Speed"]));
	}
	else
	{
		SetDirectionTarget(fDirectionTarget + -static_cast<float>(m_InitParam["Speed"]));
	}

	// 移動方向に向きを合わせる
	AutoSetRotTarget();
}

//============================================================================
// ブロックの近くで止まる
//============================================================================
bool CMonster::StopBlockSide()
{
	// 通常優先度のオブジェクトを取得
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	while (pObj != nullptr)
	{
		// ブロックタイプのオブジェクトを取得
		if (pObj->GetType() == CObject::TYPE::BLOCK)
		{
			// オブジェクトをブロックタグにダウンキャスト
			CBlock* pBlock = utility::DownCast<CBlock, CObject>(pObj);

			// ブロックと座標の距離を出す
			Vec3 Norm = pBlock->GetPos() - GetPos();

			// ブロックの半幅と自分の半径・高さで引き下がる長さを出す
			float
				fWidth = pBlock->GetModel()->Size.x + GetRadius(),
				fHeight = pBlock->GetModel()->Size.y + GetHeight();

			// 高さが同じくらい、かつある程度隣接しているブロックがあれば引き下がる
			if (Norm.y * Norm.y < fHeight * fHeight &&
				Norm.x * Norm.x + Norm.z * Norm.z < fWidth * fWidth)
			{
				// アクションタイプを変更
				m_ActionType = ACTION::GOBACK;

				return true;
			}
		}

		pObj = pObj->GetNext();
	}

	return false;
}

//============================================================================
// 引き下がる
//============================================================================
void CMonster::GoBack()
{
	// 自身の目標方角をコピー
	float fDirectionTarget = GetDirectionTarget();

	// 向きに応じて引き下がる方向を設定
	if (!CheckFacingSide())
	{
		fDirectionTarget += static_cast<float>(m_InitParam["Speed"]) * 0.5f;
	}
	else
	{
		fDirectionTarget += -static_cast<float>(m_InitParam["Speed"]) * 0.5f;
	}

	// 目標方角を反映
	SetDirectionTarget(fDirectionTarget);

	// 行動キャストカウントをインクリメント
	++m_nCntActionCast;

	// 0.5秒経過したら
	if (m_nCntActionCast > 30)
	{
		// カウントリセット
		m_nCntActionCast = 0;

		// アクションタイプを立ち止まるに変更
		m_ActionType = ACTION::HOLD;
	}
}

//============================================================================
// 死亡
//============================================================================
void CMonster::DeadEnd()
{
	// 死亡モーションに変更
	if (GetNowMotion() != 2)
	{
		SetNowMotion(2);

		// 消滅SEを鳴らす
		CSound::GetInstance()->Play(CSound::LABEL::DISAPPEAR);
	}

	// 火の粉を集中発生
	CSparks::PointGenerate(GetPos());

	// 死亡モーションの再生が終了したら
	if (GetStopState())
	{
		// 破棄
		SetRelease();
	}
}