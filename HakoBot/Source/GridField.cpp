// GridField.cpp

#include "GridField.h"

#include "BlockObject.h"
#include "VecUtil.h"
#include "InputManager.h"
#include "GameState.h"


GridField::GridField()
	: m_removeCursorBlockId(0u)
{

}

void GridField::Awake()
{
	auto size = GetSize();
	Vector3 sizeFloat = static_cast<Vector3>(size);
	Vector3 sizeHalf = sizeFloat * 0.5f;

	{
		auto obj = SceneManager::GetActiveScene()->CreateGameObject();
		auto transform = obj->GetTransform();
		transform->SetParent(GetTransform());
		transform->SetEulerAngle(0.0f, 90.0f, 90.0f);
		m_pShapeScreen[0] = obj->AddComponent<ShapeScreen>();
	}

	{
		auto obj = SceneManager::GetActiveScene()->CreateGameObject();
		auto transform = obj->GetTransform();
		transform->SetParent(GetTransform());
		transform->SetEulerAngle(0.0f, 90.0f, 90.0f);
		m_pShapeScreen[1] = obj->AddComponent<ShapeScreen>();
	}

	{
		auto obj = SceneManager::GetActiveScene()->CreateGameObject();
		auto transform = obj->GetTransform();
		transform->SetParent(GetTransform());
		transform->SetEulerAngle(90.0f, 0.0f, 0.0f);
		m_pShapeScreen[2] = obj->AddComponent<ShapeScreen>();
		m_pShapeScreen[2]->SetTransparent(false);
	}

	{
		auto obj = SceneManager::GetActiveScene()->CreateGameObject();
		auto transform = obj->GetTransform();
		transform->SetParent(GetTransform());
		transform->SetEulerAngle(0.0f, 0.0f, 0.0f);
		m_pShapeScreen[3] = obj->AddComponent<ShapeScreen>();
	}
	{
		auto obj = SceneManager::GetActiveScene()->CreateGameObject();
		auto transform = obj->GetTransform();
		transform->SetParent(GetTransform());
		transform->SetEulerAngle(0.0f, 0.0f, 0.0f);
		m_pShapeScreen[4] = obj->AddComponent<ShapeScreen>();
	}
}

void GridField::Start()
{
	m_pPlaceCursor = SceneManager::GetActiveScene()->CreateGameObject();
	m_pPlaceCursorComponent = m_pPlaceCursor->AddComponent<PlaceCursor>();
}

void GridField::Update()
{
	if (GameState::GetInstance()->IsClearEnter())
	{
		if (!m_floatEffect.empty())
			ClearFloatEffect();
	}
}

void GridField::OnDestroy()
{
	for (auto&& block : m_pPlacedBlocks) {
		if (block) {
			block->Destroy();
		}
	}

	if (m_pPlaceCursor) {
		m_pPlaceCursor->Destroy();
	}

	for (int i = 0; i < 5; ++i) {
		if (m_pShapeScreen[i]) {
			m_pShapeScreen[i]->GetGameObject()->Destroy();
		}
	}
}

void GridField::SetSize(Vec3Int size)
{
	m_gridData = GridBlockData{ size };

	Vector3 sizeFloat = static_cast<Vector3>(size);
	Vector3 sizeHalf = sizeFloat * 0.5f;

	const float space = 3.0f;

	m_pShapeScreen[0]->GetTransform()->SetPosition(-sizeHalf.x - space, sizeHalf.y + 0.6f, 0.0f);
	m_pShapeScreen[1]->GetTransform()->SetPosition(sizeHalf.x + space, sizeHalf.y + 0.6f, 0.0f);
	m_pShapeScreen[2]->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
	m_pShapeScreen[3]->GetTransform()->SetPosition(0.0f, sizeHalf.y + 0.6f, sizeHalf.z + space);
	m_pShapeScreen[4]->GetTransform()->SetPosition(0.0f, sizeHalf.y + 0.6f, -(sizeHalf.z + space));

	ClearFloatEffect();
	m_floatEffect.resize(size.y - 1);
	for (int y = 0; y < size.y - 1; ++y)
	{
		Vector3 effectPos;
		effectPos.y = (float)y + 0.8f;
		m_floatEffect[y].resize(size.z);

		for (int z = 0; z < size.z; ++z)
		{
			effectPos.z = (float)z - (size.z - 1) * 0.5f;
			m_floatEffect[y][z].resize(size.x);

			for (int x = 0; x < size.x; ++x)
			{
				effectPos.x = (float)x - (size.x - 1) * 0.5f;

				auto obj = SceneManager::GetActiveScene()->CreateGameObject();
				auto trans = obj->GetTransform();
				trans->SetParent(GetTransform());
				trans->SetPosition(effectPos, Space::LOCAL);
				auto effect = obj->AddComponent<EffectRenderer>();
				effect->Load("Assets/Effect/Float/Float.efkefc");
				effect->Play();
				m_floatEffect[y][z][x] = effect;
			}
		}
	}
	UpdateFloatEffect();
}

