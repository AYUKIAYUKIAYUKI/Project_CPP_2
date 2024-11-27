//============================================================================
//
// バウンディングボリューム、ヘッダファイル [bouding_volume.h]
// Author : 福田歩希
//
//============================================================================

#ifndef _BOUNDING_VOLUME_H_
#define _BOUNDING_VOLUME_H_	// 二重インクルード防止

//****************************************************
// バウンディングボリュームクラス
//****************************************************
class CBounding_Volume
{
public:

	// <special function>
	CBounding_Volume();					// コンストラクタ
	virtual ~CBounding_Volume() = 0;	// デストラクタ

	// <getter>
	const D3DXVECTOR3& GetCenterPos() const;	// 中心点を取得

	// <virtual setter>
	virtual void SetCenterPos(D3DXVECTOR3 Pos);	// 中心点を設定

private:

	// <data>
	D3DXVECTOR3 m_CenterPos;	// 中心点
};

#endif	// _BOUNDING_VOLUME_H_