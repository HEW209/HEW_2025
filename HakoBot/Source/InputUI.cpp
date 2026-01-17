#include "InputUI.h"
#include "InputManager.h"
#include "GameState.h"

static const Vector2 g_buttonOffset(-1.25f, 0.0f);  // ボタン位置
static const Vector2 g_menuUIPos(-4.8f, 3.3f);      // メニュー位置
static const Vector2 g_basicUIPos(5.7f, -3.2f);     // 基本操作位置
static const Vector2 g_specialUIPos(-4.8f, -3.2f);  // 特殊操作位置
static const float g_offsetDistance = 0.5f;         // UIの間隔
static const float g_textSize = 200.0f;             // テキストサイズ
static const float g_buttonSize = 50.0f;            // ボタンサイズ
static const float g_pushScale = 0.9f;              // ボタン押下時のスケーリング

InputUI::InputUI() :
    m_menuButton(nullptr),
    m_menuText(nullptr),
    m_basicTexts(),
    m_basicButtons(),
    m_buttonLB(nullptr),
    m_specialTexts(nullptr),
    m_specialButtons(nullptr)
{
}

void InputUI::Awake()
{
    // メニュー
    {
        m_menuText = GetGameObject()->AddComponent<SpriteRenderer>();
        m_menuText->SetUI(true);
        m_menuText->LoadTexture("Assets/Textures/Texts/menu.png");
        m_menuText->SetSize(g_textSize);
        m_menuText->SetOffsetPos(g_menuUIPos);

        m_menuButton = GetGameObject()->AddComponent<SpriteRenderer>();
        m_menuButton->SetUI(true);
        m_menuButton->LoadTexture("Assets/Textures/Button/Button_Menu.png");
        m_menuButton->SetSize(g_buttonSize);
        m_menuButton->SetOffsetPos(g_menuUIPos + g_buttonOffset);
    }

    // 基本操作
    for (int i = 0; i < (UINT)Basic::COUNT; ++i)
    {
        Vector2 offset(0.0f, i * g_offsetDistance);

        // テキスト
        auto text = GetGameObject()->AddComponent<SpriteRenderer>();
        text->SetUI(true);
        switch ((Basic)i)
        {
        case Basic::KAITEN:
            text->LoadTexture("Assets/Textures/Texts/kaiten.png");
            break;
        case Basic::TAKASA:
            text->LoadTexture("Assets/Textures/Texts/takasa.png");
            break;
        case Basic::MOTU:
            text->LoadTexture("Assets/Textures/Texts/motu.png");
            break;
        }
        text->SetSize(g_textSize);
        text->SetOffsetPos(g_basicUIPos + offset);
        m_basicTexts[i] = text;

        // ボタン
        offset.x -= g_buttonSize * 0.01f * 0.5f;
        auto button = GetGameObject()->AddComponent<SpriteRenderer>();
        button->SetUI(true);
        switch ((Basic)i)
        {
        case Basic::KAITEN:
            button->LoadTexture("Assets/Textures/Button/Button_RB.png");
            break;
        case Basic::TAKASA:
            button->LoadTexture("Assets/Textures/Button/UpDown.png");
            break;
        case Basic::MOTU:
            button->LoadTexture("Assets/Textures/Button/Button_B.png");
            break;
        }
        button->SetSize(g_buttonSize);
        button->SetOffsetPos(g_basicUIPos + g_buttonOffset + offset);
        m_basicButtons[i] = button;

        if (i == (UINT)Basic::KAITEN)
        {
            Vector2 offsetRB = offset;
            offsetRB.x += g_buttonSize * 0.01f * 0.45f;
            button->SetOffsetPos(g_basicUIPos + g_buttonOffset + offsetRB);

            auto buttonLB = GetGameObject()->AddComponent<SpriteRenderer>();
            buttonLB->SetUI(true);
            buttonLB->LoadTexture("Assets/Textures/Button/Button_LB.png");
            buttonLB->SetSize(g_buttonSize);
            Vector2 offsetLT = offset;
            offsetLT.x -= g_buttonSize * 0.01f * 0.45f;
            buttonLB->SetOffsetPos(g_basicUIPos + g_buttonOffset + offsetLT);
            m_buttonLB = buttonLB;
        }
    }

    // 特殊操作
    for (int i = 0; i < (UINT)Special::COUNT; ++i)
    {
        Vector2 offset(0.0f, i * g_offsetDistance);

        // テキスト
        auto text = GetGameObject()->AddComponent<SpriteRenderer>();
        text->SetUI(true);
        switch ((Special)i)
        {
        case Special::BLOCK_GHOST:
            text->LoadTexture("Assets/Textures/Texts/ghost.png");
            break;
        case Special::ROTATE_LOCK:
            text->LoadTexture("Assets/Textures/Texts/muki.png");
            break;
        }
        text->SetSize(g_textSize);
        text->SetOffsetPos(g_specialUIPos + offset);
        m_specialTexts[i] = text;

        // ボタン
        auto button = GetGameObject()->AddComponent<SpriteRenderer>();
        button->SetUI(true);
        switch ((Special)i)
        {
        case Special::BLOCK_GHOST:
            button->LoadTexture("Assets/Textures/Button/Button_LT.png");
            break;
        case Special::ROTATE_LOCK:
            button->LoadTexture("Assets/Textures/Button/Button_RT.png");
            break;
        }
        button->SetSize(g_buttonSize);
        button->SetOffsetPos(g_specialUIPos + g_buttonOffset + offset);
        m_specialButtons[i] = button;
    }
}

