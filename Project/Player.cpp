#include "Player.h"

/**
 * �R���X�g���N�^
 */
CPlayer::CPlayer() :
	m_Mesh(),
	m_Pos(0.0f, 0.0f, 0.0f),
	m_RotZ(0.0f),
	m_bDead(false),
	m_ShotMesh(),
	m_ShotArray(),
	m_Shotwait() {

}

/**
 * �f�X�g���N�^
 */
CPlayer::~CPlayer(){
}

/**
 * �ǂݍ���
 */
bool CPlayer::Load(void){
	// ���b�V���̓ǂݍ���
	if (!m_Mesh.Load("Player.mom"))
	{
		return false;
	}
	if (!m_ShotMesh.Load("pshot.mom"))
	{
		return false;
	}
	for (int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].SetMesh(&m_ShotMesh);
	}
	return true;
}

/**
 * ������
 */
void CPlayer::Initialize(void){
	m_Pos = Vector3(0.0f, 0.0f, -FIELD_HALF_Z + 2.0f);
	m_RotZ = 0;
	m_bDead = false;
	for (int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].Initialize();
	}
}

/**
 * �X�V
 */
void CPlayer::Update(void){
	if (m_bDead)
	{
		return;
	}
	float Roll = 0;
	if (g_pInput->IsKeyHold(MOFKEY_LEFT))
	{
		m_Pos.x = max(m_Pos.x - PLAYER_SPEED, -FIELD_HALF_X);
		Roll -= MOF_MATH_PI;
	}
	if (g_pInput->IsKeyHold(MOFKEY_RIGHT))
	{
		m_Pos.x = min(m_Pos.x + PLAYER_SPEED, FIELD_HALF_X);
		Roll += MOF_MATH_PI;
	}
	if (g_pInput->IsKeyHold(MOFKEY_UP))
	{
		m_Pos.z = min(m_Pos.z + PLAYER_SPEED, FIELD_HALF_Z);
	}
	if (g_pInput->IsKeyHold(MOFKEY_DOWN))
	{
		m_Pos.z = max(m_Pos.z - PLAYER_SPEED, -FIELD_HALF_Z);
	}
	float RotSpeed = MOF_ToRadian(10);
	if (Roll == 0)
	{
		RotSpeed = min(abs(m_RotZ) * 0.1f, RotSpeed);
	}
	if (abs(m_RotZ) <= RotSpeed || signbit(m_RotZ) != signbit(Roll))
	{
		m_RotZ += Roll;
	}
	m_RotZ -= copysignf(min(RotSpeed, abs(m_RotZ)),m_RotZ);

	if (m_Shotwait <= 0)
	{
		if (g_pInput->IsKeyHold(MOFKEY_SPACE))
		{
			for (int cnt = 0; cnt < 2; cnt++)
			{
				for (int i = 0; i < PLAYERSHOT_COUNT; i++)
				{
					if (m_ShotArray[i].GetShow()) { continue; }
					CVector3 ShotPos(0.4f * (cnt * 2 - 1), 0, 0);
					ShotPos.RotationZ(m_RotZ);
					ShotPos += m_Pos;
					m_Shotwait = PLAYERSHOT_WAIT;
					m_ShotArray[i].Fire(ShotPos);
					break;
				}
			}
		}
	}
	else
	{
		m_Shotwait--;
	}
	for (int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].Update();
	}
}

/**
 * �`��
 */
void CPlayer::Render(void){
	if (m_bDead)
	{
		return;
	}
	CMatrix44 matWorld;
	matWorld.RotationZ(m_RotZ);
	matWorld.SetTranslation(m_Pos);
	m_Mesh.Render(matWorld);
	for (int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].Render();
	}
}

/**
 * �f�o�b�O�����`��
 */
void CPlayer::RenderDebugText(void){
	// �ʒu�̕`��
	CGraphicsUtilities::RenderString(10,40,MOF_XRGB(0,0,0),
			"�v���C���[�ʒu X : %.1f , Y : %.1f , Z : %.1f",m_Pos.x,m_Pos.y,m_Pos.z);
}

/**
 * ���
 */
void CPlayer::Release(void){
	m_Mesh.Release();
	m_ShotMesh.Release();
}

void CPlayer::RenderDebug(void) {
	CGraphicsUtilities::RenderSphere(GetSphere(), Vector4(0, 1, 0, 0.3f));
	for (int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].RenderDebug();
	}
}

void CPlayer::CollisionEnemy(CEnemy& ene) {
	if (!ene.GetShow())
	{
		return;
	}
	CSphere ps = GetSphere();
	CSphere es = ene.GetSphere();
	if (ps.CollisionSphere(es))
	{
		m_bDead = true;
	}
	for (int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		if (!m_ShotArray[i].GetShow())
		{
			continue;
		}
		CSphere ss = m_ShotArray[i].GetSphere();
		if (ss.CollisionSphere(es))
		{
			ene.Damage(1);
			m_ShotArray[i].SetShow(false);
			break;
		}
	}
}

void CPlayer::CollisionEnemyShot(CEnemyShot& shot) {
	CSphere ps = GetSphere();
	if (!shot.GetShow())
	{
		return;
	}
	CSphere ss = shot.GetSphere();
	if (ss.CollisionSphere(ps))
	{
		m_bDead = true;
		shot.SetShow(false);
	}
}