// BlockObject.cpp

#include "BlockObject.h"
#include "GameState.h"
#include <Component/Collider.h>

void BlockObject::Awake()
{
	m_pBlockMeshRenderer = GetGameObject()->AddComponent<OutlineMeshRenderer>();
	m_pBlockMeshRenderer->SetEnabled(false);
	m_pBlockMeshRenderer->SetShouldDrawOutline(false);
	m_pBlockMeshRenderer->SetOutlineColor(Color{1.0f, 0.5f, 0.0f, 1.0f});
	m_pBlockMeshRenderer->SetOutlineThickness(5.0f);
	m_pBlockMeshRenderer->SetShouldDrawShadow(true);

	for (auto&& material : *m_pBlockMeshRenderer->GetMaterials()) {
		material.SetPixelShader("Assets/Shader/Default_PS.cso");
		material.SetBlendState(BlendState::DEFAULT);
		material.SetDepthStencilState(DepthStencilState::DEFAULT);
	}

	m_isTransparent = GameState::GetInstance()->IsBlockTransparent();
	SetTransparent(m_isTransparent);
}

void BlockObject::Update()
{
	if (GameState::GetInstance()->IsBlockTransparent() != m_isTransparent)
	{
		m_isTransparent = !m_isTransparent;
		SetTransparent(m_isTransparent);
	}
}

void BlockObject::OnDestroy()
{
	for (auto&& block : m_pBlocks) {
		if (block) {
			block->Destroy();
		}
	}
}

