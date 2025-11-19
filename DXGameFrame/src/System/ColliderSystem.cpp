#include <System/ColliderSystem.h>
#include <Utility/Utility.h>
#include <GameFrame/Transform.h>

constexpr int ITER_NUM = 4;                 // 反復処理の回数(処理重かったらなくす)
constexpr float PENETRATION_SLOP = 0.01f;   // 震えるのをふせぐめり込み許容値

void ColliderSystem::Check()
{
    for (int iter = 0; iter < ITER_NUM; ++iter)
    {
	// colliderのデータ全部取り出す
	std::vector<Collider::ObbData> obbData;
	obbData.clear();
	obbData.reserve(m_colliders.size());

	std::vector<Collider*>::iterator colliderIt = m_colliders.begin();
	for (; colliderIt != m_colliders.end(); ++colliderIt)
	{
		Collider::ObbData data;
		Vector3 vector;
		Quaternion colliderQuaternion = (*colliderIt)->GetQuaternion();
		Quaternion objectQuaternion = (*colliderIt)->GetTransform()->GetQuaternion();

		// ローカルのx,y,z軸をワールドベクトルにする
		vector = { 1.0f,0.0f,0.0f };
	    	data.axis.x = (objectQuaternion * colliderQuaternion) * vector;
		vector = { 0.0f,1.0f,0.0f };
            data.axis.y = (objectQuaternion * colliderQuaternion) * vector;
		vector = { 0.0f,0.0f,1.0f };
            data.axis.z = (objectQuaternion * colliderQuaternion) * vector;

		// ワールド座標に変える
            data.pos = (*colliderIt)->GetTransform()->GetPosition() + (objectQuaternion * colliderQuaternion)* (*colliderIt)->GetPosition();

		// スケールも移す
		data.scale = (*colliderIt)->GetScale();

		obbData.push_back(data);
	}

        Vector3 mtv;                    // 移動させるためのベクトル

    // 全ての組み合わせをチェックする
	for (int i = 0; i < obbData.size(); ++i)
	{
		for (int j = i + 1; j < obbData.size(); ++j)
		{
			if (CheckCollisionOBB(obbData[i], obbData[j],&mtv))
			{
	    		if (m_colliders[i]->OnCollisionEnter && iter == 0)
				{
					m_colliders[i]->OnCollisionEnter(m_colliders[j]->GetGameObject());
				}
	    		if (m_colliders[j]->OnCollisionEnter && iter == 0)
				{
					m_colliders[j]->OnCollisionEnter(m_colliders[i]->GetGameObject());
				}

                // 衝突したら動かす
                    if (m_colliders[i]->IsStatic != m_colliders[j]->IsStatic)
                    {
                        int idx = (m_colliders[i]->IsStatic) ? j : i;   // 動かす方を選ぶ

                        float dir = (idx == i) ? 1.0f : -1.0f;          // 向き調整

                        mtv *= dir;

                        m_colliders[idx]->GetTransform()->Translate(mtv);
                        obbData[idx].pos += mtv;
                    }
	    		}
			}
		}
	}
}

void ColliderSystem::Register(Collider* pCollider)
{
	m_colliders.push_back(pCollider);
}

void ColliderSystem::Unregister(Collider* pCollider)
{
	std::erase(m_colliders, pCollider);
}

double ColliderSystem::GetProjectionRadius(const Vector3 scale, const Vector3 axis, const Collider::Axis3 obbAxes)
{
	// 各ローカル軸と分離軸の内積の絶対値 を取り、
	// それをOBBのサイズ（halfExtents）と掛け合わせる
	return
		std::abs(Dot(axis, obbAxes.x)) * scale.x * 0.5f +
		std::abs(Dot(axis, obbAxes.y)) * scale.y * 0.5f +
		std::abs(Dot(axis, obbAxes.z)) * scale.z * 0.5f;
}