bool GridField::IsOverlap(const BlockSetData& blockSet, const Vector3& position, const Quaternion& rotation)
{
	Quaternion snappedRot = SnapRotationToNearest90(rotation);
	Vec3 start = static_cast<Vec3>(GetGridOrigin());
	Vec3 end = start + static_cast<Vec3>(m_gridData.GetSize());

	// 土台の周りにも置けなくする
	start.x -= 1.0f;
	start.z -= 1.0f;
	end.x += 1.0f;
	end.z += 1.0f;

	for (auto&& blockPos : blockSet.blocks) {
		Vec3 pos = static_cast<Vec3>(position) + snappedRot * blockPos;
		if (IsOverlapBlockImpl(pos, start, end)) {
			return true;
		}
	}

	return false;
}

void GridField::SetPlaceCursor(const BlockSetData& blockSet, const Vector3& position, const Quaternion& rotation, const std::string& modelPath)
{
	Quaternion snappedRot = SnapRotationToNearest90(rotation);

	if (!IsInside(blockSet, position, snappedRot)) {
		m_pPlaceCursorComponent->SetBlockSet(BlockSetData{});
		m_pPlaceCursorComponent->SetModelPath("");
		return;
	}

	Vec3Int coord = CalcGridCoord(position);
	Vector3 origin = GetGridOrigin();

	Vector3 pos = origin + static_cast<Vector3>(coord) + Vector3{0.5f, 0.5f, 0.5f};

	m_pPlaceCursorComponent->SetBlockSet(blockSet);
	m_pPlaceCursorComponent->SetModelPath(modelPath);
	m_pPlaceCursorComponent->SetPlaceable(CanPlace(blockSet, position, snappedRot));
	m_pPlaceCursor->GetTransform()->SetPosition(pos);
	m_pPlaceCursor->GetTransform()->SetQuaternion(snappedRot);
}

void GridField::ResetPlaceCursor()
{
	m_pPlaceCursorComponent->SetBlockSet(BlockSetData{});
}

bool GridField::PlaceBlock()
{
	BlockSetData blockSet = m_pPlaceCursorComponent->GetBlockSet();
	std::string modelPath = m_pPlaceCursorComponent->GetModelPath();

	if (blockSet.blocks.empty()) {
		return false;
	}

	Vector3 pos = m_pPlaceCursor->GetTransform()->GetPosition();
	Quaternion rot = m_pPlaceCursor->GetTransform()->GetQuaternion();

	Vec3Int posGrid = CalcGridCoord(pos - Vector3{0.5f, 0.5f, 0.5f});

	auto blockId = m_gridData.PlaceBlock(blockSet, posGrid, rot, modelPath);

	if (!blockId) {
		return false;
	}

	auto obj = SceneManager::GetActiveScene()->CreateGameObject();
	auto component = obj->AddComponent<BlockObject>();
	component->SetUseCollider(true);
	component->SetBlockSet(blockSet);
	component->SetModel(modelPath);
	auto transform = obj->GetTransform();
	transform->SetPosition(pos);
	transform->SetQuaternion(rot);

	if (m_pPlacedBlocks.size() < blockId) {
		m_pPlacedBlocks.resize(blockId);
	}
	else if (m_pPlacedBlocks[blockId - 1]) {
		m_pPlacedBlocks[blockId - 1]->Destroy();
	}

	m_pPlacedBlocks[blockId - 1] = obj;

	m_pPlaceCursorComponent->SetBlockSet(BlockSetData{});
	m_pPlaceCursorComponent->SetModelPath("");

	m_pShapeScreen[0]->SetCurrentShape(m_gridData.GetShape(0));
	m_pShapeScreen[1]->SetCurrentShape(m_gridData.GetShape(0));
	m_pShapeScreen[2]->SetCurrentShape(m_gridData.GetShape(1));
	m_pShapeScreen[3]->SetCurrentShape(m_gridData.GetShape(2));
	m_pShapeScreen[4]->SetCurrentShape(m_gridData.GetShape(2));

	component->CreatePlaceEffect();
	UpdateFloatEffect();

	return true;
}

