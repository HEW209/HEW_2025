/*****************************************************************//**
 * @file   Renderer2D.h
 * @brief  2D•`‰æƒRƒ“ƒ|[ƒlƒ“ƒg‚ÌŠî’êƒNƒ‰ƒX
 *
 * @author Î“c—å
 * @date   2025/11/24
 *********************************************************************/
#pragma once

#include <GameFrame/Component.h>

/**
 * @brief 2D•`‰æƒRƒ“ƒ|[ƒlƒ“ƒg‚ÌŠî’êƒNƒ‰ƒX
 */
class Renderer2D : public Component
{
public:
	Renderer2D();
	virtual ~Renderer2D();

	/**
	 * @brief •`‰æˆ—
	 */
	virtual void Draw() = 0;

	/**
	 * @brief Œ»İ‚Ì•`‰æ—Dæ“x‚ğæ“¾‚·‚é
	 * @return •`‰æ—Dæ“x (‘å‚«‚¢‚Ù‚Çã‚É•\¦‚³‚ê‚é)
	 */
	int GetOrder() const;

	/**
	 * @brief •`‰æ—Dæ“x‚ğİ’è‚·‚é
	 * @param order •`‰æ—Dæ“x (‘å‚«‚¢‚Ù‚Çã‚É•\¦‚³‚ê‚é)
	 */
	void SetOrder(int order);

	/**
	 * @brief ‚±‚ÌRenderer‚ğUI—p‚Æ‚µ‚Äˆµ‚¤‚©İ’è‚·‚é
	 * @param isUI UIİ’èƒtƒ‰ƒO
	 */
	void SetUI(bool isUI);

	/**
	 * @brief ‚±‚ÌRenderer‚ªUI‚©‚ğæ“¾‚·‚é
	 * @return UI‚È‚çtrue‚ğ•Ô‚·
	 */
	bool IsUI();

	/**
	 * @brief ‚±‚ÌRenderer‚ğ”wŒi‚Æ‚µ‚Äˆµ‚¤‚©İ’è‚·‚é
	 * @param isBackGround ”wŒiİ’èƒtƒ‰ƒO
	 */
	void SetBackGround(bool isBackGround);

	/**
	 * @brief ‚±‚ÌRenderer‚ª”wŒi‚©‚ğæ“¾‚·‚é
	 * @return ”wŒi•`‰æ‚È‚çtrue‚ğ•Ô‚·
	 */
	bool IsBackGround();

private:
	/// •`‰æ—Dæ“x
	int m_drawOrder;

	/// UIƒtƒ‰ƒO
	bool m_isUI;

	/// ”wŒi•`‰æ
	bool m_isBackGround;
};