bool ColliderSystem::CheckCollisionOBB(Collider::ObbData data, Collider::ObbData otherData,Vector3* pMtv)
{
    // 3. 中心間のベクトル
    Vector3 T = otherData.pos - data.pos;

    // 軸リスト作る
    Vector3 axisList[15] = {
        data.axis.x,
        data.axis.y,
        data.axis.z,
        otherData.axis.x,
        otherData.axis.y,
        otherData.axis.z
    };

    float minOverlap = FLT_MAX; // 最小の重なり量
    Vector3 mtvAxis;            // 最小の重なりが発生した軸

    // 4. すべての分離軸候補でテストを実行
    // とりあえず6軸でチェック
    for (int i = 0; i < 6; ++i)
    {
        Vector3 L = axisList[i];

        // ゼロベクトルに近い軸はテストをスキップ (ほぼ平行な軸の外積)
        if (L.Magnitude() * L.Magnitude() < FLT_EPSILON) {
            continue;
        }
        L = L.Normalized(); // 軸を正規化

        // (1) 2つのOBBの中心間の距離を、軸Lに射影
        double distance = std::abs(Dot(T, L));

        // (2) 2つのOBBの射影半径の合計
        double r = GetProjectionRadius(data.scale, L, data.axis);
        double otherR = GetProjectionRadius(otherData.scale, L, otherData.axis);

        // 判定の重なり量
        double overlap = r + otherR - distance;

        // (3) 判定
        if (overlap <= 0) {
            // 分離軸が見つかった！ (隙間がある)
            // この時点で衝突していないことが確定
            return false;
        }

        // 最も小さい重なり量とその時の軸を記録する
        if (minOverlap > overlap)
        {
            minOverlap = overlap;
            mtvAxis = L;
        }
    }

    int idx = 6;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            axisList[idx] = Cross(axisList[i], axisList[j + 3]);
            idx++;
        }
    }

    for (int i = 6; i < 15; ++i)
    {
        Vector3 L = axisList[i];

        // ゼロベクトルに近い軸はテストをスキップ (ほぼ平行な軸の外積)
        if (L.Magnitude() * L.Magnitude() < FLT_EPSILON) {
            continue;
        }
        L = L.Normalized(); // 軸を正規化

        // (1) 2つのOBBの中心間の距離を、軸Lに射影
        double distance = std::abs(Dot(T, L));

        // (2) 2つのOBBの射影半径の合計
        double r = GetProjectionRadius(data.scale, L, data.axis);
        double otherR = GetProjectionRadius(otherData.scale, L, otherData.axis);

        // 判定の重なり量
        double overlap = r + otherR - distance;

        // (3) 判定
        if (overlap <= 0) {
            // 分離軸が見つかった！ (隙間がある)
            // この時点で衝突していないことが確定
            return false;
        }

        // 最も小さい重なり量とその時の軸を記録する
        if (minOverlap > overlap)
        {
            minOverlap = overlap;
            mtvAxis = L;
        }
    }

    // めり込み許容値を適用
    minOverlap = std::max(0.0f, minOverlap - PENETRATION_SLOP);

    // (1) MTVの計算
    *pMtv = mtvAxis * minOverlap;

    // (2) MTVの方向を補正する（重要！）
    // MTVは「AをBから押し出す」方向でなければならない。
    // AからBへのベクトル(T)と、mtvAxis の向きが逆（内積が負）なら、MTVを反転させる
    if (Dot(T, mtvAxis) >= 0) {
        *pMtv = *pMtv * -1.0;
    }
    // 15軸すべてをテストしたが、分離軸は一つも見つからなかった
    // したがって、衝突している
    return true;
}

float ColliderSystem::Dot(Vector3 v, Vector3 other)
{
    return v.x * other.x + v.y * other.y + v.z * other.z;
}

Vector3 ColliderSystem::Cross(Vector3 v, Vector3 other)
{
    return { v.y * other.z - v.z * other.y, v.z * other.x - v.x * other.z,v.x * other.y - v.y * other.x };
}
