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

#include "player.h"
#include "block.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CMonster::CMonster() :
	CEnemy{},
	m_ActionType{ ACTION::HOLD },
	m_nCntActionCast{ 0 }
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
	// 補正強度を設定
	SetCorrectCoef(CORRECT_COEF);

	// 初期移動速度を設定
	SetMoveSpeed(DEFAULT_MOVE_SPEED);

	// 初期体力を設定
	SetLife(MAX_LIFE);

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

	// エネミークラスの更新処理
	CEnemy::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CMonster::Draw()
{
	// エネミークラスの描画処理
	CEnemy::Draw();
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
	pNewInstance->SetType(TYPE::ENEMY);

	// 初期設定
	pNewInstance->Init();

	// モーションをセット
	pNewInstance->SetMotion(utility::OpenJsonFile("Data\\JSON\\CHARACTER\\enemy_motion.json"));

	// バウンディングサイズを設定
	pNewInstance->SetBndSize(3.0f, 3.0f);

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
void CMonster::BranchAction()
{
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
	// プレイヤータイプのオブジェクトを検索
	CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::PLAYER);

	// オブジェクトが見つからければ終了
	if (pObj == nullptr)
		return;

	// プレイヤークラスにダウンキャスト
	CPlayer* pPlayer = utility::DownCast<CPlayer, CObject>(pObj);

	// プレイヤーへの距離を出す
	Vec3 Norm = pPlayer->GetPos() - GetPos();

	// プレイヤーに近ければ歩いてくる
	if (Norm.x * Norm.x + Norm.y * Norm.y + Norm.z * Norm.z < 2000.0f)
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
	// 衝突検出
	HitCheck();

	// ブロックに接近していたら終了
	if (StopBlockSide())
		return;

	// プレイヤータイプのオブジェクトを検索
	CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::PLAYER);

	// オブジェクトが見つからければ終了
	if (pObj == nullptr)
		return;

	// プレイヤークラスにダウンキャスト
	CPlayer* pPlayer = utility::DownCast<CPlayer, CObject>(pObj);

	// 自身の目標方角をコピー
	float fDirectionTarget = GetDirectionTarget();

	// 方角の差を出す
	float fDifference = pPlayer->GetDirection() - fDirectionTarget;

	// 差を埋めるように目標方角を変動
	if (fDifference > 0.0f)
	{
		SetDirectionTarget(fDirectionTarget + DEFAULT_MOVE_SPEED);
	}
	else
	{
		SetDirectionTarget(fDirectionTarget + -DEFAULT_MOVE_SPEED);
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

#ifdef _DEBUG
			ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
			if (ImGui::Begin("Test")) {
				ImGui::Text("Y:%f", Norm.y * Norm.y);
				ImGui::End();
			}
#endif // _DEBUG

			// 高さが同じくらい、かつある程度隣接しているブロックがあれば引き下がる
			if (Norm.y * Norm.y < 200.0f &&
				Norm.x * Norm.x + Norm.z * Norm.z < 1000.0f)
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
		fDirectionTarget += DEFAULT_MOVE_SPEED * 0.5f;
	}
	else
	{
		fDirectionTarget += -DEFAULT_MOVE_SPEED * 0.5f;
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

}