#pragma once

#include "GameDefine.h"
#include "PlayerShot.h"
#include "Enemy.h"
#include "Boss.h"

// �ړ����x
#define PLAYER_SPEED		(0.1f)

// �e�̔��ˌ��E��
#define PLAYERSHOT_COUNT	(40)

// �e�̔��ˊԊu
#define PLAYERSHOT_WAIT		(5)

class CPlayer{
private:
	CMeshContainer	m_Mesh;
	CVector3		m_Pos;
	float			m_RotZ;

	bool			m_bDead;

	CMeshContainer	m_ShotMesh;
	CPlayerShot		m_ShotArray[PLAYERSHOT_COUNT];
	int				m_ShotWait;
public:
	CPlayer();
	~CPlayer();
	bool Load();
	void Initialize();
	void Update();
	void CollisionEnemy(CEnemy& ene);
	void CollisionEnemyShot(CEnemyShot& shot);
	void CollisionBoss(CBoss& boss);
	void Render();
	void RenderDebug();
	void RenderDebugText();
	const CVector3 GetPosition(){ return m_Pos; }
	CSphere GetSphere(){ return CSphere(m_Pos,0.4f); }
	bool IsDead(){ return m_bDead; }
	void Release();
};