///////////////////////////////////////////////////////////////////////
//  TitleUI.h 
//  2025年12月11日作成 佐藤蒼也
//  タイトルのUIアニメーションを管理するクラス
//  
//  12/17佐藤蒼也　タイトルロゴ置き換え、いろいろスケール調整
// 
//
//
///////////////////////////////////////////////////////////////////////


#pragma once
#include <DXGameFrame.h>
class TitleUI :
    public Component
{
    void Start() override;
    void Update() override;

private:
    SpriteRenderer* m_pTitle;           //「ハコボット」タイトルロゴ
    SpriteRenderer* m_pRend;            //「はじめる」

    Vector2 m_defaultTitlePos;          //ロゴ初期座標
    Vector2 m_rendDefaultScale;         //初期サイズ取得用
    Color m_rendColor;                  //色変更用
    float m_valueColor;                 //色変更用
    float m_valueTitle;                 //ロゴ位置変更
    bool m_bTitleFall;                  //タイトルが落ちたらtrue
    bool m_bPlayBGM;                    //BGM再生管理用
    bool m_bTitleAnimComplete;          //タイトルアニメーションが完了したらtrue
    bool m_bTitleLanding;               //ロゴ落下音ならしたらtrue
};

