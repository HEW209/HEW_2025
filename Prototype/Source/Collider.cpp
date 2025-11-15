// Collider.cpp

#include "Collider.h"

std::vector<Collider*> s_colliders;

Collider::Collider() :Component::Component(),onCollision(nullptr)
{
	// colliderをまとめる配列に追加
	s_colliders.push_back(this);

    m_scale = { 1.0f,1.0f,1.0f };
}

Collider::~Collider()
{
	// colliderをまとめる配列から消去
	std::erase(s_colliders, this);
}

void CheckCollision()
{
	// colliderのデータ全部取り出す
	std::vector<Collider::ObbData> s_obbData;
    s_obbData.clear();
    s_obbData.reserve(s_colliders.size());

	std::vector<Collider*>::iterator colliderIt = s_colliders.begin();
	for (; colliderIt != s_colliders.end(); ++colliderIt)
	{
		Collider::ObbData data;
		Vector3 vector;
		Quaternion colliderQuaternion = (*colliderIt)->GetQuaternion();
		Quaternion objectQuaternion = (*colliderIt)->GetTransform()->GetQuaternion();

		// ローカルのx,y,z軸をワールドベクトルにする
		vector = { 1.0f,0.0f,0.0f };
		data.axis.x = colliderQuaternion * objectQuaternion * vector;
		vector = { 0.0f,1.0f,0.0f };
		data.axis.y = colliderQuaternion * objectQuaternion * vector;
		vector = { 0.0f,0.0f,1.0f };
		data.axis.z = colliderQuaternion * objectQuaternion * vector;

        // ワールド座標に変える
        data.pos = (*colliderIt)->GetTransform()->m_position + (*colliderIt)->GetPosition();

        // スケールも移す
        data.scale = (*colliderIt)->GetScale();

        s_obbData.push_back(data);
	}

	for (int i = 0; i < s_obbData.size(); ++i)
	{
		for (int j = i + 1 ; j < s_obbData.size(); ++j)
		{
            if (CheckCollisionOBB(s_obbData[i], s_obbData[j]))
            {
                if (s_colliders[i]->onCollision)
                {
                    s_colliders[i]->onCollision(s_colliders[j]->GetGameObject());
                }
                if (s_colliders[j]->onCollision)
                {
                    s_colliders[j]->onCollision(s_colliders[i]->GetGameObject());
                }
            }
		}
	}
}

double GetProjectionRadius(const Vector3 scale, const Vector3 axis, const Collider::Axis3 obbAxes) {
	// 各ローカル軸と分離軸の内積の絶対値 を取り、
	// それをOBBのサイズ（halfExtents）と掛け合わせる
	return
		std::abs(Dot(axis, obbAxes.x)) * scale.x * 0.5f +
		std::abs(Dot(axis, obbAxes.y)) * scale.y * 0.5f +
		std::abs(Dot(axis, obbAxes.z)) * scale.z * 0.5f;
}

float Dot(Vector3 v, Vector3 other)
{
	return v.x * other.x + v.y * other.y + v.z * other.z;
}

bool CheckCollisionOBB(Collider::ObbData data, Collider::ObbData otherData)
{
    // 3. 中心間のベクトル
    Vector3 T = data.pos - otherData.pos;

    // 軸リスト作る
    Vector3 axisList[15] = {
        data.axis.x,
        data.axis.y,
        data.axis.z,
        otherData.axis.x,
        otherData.axis.y,
        otherData.axis.z
    };

    // 4. すべての分離軸候補でテストを実行
    // とりあえず6軸でチェック
    for (int i = 0; i < 6; ++i) 
    {
        Vector3 L = axisList[i];

        // ゼロベクトルに近い軸はテストをスキップ (ほぼ平行な軸の外積)
        if (L.Magnitude() < FLT_EPSILON) {
            continue;
        }
        L = L.Normalized(); // 軸を正規化

        // (1) 2つのOBBの中心間の距離を、軸Lに射影
        double distance = std::abs(Dot(T, L));

        // (2) 2つのOBBの射影半径の合計
        double r = GetProjectionRadius(data.scale, L, data.axis);
        double otherR = GetProjectionRadius(otherData.scale, L, otherData.axis);
        double totalRadius = r + otherR;

        // (3) 判定
        if (distance > totalRadius) {
            // 分離軸が見つかった！ (隙間がある)
            // この時点で衝突していないことが確定
            return false;
        }
    }

    int idx = 6;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            axisList[idx] = Cross(axisList[i], axisList[j + 3]);
        }
    }

    for (int i = 6; i < 15; ++i)
    {
        Vector3 L = axisList[i];

        // ゼロベクトルに近い軸はテストをスキップ (ほぼ平行な軸の外積)
        if (L.Magnitude() < FLT_EPSILON) {
            continue;
        }
        L = L.Normalized(); // 軸を正規化

        // (1) 2つのOBBの中心間の距離を、軸Lに射影
        double distance = std::abs(Dot(T, L));

        // (2) 2つのOBBの射影半径の合計
        double r = GetProjectionRadius(data.scale, L, data.axis);
        double otherR = GetProjectionRadius(otherData.scale, L, otherData.axis);
        double totalRadius = r + otherR;

        // (3) 判定
        if (distance > totalRadius) {
            // 分離軸が見つかった！ (隙間がある)
            // この時点で衝突していないことが確定
            return false;
        }
    }

    // 15軸すべてをテストしたが、分離軸は一つも見つからなかった
    // したがって、衝突している
    return true;
}

Vector3 Cross(Vector3 v, Vector3 other)
{
    return {v.y * other.z - v.z * other.y, v.z * other.x - v.x * other.z,v.x * other.y - v.y * other.x };
}

