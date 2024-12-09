//============================================================================
// 
// エネミー [enemy.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "enemy.h"
#include "bounding_cylinder.h"

#include "collision.h"
#include "player.h"
#include "player_state_damage.h"
#include "block.h"
#include "renderer.h"

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
CEnemy::CEnemy() :
	CCharacter{},
	m_ActionType{ ACTION::HOLD },
	m_nCntActionCast{ 0 },
	m_pBndCylinder{ DBG_NEW CBounding_Cylinder() }
{

}

//============================================================================
// デストラクタ
//============================================================================
CEnemy::~CEnemy()
{
	// バウンディングシリンダーの破棄
	if (m_pBndCylinder != nullptr)
	{
		// メモリを解放
		delete m_pBndCylinder;

		// ポインタを初期化
		m_pBndCylinder = nullptr;
	}
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CEnemy::Init()
{
	// 補正強度を設定
	SetCorrectCoef(CORRECT_COEF);

	// 初期移動速度を設定
	SetMoveSpeed(DEFAULT_MOVE_SPEED);

	// 初期体力を設定
	SetLife(MAX_LIFE);

	// キャラクタークラスの初期設定
	if (FAILED(CCharacter::Init()))
	{
		assert(false && "エネミークラスの初期設定に失敗");
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CEnemy::Uninit()
{
	// キャラクタークラスの終了処理
	CCharacter::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CEnemy::Update()
{
	// 次の行動を決定
	SetNextAction();

	// 行動分岐
	BranchAction();

	// 高さの補正
	AdjustHeight();

	// 方角に座標を合わせる
	AutoSetPosTarget();

	// 当たり判定の中心点を設定
	m_pBndCylinder->SetCenterPos(GetPos());

	// キャラクタークラスの更新処理
	CCharacter::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CEnemy::Draw()
{
	// キャラクターの描画処理
	CCharacter::Draw();
}

//============================================================================
// 半径を取得
//============================================================================
float CEnemy::GetRadius() const
{
	return m_pBndCylinder->GetRadius();
}

//============================================================================
// 高さを取得
//============================================================================
float CEnemy::GetHeight() const
{
	return m_pBndCylinder->GetHeight();
}

//============================================================================
// 円柱バウンディングを取得
//============================================================================
const CBounding_Cylinder* const CEnemy::GetBndCylinder() const
{
	return m_pBndCylinder;
}

//============================================================================
// ダメージを受ける
//============================================================================
void CEnemy::SetDamage(int nDamage)
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
CEnemy* CEnemy::Create()
{
	// インスタンスを生成
	CEnemy* pNewInstance = DBG_NEW CEnemy();

	// タイプを設定
	pNewInstance->SetType(TYPE::ENEMY);

	// 初期設定
	pNewInstance->Init();

	// モーションをセット
	pNewInstance->SetMotion(utility::OpenJsonFile("Data\\JSON\\CHARACTER\\enemy_motion.json"));

	// 半径を設定
	pNewInstance->m_pBndCylinder->SetRadius(3.0f);

	// 高さを設定
	pNewInstance->m_pBndCylinder->SetHeight(3.0f);

	return pNewInstance;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 次の行動を決定
//============================================================================
void CEnemy::SetNextAction()
{
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Enemy")) {
		ImGui::Text("%d", m_ActionType);
		ImGui::End();
	}
}

//============================================================================
// 行動分岐
//============================================================================
void CEnemy::BranchAction()
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
void CEnemy::Hold()
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
void CEnemy::Coming()
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
		SetDirectionTarget(fDirectionTarget + DEFAULT_MOVE_SPEED);
	else
		SetDirectionTarget(fDirectionTarget + -DEFAULT_MOVE_SPEED);

	// 移動方向に向きを合わせる
	AutoSetRotTarget();
}

//============================================================================
// ブロックの近くで止まる
//============================================================================
bool CEnemy::StopBlockSide()
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

			ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
			if (ImGui::Begin("Test")) {
				ImGui::Text("Y:%f", Norm.y * Norm.y);
				ImGui::End();
			}

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
void CEnemy::GoBack()
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
void CEnemy::DeadEnd()
{

}

//============================================================================
// 高さを補正
//============================================================================
void CEnemy::AdjustHeight()
{
	// 目標座標をY軸の加速度分変動する
	Vec3 NewPosTarget = GetPosTarget();
	NewPosTarget.y += GetVelY();
	SetPosTarget(NewPosTarget);

	// 高さの目標に下限を設定
	if (GetPosTarget().y < m_pBndCylinder->GetHeight())
	{
		// 目標座標を下限に固定
		NewPosTarget.y = m_pBndCylinder->GetHeight();
		SetPosTarget(NewPosTarget);

		// Y軸方向の加速度を初期化
		SetVelY(0.0f);
	}
}

//============================================================================
// 衝突検出
//============================================================================
bool CEnemy::HitCheck()
{
	// プレイヤータイプのオブジェクトを検索
	CObject* pObj = CObject::FindSpecificObject(CObject::TYPE::PLAYER);

	// オブジェクトが見つからければ終了
	if (pObj == nullptr)
		return false;

	// プレイヤークラスにダウンキャスト
	CPlayer* pPlayer = utility::DownCast<CPlayer, CObject>(pObj);

	// プレイヤーの持つ円柱範囲内に侵入していたら衝突
	if (collision::HitCylinderToCylinder(m_pBndCylinder, pPlayer->GetBndCylinder()))
	{
		// 1ダメージを与える
		pPlayer->SetDamage(-1);

		return true;
	}

	return false;
}