void GridField::SetRemoveCursor(const Vector3& position)
{
	if (0u < m_removeCursorBlockId && m_removeCursorBlockId <= m_pPlacedBlocks.size()) {
		if (m_pPlacedBlocks[m_removeCursorBlockId - 1]) {
			m_pPlacedBlocks[m_removeCursorBlockId - 1]->GetComponent<BlockObject>()->SetSelect(false);
		}
	}

	if (!IsInside(position)) {
		m_removeCursorBlockId = 0u;
		return;
	}

	Vec3Int gridCoord = CalcGridCoord(position);
	m_removeCursorBlockId = m_gridData.GetId(gridCoord);
	const std::vector<Vec3Int> removeOffsets = {
		{1, 0, 0},
		{-1, 0, 0},
		{0, 0, 1},
		{0, 0, -1},
	};
	for (int i = 0; m_removeCursorBlockId == 0u && i < 4; ++i) {
		m_removeCursorBlockId = m_gridData.GetId(gridCoord + removeOffsets[i]);
	}
	if (0u < m_removeCursorBlockId && m_removeCursorBlockId <= m_pPlacedBlocks.size()) {
		if (m_pPlacedBlocks[m_removeCursorBlockId - 1]) {
			m_pPlacedBlocks[m_removeCursorBlockId - 1]->GetComponent<BlockObject>()->SetSelect(true);
		}
	}
}

void GridField::ResetRemoveCursor()
{
	if (0u < m_removeCursorBlockId && m_removeCursorBlockId <= m_pPlacedBlocks.size()) {
		if (m_pPlacedBlocks[m_removeCursorBlockId - 1]) {
			m_pPlacedBlocks[m_removeCursorBlockId - 1]->GetComponent<BlockObject>()->SetSelect(false);
		}
	}
	m_removeCursorBlockId = 0u;
}

std::optional<BlockData> GridField::RemoveBlock()
{
	auto data = m_gridData.RemoveBlock(m_removeCursorBlockId);
	if (0u < m_removeCursorBlockId && m_removeCursorBlockId <= m_pPlacedBlocks.size()) {
		if (m_pPlacedBlocks[m_removeCursorBlockId - 1]) {
			m_pPlacedBlocks[m_removeCursorBlockId - 1]->Destroy();
		}
	}
	m_removeCursorBlockId = 0u;

	m_pShapeScreen[0]->SetCurrentShape(m_gridData.GetShape(0));
	m_pShapeScreen[1]->SetCurrentShape(m_gridData.GetShape(0));
	m_pShapeScreen[2]->SetCurrentShape(m_gridData.GetShape(1));
	m_pShapeScreen[3]->SetCurrentShape(m_gridData.GetShape(2));
	m_pShapeScreen[4]->SetCurrentShape(m_gridData.GetShape(2));

	UpdateFloatEffect();

	return data;
}

void GridField::SetClearShape(ShapeType shapeX, ShapeType shapeY, ShapeType shapeZ)
{
	m_clearShape[0] = shapeX;
	m_clearShape[1] = shapeY;
	m_clearShape[2] = shapeZ;

	m_pShapeScreen[0]->SetClearShape(m_clearShape[0], true);
	m_pShapeScreen[1]->SetClearShape(m_clearShape[0], false);
	m_pShapeScreen[2]->SetClearShape(m_clearShape[1], true);
	m_pShapeScreen[3]->SetClearShape(m_clearShape[2], false);
	m_pShapeScreen[4]->SetClearShape(m_clearShape[2], true);
}

bool GridField::IsClear()
{
	for (int i = 0; i < 3; ++i)
	{
		if (m_clearShape[i] != m_gridData.GetShape(i))
		{
			return false;
		}
	}

	return true;
}

