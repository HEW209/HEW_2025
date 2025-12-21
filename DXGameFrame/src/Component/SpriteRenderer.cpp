// SpriteRenderer.cpp
#include <Component/SpriteRenderer.h>
#include <GameFrame/Transform.h>
#include <DirectX/SpriteDrawer.h>
#include <DirectX/ConfigManager.h>
#include <DirectX/ConstantBufferManager.h>

SpriteRenderer::SpriteRenderer()
{
	SetOffsetPos(0.0f, 0.0f);
	SetSize(100.0f, 100.0f);
	SetUVOffsetPos(0.0f, 0.0f);
	SetUVScale(1.0f, 1.0f);
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	// デフォルトシェーダーとテクスチャをセット
	m_material.SetVertexShader(ConfigManager::Instance().GetString(ConfigKey::DefaultSpriteVS));
	m_material.SetPixelShader(ConfigManager::Instance().GetString(ConfigKey::DefaultSpritePS));
	m_material.SetTexture(ConfigManager::Instance().GetString(ConfigKey::DefaultTexture));

	// スプライト描画用パイプラインステートをセット
	PipelineState pipelineState;
	pipelineState.rasterizerState = RasterizerState::NONE;
	pipelineState.depthStencilState = DepthStencilState::DISABLE;
	pipelineState.samplerState = SamplerState::LINEAR_WRAP;
	pipelineState.blendState = BlendState::ALPHA;
	m_material.SetPipelineState(pipelineState);
}

void SpriteRenderer::Draw()
{
	// Transformからワールド行列をセット
	DirectX::XMMATRIX matrix;
	matrix = GetTransform()->GetWorldMatrix();
	ConstantBufferManager::Instance().SetWorld(matrix);

	m_material.SetParameter(&m_parameter, sizeof(m_parameter));
	SpriteDrawer::Instance().Draw(m_material);
}

void SpriteRenderer::LoadTexture(const std::string& filePath, bool resize)
{
	m_material.SetTexture(filePath, TextureSlot::Main);
	
	if (resize)
	{
		// スプライトサイズをテクスチャサイズに合わせる
		Texture* pTexture = m_material.GetTexture(TextureSlot::Main);
		if (pTexture != nullptr)
		{
			Texture::Size size = pTexture->GetSize();
			SetSize((float)size.x, (float)size.y);
		}
	}
}

void SpriteRenderer::SetOffsetPos(Vector2 offset)
{
	SetOffsetPos(offset.x, offset.y);
}

void SpriteRenderer::SetOffsetPos(float x, float y)
{
	m_parameter.offset.x = x;
	m_parameter.offset.y = y;
}

void SpriteRenderer::SetSize(Vector2 size)
{
	SetSize(size.x, size.y);
}

void SpriteRenderer::SetSize(float x)
{
	float y;
	Texture* pTexture = m_material.GetTexture(TextureSlot::Main);
	if (pTexture != nullptr)
	{
		// 縦のサイズを取得
		Texture::Size size = pTexture->GetSize();
		y = x * (float)size.y / (float)size.x;
	}
	else
	{
		y = x;
	}

	SetSize(x, y);
}

void SpriteRenderer::SetSize(float x, float y)
{
	int pixelPerUnit = ConfigManager::Instance().GetInt(ConfigKey::PixelPerUnit);
	m_parameter.size.x = x / (float)pixelPerUnit;
	m_parameter.size.y = y / (float)pixelPerUnit;
}

void SpriteRenderer::SetUVOffsetPos(Vector2 offset)
{
	SetUVOffsetPos(offset.x, offset.y);
}

void SpriteRenderer::SetUVOffsetPos(float u, float v)
{
	m_parameter.uvOffset.x = u;
	m_parameter.uvOffset.y = v;
}

void SpriteRenderer::SetUVScale(Vector2 scale)
{
	SetUVScale(scale.x, scale.y);
}

void SpriteRenderer::SetUVScale(float u, float v)
{
	m_parameter.uvScale.x = u;
	m_parameter.uvScale.y = v;
}

void SpriteRenderer::SetColor(Color color)
{
	SetColor(color.r, color.g, color.b, color.a);
}

void SpriteRenderer::SetColor(float r, float g, float b, float a)
{
	m_parameter.color.x = r;
	m_parameter.color.y = g;
	m_parameter.color.z = b;
	m_parameter.color.w = a;
}

void SpriteRenderer::SetAntiAliasing(bool isAntiAliasing)
{
	if (isAntiAliasing)
	{
		m_material.SetSamplerState(SamplerState::LINEAR_WRAP);
	}
	else
	{
		m_material.SetSamplerState(SamplerState::POINT_WRAP);
	}
}

Vector2 SpriteRenderer::GetOffsetPos()
{
	return Vector2(m_parameter.offset.x, m_parameter.offset.y);
}

Vector2 SpriteRenderer::GetSize()
{
	int pixelPerUnit = ConfigManager::Instance().GetInt(ConfigKey::PixelPerUnit);
	return Vector2(m_parameter.size.x, m_parameter.size.y) * (float)pixelPerUnit;
}

Vector2 SpriteRenderer::GetUVOffsetPos()
{
	return Vector2(m_parameter.uvOffset.x, m_parameter.uvOffset.y);
}

Vector2 SpriteRenderer::GetUVScale()
{
	return Vector2(m_parameter.uvScale.x, m_parameter.uvScale.y);
}

Color SpriteRenderer::GetColor()
{
	return Color(m_parameter.color.x, m_parameter.color.y,
		m_parameter.color.z, m_parameter.color.w);
}

void SpriteRenderer::SetBlendState(BlendState blendState)
{
	m_material.SetBlendState(blendState);
}
