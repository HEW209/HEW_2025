/******************************************************************//**
 * @file   SpriteRenderer.h
 * @brief  スプライトの描画を行う
 * 
 * @author 石田怜
 * @date   2025/11/25
 *********************************************************************/
#pragma once

#include <Component/Renderer2D.h>
#include <Utility/Vector2.h>
#include <Utility/Color.h>
#include <DirectX/Material.h>

/**
 * @brief スプライトの描画を行う
 */
class SpriteRenderer : public Renderer2D
{
public:
	SpriteRenderer();
	~SpriteRenderer() = default;

	/**
	 * @brief スプライト描画処理
	 */
	void Draw() override;

	/**
	 * @brief テクスチャを読み込む
	 * @param filePath テクスチャ画像へのファイルパス
	 * @param resize サイズを自動的に設定するか
	 */
	void LoadTexture(const std::string& filePath, bool resize = true);

	/**
	 * @brief オフセット座標を設定する
	 * @param offset オフセット座標
	 */
	void SetOffsetPos(Vector2 offset);

	/**
	 * @brief オフセット座標を設定する
	 * @param x オフセットx座標
	 * @param y オフセットy座標
	 */
	void SetOffsetPos(float x, float y);

	/**
	 * @brief 描画サイズを設定する
	 * @param size 描画サイズ
	 */
	void SetSize(Vector2 size);

	/**
	 * @brief 描画サイズを設定する
	 * yの値はアスペクト比に合わせて自動的に調整される
	 * @param x 描画xサイズ
	 */
	void SetSize(float x);

	/**
	 * @brief 描画サイズを設定する
	 * @param x 描画xサイズ
	 * @param y 描画yサイズ
	 */
	void SetSize(float x, float y);

	/**
	 * @brief UVオフセット座標を設定する
	 * @param offset UVオフセット座標
	 */
	void SetUVOffsetPos(Vector2 offset);

	/**
	 * @brief UVオフセット座標を設定する
	 * @param x UVオフセットu座標
	 * @param y UVオフセットv座標
	 */
	void SetUVOffsetPos(float u, float v);

	/**
	 * @brief UVスケールを設定する
	 * @param scale UVスケール
	 */
	void SetUVScale(Vector2 scale);
	
	/**
	 * @brief UVスケールを設定する
	 * @param u Uスケール
	 * @param v Vスケール
	 */
	void SetUVScale(float u, float v);

	/**
	 * @brief スプライトの乗算カラーを設定する
	 * @param color スプライト乗算カラー
	 */
	void SetColor(Color color);

	/**
	 * @brief スプライトの乗算カラーを設定する
	 * @param r 乗算r成分
	 * @param g 乗算g成分
	 * @param b 乗算b成分
	 * @param a 乗算a成分
	 */
	void SetColor(float r, float g, float b, float a);

	/**
	 * @brief アンチエイリアシング設定を行う
	 * @param isAntiAliasing 画像を滑らかに表示するか
	 */
	void SetAntiAliasing(bool isAntiAliasing);

	/**
	 * @brief 現在のオフセット座標を取得する
	 * @return オフセット座標
	 */
	Vector2 GetOffsetPos();

	/**
	 * @brief 現在の描画サイズを取得する
	 * @return 描画サイズ
	 */
	Vector2 GetSize();

	/**
	 * @brief 現在のUVオフセット座標を取得する
	 * @return UVオフセット座標
	 */
	Vector2 GetUVOffsetPos();

	/**
	 * @brief 現在のUVスケールを取得する
	 * @return UVスケール
	 */
	Vector2 GetUVScale();

	/**
	 * @brief 現在の乗算カラーを取得する
	 * @return 乗算カラー
	 */
	Color GetColor();

	/**
	 * @brief ブレンドステートを設定する
	 * @param blendState
	 */
	void SetBlendState(BlendState blendState);

private:
	/// スプライトのパラメータ
	struct SpriteParameter
	{
		DirectX::XMFLOAT2 offset;
		DirectX::XMFLOAT2 size;
		DirectX::XMFLOAT2 uvOffset;
		DirectX::XMFLOAT2 uvScale;
		DirectX::XMFLOAT4 color;
	};

	/// スプライト描画情報
	SpriteParameter m_parameter;

	/// 描画に使用するマテリアル
	Material m_material;
};
