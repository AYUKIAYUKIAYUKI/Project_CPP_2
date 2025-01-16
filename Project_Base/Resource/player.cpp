//============================================================================
// 
// プレイヤー [player.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "player.h"
#include "bounding_cylinder.h"
#include "player_state.h"
#include "player_state_default.h"
#include "player_state_dash.h"
#include "player_state_damage.h"
#include "object_Parts.h"

#include "block.h"
#include "collision.h"
#include "field_manager.h"

//****************************************************
// プリプロセッサディレクティブ
//****************************************************
#define SHOW_NORMALIZED_POSITION_HITCHECK 0	// 正規化座標での当たり判定を表示

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
CPlayer::CPlayer() :
	CCharacter{ LAYER::DEFAULT },
	m_pBndCylinder{ DBG_NEW CBounding_Cylinder() },
	m_pState{ nullptr }
{

}

//============================================================================
// デストラクタ
//============================================================================
CPlayer::~CPlayer()
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
HRESULT CPlayer::Init()
{
	// 初期ステートの生成
	if (!m_pState)
	{
		m_pState = DBG_NEW CPlayer_State_Default();
	}
	
	// キャラクタークラスの初期設定
	if (FAILED(CCharacter::Init()))
	{
		assert(false && "プレイヤークラスの初期設定に失敗");
	}

	// 補正強度を設定
	SetCorrectCoef(CORRECT_COEF);
	
	// 初期方角を設定
	SetDirection(D3DX_PI * -0.5f);
	SetDirectionTarget(D3DX_PI * -0.5f);

	// 初期の向きをセット
	SetRot({ 0.0f, D3DX_PI, 0.0f });
	SetRotTarget({ 0.0f, D3DX_PI, 0.0f });

	// 初期の座標をセット
	SetPos({ 0.0f, 60.0f, 0.0f });
	SetPosTarget({ 0.0f, 60.0f, 0.0f });

	// 初期移動速度を設定
	SetMoveSpeed(DEFAULT_MOVE_SPEED);

	// 初期体力を設定
	SetLife(MAX_LIFE);

	// 飛行モーションをセット
	SetNowMotion(8);

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CPlayer::Uninit()
{
	// ステート破棄
	if (m_pState)
	{
		// ステートの破棄
		delete m_pState;
		
		// ポインタを初期化
		m_pState = nullptr;
	}

	// キャラクタークラスの終了処理
	CCharacter::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CPlayer::Update()
{
	// ステートの変更確認
	m_pState = m_pState->CheckChangeState<CPlayer_State>();

	// ステートの更新
	m_pState->Update();

	// 高さの補正
	AdjustHeight();

	// 座標を方角に合わせる
	AutoSetPosTarget();

	// 当たり判定 (判定手順は、変動した目標方角・目標座標がセーフかどうか)
	HitCheck();

	// 円柱バウンディングの中心点をプレイヤーの座標に
	m_pBndCylinder->SetCenterPos(GetPos());

	// キャラクタークラスの更新処理
	CCharacter::Update();

#if 1
#ifdef _DEBUG
	// ウィンドウを表示
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Player Param"))
	{
		ImGui::Text("State:%s", typeid(*m_pState).name());
		ImGui::Text("Direction:%.3f", GetDirection() * (180 / D3DX_PI));
		ImGui::Text("DirectionTarget:%.3f", GetDirectionTarget() * (180 / D3DX_PI));
		ImGui::Text("Speed:%.1f", GetMoveSpeed());
		ImGui::Text("Rot:X %.1f:Y %.1f:Z %.1f", GetRot().x * (180 / D3DX_PI), GetRot().y * (180 / D3DX_PI), GetRot().z * (180 / D3DX_PI));
		ImGui::Text("RotTarget:X %.1f:Y %.1f:Z %.1f", GetRotTarget().x * (180 / D3DX_PI), GetRotTarget().y * (180 / D3DX_PI), GetRotTarget().z * (180 / D3DX_PI));
		ImGui::Text("Pos:X %.1f:Y %.6f:Z %.1f", GetPos().x, GetPos().y, GetPos().z);
		ImGui::Text("PosTarget:X %.1f:Y %.6f:Z %.1f", GetPosTarget().x, GetPosTarget().y, GetPosTarget().z);
		ImGui::Text("VelY:%.1f", GetVelY());
		ImGui::Text("Life:%d", GetLife());
		ImGui::Text("Motion:%d", GetNowMotion());
		ImGui::End();
	}
#endif	// _DEBUG
#endif
}

//============================================================================
// 描画処理
//============================================================================
void CPlayer::Draw()
{

}

//============================================================================
// ダメージを受ける
//============================================================================
void CPlayer::SetDamage(int nDamage, float fImpact)
{
	if (typeid(*m_pState) != typeid(CPlayer_State_Damage))
	{
		// ダメージ量分、体力を変動
		int nNewLife = GetLife();
		nNewLife += nDamage;
		SetLife(nNewLife);

		// 軽く上に吹き飛ぶ
		SetVelY(3.0f);

		// 衝撃量分、方角を変動
		float fNewDirectionTarget = GetDirectionTarget();
		fNewDirectionTarget += fImpact;
		SetDirectionTarget(fNewDirectionTarget);

		// ダメージ状態へ変更するよう命令する
		m_pState->To_Damage();
	}
}

//============================================================================
// 浮遊を有効化
//============================================================================
void CPlayer::EnableWings()
{
	
}

//============================================================================
// 半径を取得
//============================================================================
float CPlayer::GetRadius() const
{
	return m_pBndCylinder->GetRadius();
}

//============================================================================
// 高さを取得
//============================================================================
float CPlayer::GetHeight() const
{
	return m_pBndCylinder->GetHeight();
}

//============================================================================
// バウンディングシリンダーを取得
//============================================================================
const CBounding_Cylinder* const CPlayer::GetBndCylinder() const
{
	return m_pBndCylinder;
}

//============================================================================
// 現在のステートを取得
//============================================================================
const CPlayer_State* const CPlayer::GetNowState() const
{
	// ステートのポインタを返す
	return m_pState;
}

//============================================================================
// 生成
//============================================================================
CPlayer* CPlayer::Create()
{
	// インスタンスを生成
	CPlayer* pNewInstance = DBG_NEW CPlayer();

	// タイプを設定
	pNewInstance->SetType(TYPE::PLAYER);
	
	// 初期設定
	pNewInstance->Init();

	// モーションをセット
	pNewInstance->SetMotion(LAYER::DEFAULT, utility::OpenJsonFile("Data\\JSON\\CHARACTER\\PLAYER\\player_motion.json"));

	// 半径を設定
	pNewInstance->m_pBndCylinder->SetRadius(3.0f);

	// 高さを設定
	pNewInstance->m_pBndCylinder->SetHeight(8.5f);

	return pNewInstance;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 高さを補正
//============================================================================
void CPlayer::AdjustHeight()
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
// 当たり判定
//============================================================================
void CPlayer::HitCheck()
{
	// 衝突の有無を記録
	bool bDetect = false;

	// 通常優先度のオブジェクトを取得
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	while (pObj != nullptr)
	{
		// ブロッククラスにダウンキャスト
		CBlock* pBlock = CObject::DownCast<CBlock>(pObj);

		// キャスト失敗で
		if (!pBlock)
		{
			pObj = pObj->GetNext();
			continue;
		}

		// プレイヤーのパラメータをコピー
		const Vec3& CylinderPosTarget = GetPosTarget();	// 目標座標
		const Vec3& CylinderOldPos = GetPos();			// 現在座標
		const float& CylinderRadius = GetRadius();		// 半径
		const float& CylinderHeight = GetHeight();		// 高さ

		// バウンディングボックスのパラメータをコピー
		const Vec3& BoxSize = pBlock->GetSize();			// ブロックの座標
		const Vec3& BoxPos = pBlock->GetPos();				// ブロックの座標
		const float& fBoxDirection = pBlock->GetRot().y;	// ブロックの回転方向
#if SHOW_NORMALIZED_POSITION_HITCHECK
		{
			TESTTEST->SetScale(BoxSize);
		}
#endif	// _SHOW_NORMALIZED_POSITION_HITCHECK_

		// ①ボックスの中心点からシリンダーの座標への距離ベクトルを計算
		const Vec3& RelativePos = CylinderPosTarget - BoxPos;
		const Vec3& RelativeOldPos = CylinderOldPos - BoxPos;

		// ②座標系を正規化するため、距離ベクトルにボックスの回転量を打ち消すように回転行列を適用
		const Vec3& ResultPos = utility::RotatePointAroundY(-fBoxDirection, RelativePos);
		const Vec3& ResultOldPos = utility::RotatePointAroundY(-fBoxDirection, RelativeOldPos);
#if SHOW_NORMALIZED_POSITION_HITCHECK
		{
			ててて->SetScale({ GetBndCylinder()->GetRadius(), GetBndCylinder()->GetHeight(), GetBndCylinder()->GetRadius() });
			ててて->SetPos(ResultPos);
		}
#endif	// _SHOW_NORMALIZED_POSITION_HITCHECK_

		// 座標系を正規化した仮定し、ボックスからシリンダーへの距離ベクトルを用いて衝突判定
		// (ボックスの座標は原点で、回転は考慮せず、AABBと仮定する)
		if (collision::HitCylinderToAABB(ResultPos, CylinderRadius, CylinderHeight, VEC3_INIT, BoxSize))
		{
#ifdef _DEBUG
			// 判定表示を赤色に
			//m_pBndCylinder->SetCol({ 1.0f, 0.0f, 0.0f, 0.5f });
#endif // _DEBUG
				
			// 衝突があったことを記録
			bDetect = 1;
			
			// 衝突面を判定
			int nIdx = collision::GetCylinderToAABB(ResultOldPos, ResultPos, CylinderRadius, CylinderHeight, VEC3_INIT, BoxSize);

			// 上下の判定は2DのY軸判定と変わらないので普通にブロックの上下幅で押し出し
			// 左右の判定は正規化した座標で押し出しを行った後に回転させて本来のあるべき座標を設定
			switch (nIdx)
			{
			case 0:	// 分類不能
				break;

			case 1: // 上
			{
				// Y軸方向の加速度を初期化
				SetVelY(0.0f);

				// 新しい目標座標を作成
				const Vec3& NewPosTarget = { CylinderPosTarget.x,
					BoxPos.y + BoxSize.y + CylinderHeight,
					CylinderPosTarget.z };
			 
				// 目標座標を反映
				SetPosTarget(NewPosTarget);
			
				break;
			}
			case 2: // 下
			{
				// Y軸方向の加速度を重力方向へ固定
				SetVelY(0.0f);

				// 新しい目標座標を作成
				const Vec3& NewPosTarget = {
					CylinderPosTarget.x,
					BoxPos.y - BoxSize.y - CylinderHeight,
					CylinderPosTarget.z };

				// 目標座標を反映
				SetPosTarget(NewPosTarget);

				break;
			}
			case 3:	// 左
			{
#if 0	// 押し出し方法の切り替え (調整中)
				// 正規化された座標系内で新しい目標座標を作成
				Vec3 NewPosTarget = {
					/* BoxPos.x <- 0.0f */ + BoxSize.x + CylinderRadius,
					CylinderPosTarget.y,
					//(ResultOldPos.z/* - BoxPos.z <- 0.0f */) * (ResultPos.x / ResultOldPos.x)};
					//ResultOldPos.z};
					CylinderOldPos.z * (ResultPos.x / ResultOldPos.x) };
				/* ↑Z座標について、円弧で移動するため、膨大なスイープ判定を行うことになってしまう…
				ボックスの中心から元のZ座標への方向成分をXのサイズ分と均等な割合になるように調整する */

				// 打ち消した回転を再度かけ合わせて本来の向きに戻す
				NewPosTarget = utility::RotatePointAroundY(fBoxDirection + D3DX_PI * 0.5f, NewPosTarget);

				CRenderer::SetTimeString("正規化座標系での座標:x " + to_string(NewPosTarget.x) + ":y " + to_string(NewPosTarget.y) + ":z " + to_string(NewPosTarget.z), 600);
				CRenderer::SetTimeString("Z割合" + to_string((ResultPos.x / ResultOldPos.x)), 600);

				// 直した向きからいるべき方角を補正
				SetDirection(atan2f(NewPosTarget.x, NewPosTarget.z));

				// 補正方角から目標座標を設定
				AutoSetPosTarget();
				
				CRenderer::SetTimeString("通常座標系での座標:x " + to_string(GetPosTarget().x) + ":y " + to_string(GetPosTarget().y) + ":z " + to_string(GetPosTarget().z), 600);
#else
				//// 変更された目標方角の現在の方角へ戻す	/* パターン1 */
				//SetDirectionTarget(GetDirection());

				//// 変更された目標座標を現在の座標に戻す
				//AutoSetPosTarget();
#endif
				// 衝突前の座標から、当たる直前となる方角を出す	/* パターン2 */
				SetDirectionTarget(atan2f(CylinderOldPos.z, CylinderOldPos.x));

				// 衝突寸前の方角から目標座標を割り出す
				AutoSetPosTarget();

				break;
			}
			case 4:	// 右
			{
				/* パターン2 */

				// 衝突前の座標から、当たる直前となる方角を出す	/* パターン2 */
				SetDirectionTarget(atan2f(CylinderOldPos.z, CylinderOldPos.x));

				// 衝突寸前の方角から目標座標を割り出す
				AutoSetPosTarget();

				break;
			}
			default:
				break;
			}

			break;
		}

		pObj = pObj->GetNext();
	}

	// 何にも衝突していなければ
	if (!bDetect)
	{
#ifdef _DEBUG
		// 判定表示を通常色に戻す
		//m_pBndCylinder->SetCol({ 1.0f, 1.0f, 1.0f, 0.5f });
#endif // _DEBUG
	}
}