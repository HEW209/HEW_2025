#include "TitlePlayerMove.h"

TitlePlayerMove::TitlePlayerMove() :
	m_pPlayerTransform(nullptr),
	m_bComeBack(true),//最初は帰ってくるためtureを入れておく
	m_fPlayerMoveSpeed(0.05f)
{


}


void TitlePlayerMove::Update()
{
	//プレイヤーが戻ってきて入れば
	if (m_bComeBack) {
		//奥に行く処理
		auto transform = m_pPlayerTransform->GetPosition();
		m_pPlayerTransform->SetPosition(transform.x + m_fPlayerMoveSpeed, transform.y, transform.z );
		//回転処理
		auto rota = m_pPlayerTransform->GetEulerAngle();
		m_pPlayerTransform->SetEulerAngle(rota.x, -90.0f, rota.z);

		if (m_pPlayerTransform->GetPosition().x >= 9.0f)
		{
			//移動位置を戻す
			transform = m_pPlayerTransform->GetPosition();
			m_pPlayerTransform->SetPosition(9.0f, transform.y, transform.z);


			m_bComeBack = false;

		}

	}
	else {
		//奥に進処理
		//手前にもどってくる処理
		auto transform = m_pPlayerTransform->GetPosition();
		m_pPlayerTransform->SetPosition(transform.x - m_fPlayerMoveSpeed, transform.y, transform.z);

		//回転処理
		auto rota = m_pPlayerTransform->GetEulerAngle();
		m_pPlayerTransform->SetEulerAngle(rota.x, 90.0f, rota.z);

		if (m_pPlayerTransform->GetPosition().x <= -7.0f)
		{
			//移動位置を戻す
			transform = m_pPlayerTransform->GetPosition();
			m_pPlayerTransform->SetPosition(-7.0f, transform.y, transform.z);
			//奥に行く
			m_bComeBack = true;

		}
	}
}
