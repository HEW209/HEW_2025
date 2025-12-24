#pragma once
#include <DXGameFrame.h>
#include "BlockObject.h"
/*********************************************************************
 * @file   TitlePlauerMove.h
 * @brief  プレイヤーモデルの移動コンポーネント
 *
 * @author 長尾潤弥
 * @date   2025/12/24
 *********************************************************************/
class TitlePlayerMove :
    public Component
{
public:
    TitlePlayerMove();
    ~TitlePlayerMove() = default;

    void Awake()override;
    void SetPlayer(Transform* pPlayerTransform) { m_pPlayerTransform = pPlayerTransform; }
    //更新処理
    void Update()override;
private:

    //true  :カメラから見て手前に戻って来てる
    //false :奥に進んでいる
    bool m_bComeBack;
    //プレイヤーの移動量
    float m_fPlayerMoveSpeed;
    Transform* m_pPlayerTransform;
    //プレイヤーのアニメーション
    MeshGroup::AnimeNo m_motuAnime;
    //アニメーション用
    ObjPtr<MeshRenderer> m_pRenderer;


};

