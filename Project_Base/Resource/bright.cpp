//============================================================================
// 
// 閃光 [bright.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "bright.h"
#include "enemy.h"
#include "monster.h"
#include "ghost.h"
#include "flyer.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace abbr;

//****************************************************
// 静的メンバ変数の初期化
//****************************************************

// モーションデータの展開
const JSON CBright::m_MotionData = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\BRIGHT\\bright_motion.json");

// 基礎パラメータの展開
const JSON CBright::m_InitParam = utility::OpenJsonFile("Data\\JSON\\ENVIRONMENT\\BRIGHT\\bright_param.json");

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CBright::CBright() :
	m_nCntDuration{ 0 },
	m_nMaxDuration{ 0 },
	m_CreateType{ CREATETYPE::NONE },
	CMotion_Set{ LAYER::DEFAULT }
{

}

//============================================================================
// デストラクタ
//============================================================================
CBright::~CBright()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CBright::Init()
{
	// モーションオブジェクトの初期設定
	if (FAILED(CMotion_Set::Init()))
	{
		return E_FAIL;
	}

	// 出現モーションをセット
	SetNowMotion(0);

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CBright::Uninit()
{
	// モーションセット終了処理
	CMotion_Set::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CBright::Update()
{
	// 消滅判定
	if (Disappear())
		return;	// 消滅時は更新しない

	// 出現モーションが終了していたら
	if (GetStopState() && GetNowMotion() == 0)
	{
		// 通常モーションに変更
		SetNowMotion(1);
	}

	// モーションセットの更新処理
	CMotion_Set::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CBright::Draw()
{
	// モーションセットの描画処理
	CMotion_Set::Draw();
}

//============================================================================
// 発生
//============================================================================
void CBright::Generate(D3DXVECTOR3 Pos, CREATETYPE Type)
{
	Create(Pos, Type);
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 生成
//============================================================================
void CBright::Create(D3DXVECTOR3 Pos, CREATETYPE Type)
{
	CBright* pNewInstance = DBG_NEW CBright();

	// 生成失敗
	if (!pNewInstance)
	{
		assert(false && "閃光の生成に失敗");
	}

	// 閃光の初期設定
	pNewInstance->Init();

	{ // パラメータを設定

		// データをキャスト
		int
			nMaxDuration = static_cast<int>(m_InitParam["MaxDuration"]);

		// データをセット
		pNewInstance->m_nMaxDuration = nMaxDuration;	// 最大継続期間
		pNewInstance->m_CreateType = Type;				// 生成するエネミータイプ
		pNewInstance->CMotion_Set::SetPos(Pos);			// 座標
	}

	// モーションをセット
	pNewInstance->CMotion_Set::SetMotion(LAYER::DEFAULT, m_MotionData);
}

//============================================================================
// 消滅
//============================================================================
bool CBright::Disappear()
{
	// 消滅モーションで無ければ
	if (GetNowMotion() != 2)
	{
		// 継続期間をインクリメント
		++m_nCntDuration;

		// 最大継続期間に達したら
		if (m_nCntDuration > m_nMaxDuration)
		{
			// 消滅モーションに変更
			SetNowMotion(2);

			// エネミー情報を設定するためのポインタ
			CEnemy* pEnemy = nullptr;

			switch (m_CreateType)
			{
				// 無し
			case CREATETYPE::NONE:

				// 終了
				return false;

				break;

				// モンスター
			case CREATETYPE::MONSTER:
				pEnemy = CMonster::Create();
				break;

				// ゴースト
			case CREATETYPE::GHOST:
				pEnemy = CGhost::Create();
				break;

				// フライヤー
			case CREATETYPE::FLYER:
				pEnemy = CFlyer::Create();
				break;

				// 無効なタイプ指定
			default:
				throw std::runtime_error("Invalid create enemy type");
				break;
			}

			// エネミーはこのエフェクトの場所に配置
			Vec3 BrightPos = GetPos();										// エフェクトの座標をコピー
			pEnemy->SetPos(BrightPos);										// 描画ずれ防止に初期座標をこの場所に
			pEnemy->SetPosTarget(BrightPos);								// 目標座標を変更
			pEnemy->SetDirectionTarget(atan2f(BrightPos.x, BrightPos.z));	// 座標からの方角を割り当て
		}

		// 終了
		return false;
	}

	// 消滅モーション再生が終了したら
	if (GetStopState())
	{
		// 破棄予約
		SetRelease();

		return true;
	}

	return false;
}