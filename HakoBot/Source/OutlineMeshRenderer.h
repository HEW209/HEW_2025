#pragma once

#include <DXGameFrame.h>

class OutlineMeshRenderer : public MeshRenderer
{
public:
	OutlineMeshRenderer();
	virtual ~OutlineMeshRenderer() = default;

	/**
	 * @brief 描画処理
	 */
	virtual void Draw() override;

	/**
	 * @brief モデルデータを読み込む
	 * @param filePath モデルデータへのファイルパス
	 */
	virtual void LoadModel(const std::string& filePath);

	/**
	 * @brief アウトライン描画を有効化/無効化する
	 * @param shouldDrawOutline アウトライン描画を有効化する場合はtrue、無効化する場合はfalse
	 */
	void SetShouldDrawOutline(bool shouldDrawOutline) { m_shouldDrawOutline = shouldDrawOutline; }

	/**
	 * @brief アウトライン描画が有効かを取得する
	 * @return アウトライン描画が有効な場合はtrue、無効な場合はfalse
	 */
	bool ShouldDrawOutline() const { return m_shouldDrawOutline; }

	/**
	 * @brief アウトラインの色を設定する
	 * @param color アウトラインの色
	 */
	void SetOutlineColor(Color color) { m_outlineColor = color; }

	/**
	 * @brief アウトラインの色を取得する
	 * @return アウトラインの色
	 */
	Color GetOutlineColor() { return m_outlineColor; }

	/**
	 * @brief アウトラインの太さを設定する
	 * @param thickness アウトラインの太さ
	 */
	void SetOutlineThickness(float thickness) { m_outlineThickness = thickness; }

	/**
	 * @brief アウトラインの太さを取得する
	 * @return アウトラインの太さ
	 */
	float GetOutlineThickness() const { return m_outlineThickness; }

private:
	///	アウトラインの色
	Color m_outlineColor;
	/// アウトラインの太さ
	float m_outlineThickness;
	///	アウトラインを描画するか
	bool m_shouldDrawOutline;
	/// モデルの
};