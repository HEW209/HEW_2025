// Collider.cpp

#include "Collider.h"

Collider::Collider() :Component::Component(),m_transform(nullptr)
{
}

void Collider::Awake()
{
	// colliderをまとめる配列に追加
	s_colliders.push_back(this);
}

void Collider::OnDestroy()
{
	// colliderをまとめる配列から消去
	std::erase(s_colliders, this);
}

void Collider::CheckCollision()
{
	// 各軸のデータ作る
	s_axis.clear();
	s_axis.reserve(s_colliders.size());

	std::vector<Collider*>::iterator colliderIt = s_colliders.begin();
	for (; colliderIt != s_colliders.end(); ++colliderIt)
	{
		
	}

}