void BlockObject::SetBlockSet(const BlockSetData& blockSet)
{
	m_blockSet = blockSet;

	for (auto&& block : m_pBlocks) {
		if (block) {
			block->Destroy();
		}
	}
	m_pBlocks.clear();
	m_pBlocks.reserve(m_blockSet.blocks.size());

	Vector3 min{ std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
	Vector3 max{ std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest() };
	for (auto&& blockPos : blockSet.blocks) {
		auto obj = SceneManager::GetActiveScene()->CreateGameObject();
		if (m_shouldUseCollider)
		{
			obj->AddComponent<Collider>();
		}

		// ブロックの最小位置を記録
		min.x = std::min(min.x, (float)blockPos.x);
		min.y = std::min(min.y, (float)blockPos.y);
		min.z = std::min(min.z, (float)blockPos.z);

		// ブロックの最大位置を記録
		max.x = std::max(max.x, (float)blockPos.x);
		max.y = std::max(max.y, (float)blockPos.y);
		max.z = std::max(max.z, (float)blockPos.z);

		auto transform = obj->GetTransform();
		transform->SetParent(GetTransform());
		transform->SetPosition(blockPos.x, blockPos.y, blockPos.z, Space::LOCAL);
	
		m_pBlocks.push_back(obj);
	}
	m_size = max - min + Vector3(1.0f, 1.0f, 1.0f);
	m_center = (min + max) * 0.5f;
}

void BlockObject::SetModel(const std::string& modelPath)
{
	m_modelPath = modelPath;
	if (modelPath.empty()) {
		m_pBlockMeshRenderer->SetEnabled(false);
	}
	else {
		m_pBlockMeshRenderer->LoadModel(modelPath);
		m_pBlockMeshRenderer->SetEnabled(true);
	}

	m_isTransparent = GameState::GetInstance()->IsBlockTransparent();
	SetTransparent(m_isTransparent);
}

void BlockObject::SetSelect(bool isSelected)
{
	m_pBlockMeshRenderer->SetShouldDrawOutline(isSelected);
}

float BlockObject::GetGroundYOffset()
{
	if (m_blockSet.blocks.empty()) {
		return 0.0f;
	}

	// ブロック（立方体）の中心から見た8つの頂点へのオフセット
	const std::vector<Vector3> cornerOffsets = {
		{ -0.5f, -0.5f, -0.5f },
		{  0.5f, -0.5f, -0.5f },
		{ -0.5f,  0.5f, -0.5f },
		{  0.5f,  0.5f, -0.5f },
		{ -0.5f, -0.5f,  0.5f },
		{  0.5f, -0.5f,  0.5f },
		{ -0.5f,  0.5f,  0.5f },
		{  0.5f,  0.5f,  0.5f }
	};

	float minY = (std::numeric_limits<float>::max)();

	const Quaternion rotation = GetTransform()->GetQuaternion();

	for (const auto& blockPos : m_blockSet.blocks) {
		// ブロックの中心座標
		const Vector3 center(static_cast<float>(blockPos.x), static_cast<float>(blockPos.y), static_cast<float>(blockPos.z));

		for (const auto& offset : cornerOffsets) {
			// ブロックの頂点座標
			Vector3 corner = center + offset;

			Vector3 rotatedCorner = rotation * corner;

			if (rotatedCorner.y < minY) {
				minY = rotatedCorner.y;
			}
		}
	}

	return (minY == (std::numeric_limits<float>::max)()) ? 0.0f : -minY;
}

Vector3 BlockObject::GetGroundOffset()
{
	const float yOffset = GetGroundYOffset();

	return Vector3(0.0f, yOffset, 0.0f);
}

bool BlockObject::IsInside(const Vector3& worldPosition, float inflationAmount) const
{
	Transform* pTransform = GetTransform();
	if (!pTransform)
	{
		return false;
	}

	// このBlockObjectの逆ワールド行列を取得する
	DirectX::XMMATRIX worldMatrix = pTransform->GetWorldMatrix();
	DirectX::XMMATRIX inverseWorldMatrix = DirectX::XMMatrixInverse(nullptr, worldMatrix);

	DirectX::XMVECTOR worldPosVec = worldPosition.ToXMVector();

	// 座標をローカル空間に変換する
	DirectX::XMVECTOR localPosVec = DirectX::XMVector3TransformCoord(worldPosVec, inverseWorldMatrix);

	DirectX::XMFLOAT3 localPosFloat3;
	DirectX::XMStoreFloat3(&localPosFloat3, localPosVec);
	Vector3 localPosition(localPosFloat3.x, localPosFloat3.y, localPosFloat3.z);

	// ローカル座標で、各ブロックのAABBと当たり判定を行う
	for (const auto& blockPos : m_blockSet.blocks)
	{
		const float centerX = static_cast<float>(blockPos.x);
		const float centerY = static_cast<float>(blockPos.y);
		const float centerZ = static_cast<float>(blockPos.z);

		float boxHalfSize = 0.5f + inflationAmount;

		// AABB
		const float minX = centerX - boxHalfSize;
		const float maxX = centerX + boxHalfSize;
		const float minY = centerY - boxHalfSize;
		const float maxY = centerY + boxHalfSize;
		const float minZ = centerZ - boxHalfSize;
		const float maxZ = centerZ + boxHalfSize;

		// ローカル座標がAABBの内側にあるかチェック
		if (localPosition.x >= minX && localPosition.x <= maxX &&
			localPosition.y >= minY && localPosition.y <= maxY &&
			localPosition.z >= minZ && localPosition.z <= maxZ)
		{
			return true;
		}
	}

	return false;
}

std::vector<Vector3> BlockObject::GetBlockVertices()
{
	std::vector<Vector3> vertices;

	const std::vector<Vector3> cornerOffsets = {
		{ -0.5f, -0.5f, -0.5f },
		{  0.5f, -0.5f, -0.5f },
		{ -0.5f,  0.5f, -0.5f },
		{  0.5f,  0.5f, -0.5f },
		{ -0.5f, -0.5f,  0.5f },
		{  0.5f, -0.5f,  0.5f },
		{ -0.5f,  0.5f,  0.5f },
		{  0.5f,  0.5f,  0.5f }
	};

	for (auto&& block : m_blockSet.blocks) {
		for (auto&& offset : cornerOffsets) {
			Vector3 vtx = static_cast<Vector3>(block) + offset;
			auto it = std::ranges::find(vertices, vtx);
			if (it == vertices.end()) {
				vertices.push_back(vtx);
			}
		}
	}

	return vertices;
}

void BlockObject::SetTransparent(bool transparent)
{
	if (transparent) {
		m_pBlockMeshRenderer->SetOutlineColor(Color{ 1.0f, 0.5f, 0.0f, 0.4f });
		m_pBlockMeshRenderer->SetGroupTransparent(true);
		for (auto&& material : *m_pBlockMeshRenderer->GetMaterials()) {
			material.SetPixelShader("Assets/Shader/GroupTransparent_PS.cso");
			material.SetBlendState(BlendState::ALPHA);
			material.SetDepthStencilState(DepthStencilState::READ_ONLY);
			struct TransparentParam
			{
				float transparency;
				Vector3 pad;
			} transparentParam = { 0.4f, Vector3::zero };
			material.SetParameter(&transparentParam, sizeof(TransparentParam));
		}
	}
	else {
		m_pBlockMeshRenderer->SetOutlineColor(Color{ 1.0f, 0.5f, 0.0f, 1.0f });
		m_pBlockMeshRenderer->SetTransparent(false);
		for (auto&& material : *m_pBlockMeshRenderer->GetMaterials()) {
			material.SetPixelShader("Assets/Shader/Default_PS.cso");
			material.SetBlendState(BlendState::DEFAULT);
			material.SetDepthStencilState(DepthStencilState::DEFAULT);
		}
	}
}
