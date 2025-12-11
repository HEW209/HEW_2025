///////////////////////////////////////////////
//  TitleUI.h 
//  2025年12月11日作成 佐藤蒼也
//  タイトルのUIアニメーションを管理するクラス
//  
//  
// 
//
//
//////////////////////////////////////////////


#pragma once
#include <DXGameFrame.h>
class TitleUI :
    public Component
{
    void Start() override;
    void Update() override;

private:
    SpriteRenderer* m_pRend;




   
};

