// Collider.cpp

#include "Collider.h"

std::vector<Collider*> s_colliders;

Collider::Collider() :Component::Component()
{
	// colliderをまとめる配列に追加
	s_colliders.push_back(this);
}

Collider::~Collider()
{
	// colliderをまとめる配列から消去
	std::erase(s_colliders, this);
}

void CheckCollision()
{
	// 各軸のデータ作る
	std::vector<Collider::Axis3> s_axis;
	s_axis.clear();
	s_axis.reserve(s_colliders.size());

	std::vector<Collider*>::iterator colliderIt = s_colliders.begin();
	for (; colliderIt != s_colliders.end(); ++colliderIt)
	{
		Collider::Axis3 data;
		Vector3 vector;
		Quaternion colliderQuaternion = (*colliderIt)->GetQuaternion();
		Quaternion objectQuaternion = (*colliderIt)->GetTransform()->GetQuaternion();

		// ローカルのx,y,z軸をワールドベクトルにする
		vector = { 1.0f,0.0f,0.0f };
		data.x = colliderQuaternion * objectQuaternion * vector;
		vector = { 0.0f,1.0f,0.0f };
		data.y = colliderQuaternion * objectQuaternion * vector;
		vector = { 0.0f,0.0f,1.0f };
		data.z = colliderQuaternion * objectQuaternion * vector;

		s_axis.push_back(data);
	}

	std::vector<Collider::Axis3>::iterator axisIt = s_axis.begin();
	for (int i = 0; axisIt != s_axis.end(); ++axisIt,++i)
	{
		Collider::Axis3 axis3 = *axisIt;
		++axisIt;
		for (int j = i + 1 ; axisIt != s_axis.end(); ++axisIt, ++j)
		{
			Collider::Axis3 otherAxis3 = *axisIt;
		}
		axisIt = s_axis.begin() + i;
	}
}