bool GridField::IsInside(const Vector3& position)
{
	Vec3 start = static_cast<Vec3>(GetGridOrigin());
	Vec3 end = start + static_cast<Vec3>(m_gridData.GetSize());

	Vec3 pos = static_cast<Vec3>(position);

	for (int i = 0; i < 3; ++i) {
		if (pos[i] < start[i] || end[i] <= pos[i]) {
			return false;
		}
	}

	return true;
}

bool GridField::IsInside(const BlockSetData& blockSet, const Vector3& position, const Quaternion& rotation)
{
	Quaternion snappedRot = SnapRotationToNearest90(rotation);
	Vec3 start = static_cast<Vec3>(GetGridOrigin());
	Vec3 end = start + static_cast<Vec3>(m_gridData.GetSize());

	for (auto&& blockPos : blockSet.blocks) {
		Vec3 pos = static_cast<Vec3>(position) + snappedRot * blockPos;
		if (!IsInsideBlockImpl(pos, start, end)) {
			return false;
		}
	}

	return true;
}

bool GridField::CanPlace(const BlockSetData& blockSet, const Vector3& position, const Quaternion& rotation)
{
	Quaternion snappedRot = SnapRotationToNearest90(rotation);
	return m_gridData.CanPlace(blockSet, CalcGridCoord(position), snappedRot);
}

Vec3Int GridField::CalcGridCoord(const Vector3& position)
{
	Vector3 epsilon(0.001f, 0.001f, 0.001f);
	Vector3 coordFloat = SnapGrid(position - GetGridOrigin());
	return static_cast<Vec3Int>(coordFloat + epsilon);
}

Vector3 GridField::SnapGrid(const Vector3& position)
{
	Vector3 epsilon(0.001f, 0.001f, 0.001f);
	Vector3 tmp = position + epsilon;
	tmp.x = floor(tmp.x);
	tmp.y = floor(tmp.y);
	tmp.z = floor(tmp.z);

	return tmp;
}

Vector3 GridField::GetGridOrigin()
{
	Vec3Int size = m_gridData.GetSize();
	Vector3 tmp = static_cast<Vector3>(size);
	tmp.y = 0.0f;
	tmp *= -0.5f;

	return GetTransform()->GetPosition(Space::WORLD) + tmp;
}

bool GridField::IsOverlapBlockImpl(const Vec3& pos, const Vec3& start, const Vec3& end)
{
	for (int i = 0; i < 3; ++i) {
		if (pos[i] + 0.5f <= start[i] || end[i] <= pos[i] - 0.5f) {
			return false;
		}
	}

	return true;
}

bool GridField::IsInsideBlockImpl(const Vec3& pos, const Vec3& start, const Vec3& end)
{
	for (int i = 0; i < 3; ++i) {
		if (pos[i] < start[i] || end[i] <= pos[i]) {
			return false;
		}
	}

	return true;
}

void GridField::UpdateFloatEffect()
{
	Vec3Int gridPos;
	Vec3Int gridSize = m_gridData.GetSize();

	for (int z = 0; z < gridSize.z; ++z)
	{
		gridPos.z = z;

		for (int x = 0; x < gridSize.x; ++x)
		{
			gridPos.x = x;

			for (int y = 0; y < gridSize.y - 1; ++y)
			{
				gridPos.y = y;

				if (m_gridData.GetId(gridPos) == BLOCK_ID_EMPTY)
				{
					gridPos.y++;
					if (m_gridData.GetId(gridPos) != BLOCK_ID_EMPTY)
					{
						// ブロックが存在せず、一つ上にブロックがあれば表示
						m_floatEffect[y][z][x]->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
						continue;
					}
				}
				// それ以外は隠す
				m_floatEffect[y][z][x]->GetTransform()->SetScale(0.0f, 0.0f, 0.0f);
			}
		}
	}
}

void GridField::ClearFloatEffect()
{
	for (auto& effectArrayXZ : m_floatEffect)
	{
		for (auto& effectArrayX : effectArrayXZ)
		{
			for (auto& effect : effectArrayX)
			{
				if (effect)
					effect->GetGameObject()->Destroy();
			}
		}
	}
	m_floatEffect.clear();
}
