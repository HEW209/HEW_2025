// GridField.cpp

#include "GridField.h"

#include "BlockObject.h"
#include "VecUtil.h"


GridField::GridField()
	: m_removeCursorBlockId(0u)
{
}

void GridField::Start()
{
	m_pPlaceCursor = SceneManager::GetActiveScene()->CreateGameObject();
	m_pPlaceCursorComponent = m_pPlaceCursor->AddComponent<PlaceCursor>();
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
}

void GridField::SetSize(Vec3Int size)
{
	m_gridData = GridBlockData{ size };
}

bool GridField::IsOverlap(const BlockSetData& blockSet, const Vector3& position, const Quaternion& rotation)
{
	Quaternion snappedRot = SnapRotationToNearest90(rotation);
	Vec3 start = static_cast<Vec3>(GetGridOrigin());
	Vec3 end = start + static_cast<Vec3>(m_gridData.GetSize());

	for (auto&& blockPos : blockSet.blocks) {
		Vec3 pos = static_cast<Vec3>(position) + snappedRot * blockPos;
		if (IsOverlapBlockImpl(pos, start, end)) {
			return true;
		}
	}

	return false;
}

void GridField::SetPlaceCursor(const BlockSetData& blockSet, const Vector3& position, const Quaternion& rotation)
{
	Quaternion snappedRot = SnapRotationToNearest90(rotation);

	if (!IsInside(blockSet, position, snappedRot)) {
		m_pPlaceCursorComponent->SetBlockSet(BlockSetData{});
		return;
	}

	m_pPlaceCursorComponent->SetBlockSet(blockSet);
	m_pPlaceCursorComponent->SetPlaceable(CanPlace(blockSet, position, snappedRot));
	m_pPlaceCursor->GetTransform()->m_position = SnapGrid(position) + Vector3{0.5f, 0.5f, 0.5f};
	m_pPlaceCursor->GetTransform()->SetQuaternion(snappedRot);
}

void GridField::ResetPlaceCursor()
{
	m_pPlaceCursorComponent->SetBlockSet(BlockSetData{});
}

bool GridField::PlaceBlock()
{
	BlockSetData blockSet = m_pPlaceCursorComponent->GetBlockSet();

	if (blockSet.blocks.empty()) {
		return false;
	}

	Vector3 pos = m_pPlaceCursor->GetTransform()->m_position;
	Quaternion rot = m_pPlaceCursor->GetTransform()->GetQuaternion();

	Vec3Int posGrid = CalcGridCoord(pos - Vector3{0.5f, 0.5f, 0.5f});

	auto blockId = m_gridData.PlaceBlock(blockSet, posGrid, rot);

	if (!blockId) {
		return false;
	}

	auto obj = SceneManager::GetActiveScene()->CreateGameObject();
	auto component = obj->AddComponent<BlockObject>();
	component->SetBlockSet(blockSet);
	auto transform = obj->GetTransform();
	transform->m_position = pos;
	transform->SetQuaternion(rot);

	if (m_pPlacedBlocks.size() < blockId) {
		m_pPlacedBlocks.resize(blockId);
	}
	else if (m_pPlacedBlocks[blockId - 1]) {
		m_pPlacedBlocks[blockId - 1]->Destroy();
	}

	m_pPlacedBlocks[blockId - 1] = obj;

	m_pPlaceCursorComponent->SetBlockSet(BlockSetData{});

	return true;
}

void GridField::SetRemoveCursor(const Vector3& position)
{
	if (0u < m_removeCursorBlockId && m_removeCursorBlockId <= m_pPlacedBlocks.size()) {
		if (m_pPlacedBlocks[m_removeCursorBlockId - 1]) {
			m_pPlacedBlocks[m_removeCursorBlockId - 1]->GetComponent<BlockObject>()->SetSelect(false);
		}
	}
	m_removeCursorBlockId = m_gridData.GetId(CalcGridCoord(position));
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

std::optional<BlockSetAndRotationData> GridField::RemoveBlock()
{
	auto data = m_gridData.RemoveBlock(m_removeCursorBlockId);
	if (0u < m_removeCursorBlockId && m_removeCursorBlockId <= m_pPlacedBlocks.size()) {
		if (m_pPlacedBlocks[m_removeCursorBlockId - 1]) {
			m_pPlacedBlocks[m_removeCursorBlockId - 1]->Destroy();
		}
	}
	m_removeCursorBlockId = 0u;
	return data;
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

	return GetTransform()->GetWorldPosition() + tmp;
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
		if (pos[i] <= start[i] || end[i] < pos[i]) {
			return false;
		}
	}

	return true;
}
