#pragma once

#include "GameDefine.h"
#include "Enemy.h"

// スクロール速度
#define		SCROLL_SPEED	(1.0f)
typedef struct tag_ENEMYSTRAT {
	int						Count;
	float*					PosX;
	float*					Scroll;
}ENEMYSTRAT;

class CStage{
private:
	CSprite3D			m_BG;
	float				m_Scroll;
	ENEMYSTRAT*			m_pEnemyStart;
	CMeshContainer		m_EnemyMesh;
	int					m_EnemyNo;
public:
	CStage();
	~CStage();
	bool Load();
	void Initialize(ENEMYSTRAT* pSt);
	void Update(CEnemy* ene,int ecnt);
	void Render();
	void RenderDebugText();
	void Release();
};