void InputUI::Update()
{
    SwitchButton();
    PushScaling();
}

void InputUI::SwitchButton()
{
    if (GameState::GetInstance()->GetPlayer()->GetBlockHandler()->HasBlock())
    {
        m_basicTexts[(UINT)Basic::MOTU]->LoadTexture("Assets/Textures/Texts/oku.png", false);
        m_buttonLB->SetEnabled(true);
        m_basicButtons[(UINT)Basic::KAITEN]->SetEnabled(true);
        m_basicTexts[(UINT)Basic::KAITEN]->SetEnabled(true);
    }
    else
    {
        m_basicTexts[(UINT)Basic::MOTU]->LoadTexture("Assets/Textures/Texts/motu.png", false);
        m_buttonLB->SetEnabled(false);
        m_basicButtons[(UINT)Basic::KAITEN]->SetEnabled(false);
        m_basicTexts[(UINT)Basic::KAITEN]->SetEnabled(false);
    }
}

void InputUI::PushScaling()
{
    if (InputManager::CurrentInputSystem().GetButtonHold("Menu"_hash))
    {
        m_menuButton->SetSize(g_buttonSize * g_pushScale);
    }
    else
    {
        m_menuButton->SetSize(g_buttonSize);
    }

    if (InputManager::CurrentInputSystem().GetButtonHold("PlaceAndRemove"_hash))
    {
        m_basicButtons[(UINT)Basic::MOTU]->SetSize(g_buttonSize * g_pushScale);
    }
    else
    {
        m_basicButtons[(UINT)Basic::MOTU]->SetSize(g_buttonSize);
    }

    if (InputManager::CurrentInputSystem().GetButtonHold("Up"_hash) ||
        InputManager::CurrentInputSystem().GetButtonHold("Down"_hash))
    {
        m_basicButtons[(UINT)Basic::TAKASA]->SetSize(g_buttonSize * g_pushScale);
    }
    else
    {
        m_basicButtons[(UINT)Basic::TAKASA]->SetSize(g_buttonSize);
    }

    if (InputManager::CurrentInputSystem().GetButtonHold("RotateBlockRight"_hash))
    {
        m_basicButtons[(UINT)Basic::KAITEN]->SetSize(g_buttonSize * g_pushScale);
    }
    else
    {
        m_basicButtons[(UINT)Basic::KAITEN]->SetSize(g_buttonSize);
    }

    if (InputManager::CurrentInputSystem().GetButtonHold("RotateBlockLeft"_hash))
    {
        m_buttonLB->SetSize(g_buttonSize * g_pushScale);
    }
    else
    {
        m_buttonLB->SetSize(g_buttonSize);
    }
}
