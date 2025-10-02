#include "stdAFX.h"

#ifdef __SERVER
	#include "NET_Prototype.h"
#else
	#include "../Network/NET_Prototype.h"
	#include "../CObjUSER.h"
#endif

#include "Calculation.h"

#ifdef	__SERVER
#include "ZoneLIST.H"
#else
short Get_WorldDROP_M ()
{
	return 500;
}
short Get_WorldDROP ()
{
	return 800;
}
short Get_WorldEXP ()
{
	return 100;
}
short Get_WorldREWARD ()
{
	return 500;
}
#endif

extern bool IsTAIWAN ();
extern bool IsIROSE (); // 타이완 , EN 모두 참.

//-------------------------------------------------------------------------------------------------
__int64 CCal::Get_NeedRawEXP (int iLevel) // JDOEBOY: Updated to __int64 for EXP safety
{
	// 필요 경험치
	if ( iLevel >= MAX_LEVEL ) {
		// No EXP required at or above max level. JDOEBOY
		return 0;
	}

	if ( IsTAIWAN() ) {
		// [레벨 15이하일 경우]   필요 경험치 = { (LV + 3) * (LV + 5 ) * (LV + 10) * 0.7 }
		if ( iLevel <= 15 )
			return (int)( (iLevel+3)*(iLevel+5)*(iLevel+10)*0.7 );

		// [레벨 50이하일 경우]   필요 경험치 = { (LV - 5) * (LV + 2 ) * (LV + 2) * 2.2 }
		if ( iLevel <= 50 ) 
			return (int)( (iLevel-5)*(iLevel+2)*(iLevel+2)*2.2 );

		// [레벨 100이하일 경우]  필요 경험치 = { (LV - 5) * ( LV +2 ) * (LV -38 ) * 9 }
		if ( iLevel <= 100 ) 
				// JDOEBOY: Unified chunked EXP math for levels 1-255. Taiwan-specific block moved to reference section below. Only unified math is active.
				if (iLevel > MAX_LEVEL)
					iLevel = MAX_LEVEL;

				// [Level 1–15] EXP = (LV + 3) * (LV + 5) * (LV + 10) * 0.7
				if (iLevel <= 15) {
					return (__int64)(((iLevel + 3) * (iLevel + 5) * (iLevel + 10) * 0.7f));
				}

				// [Level 16–60] EXP = (LV - 5) * (LV + 2) * (LV + 2) * 2.2
				if (iLevel <= 60) {
					return (__int64)(((iLevel - 5) * (iLevel + 2) * (iLevel + 2) * 2.2f));
				}

				// [Level 61–113] EXP = (LV - 11) * (LV) * (LV + 4) * 2.5
				if (iLevel <= 113) {
					return (__int64)(((iLevel - 11) * (iLevel) * (iLevel + 4) * 2.5f));
				}

				// [Level 114–150] EXP = (LV - 31) * (LV - 20) * (LV + 4) * 3.8
				if (iLevel <= 150) {
					return (__int64)(((iLevel - 31) * (iLevel - 20) * (iLevel + 4) * 3.8f));
				}

				// [Level 151–189] EXP = (LV - 67) * (LV - 20) * (LV - 10) * 6
				if (iLevel <= 189) {
					return (__int64)(((iLevel - 67) * (iLevel - 20) * (iLevel - 10) * 6.f));
				}

				// [Level 190–210] EXP = (LV - 100) * (LV - 50) * (LV - 10) * 8
				if (iLevel <= 210) {
					return (__int64)(((iLevel - 100) * (iLevel - 50) * (iLevel - 10) * 8.f));
				}

				// [Level 211–230] EXP = (LV - 120) * (LV - 70) * (LV - 20) * 10
				if (iLevel <= 230) {
					return (__int64)(((iLevel - 120) * (iLevel - 70) * (iLevel - 20) * 10.f));
				}

				// [Level 231–255] EXP = (LV - 150) * (LV - 100) * (LV - 50) * (LV - 10) * 12
				if (iLevel <= 255) {
					return (__int64)(((iLevel - 150) * (iLevel - 100) * (iLevel - 50) * (iLevel - 10) * 12.f));
				}

				// Should never reach here, but fallback to max chunk
				return (__int64)(((iLevel - 150) * (iLevel - 100) * (iLevel - 50) * (iLevel - 10) * 12.f));

/*
// Original formulas for reference (JDOEBOY):
if ( IsTAIWAN() ) {
	if ( iLevel <= 15 )
		return (int)( (iLevel+3)*(iLevel+5)*(iLevel+10)*0.7 );
	if ( iLevel <= 50 ) 
		return (int)( (iLevel-5)*(iLevel+2)*(iLevel+2)*2.2 );
	if ( iLevel <= 100 ) 
		return (int)( (iLevel-5)*(iLevel+2)*(iLevel-38)*9 );
	if ( iLevel <= 139 ) 
		return (int)( (iLevel+27)*(iLevel+34)*(iLevel+220) );
	return (int)( (iLevel-15)*(iLevel+7)*(iLevel-126) * 41 );
}
if ( iLevel <= 60 ) {
	if ( iLevel <= 15 ) {
		return (int)( ( (iLevel+3) * (iLevel+5) * (iLevel+10)*0.7f ) );
	}
	return (int)( ( (iLevel-5) * (iLevel+2) * (iLevel+2)*2.2f ) );
}
if ( iLevel <= 113 ) {
	return (int)( ( (iLevel-11) * (iLevel) * (iLevel+4)*2.5f ) );
}
if ( iLevel <= 150 ) {
	return (int)( ( (iLevel-31) * (iLevel-20) * (iLevel+4)*3.8f ) );
}
if ( iLevel <= 189 ) {
	return (int)( ( (iLevel-67) * (iLevel-20) * (iLevel-10)*6.f ) );
}
return (int)( (iLevel-90) * (iLevel-120) * (iLevel-60) * (iLevel-170) * (iLevel-188) );
*/
	int iDropTBL;
	if ( NPC_DROP_ITEM( pMobCHAR->Get_CharNO() ) - ( 1+RANDOM(100) ) >= 0 ) {
		iDropTBL = NPC_DROP_TYPE( pMobCHAR->Get_CharNO() );
	} else {
		iDropTBL = iZoneNO;
	}
	
	int iDropTblIDX = ( iDrop_VAR > 30 ) ? RANDOM(30) : RANDOM(iDrop_VAR);
	int iDropITEM   = DROPITEM_ITEMNO( iDropTBL, iDropTblIDX );
	if ( iDropITEM <= 1000 ) {
		if ( iDropITEM >= 1 && iDropITEM <= 4 ) {
			// 다시 계산
			iDropTblIDX = 26 + (iDropITEM * 5) + RANDOM(5);
			if ( iDropTblIDX >= g_TblDropITEM.m_nColCnt ) {
				// 테이블 컬럼 갯수 초과...
				return false;
			}
			iDropITEM   = DROPITEM_ITEMNO( iDropTBL, iDropTblIDX );
			if ( iDropITEM <= 1000 ) {
				// 없다 !
				return false;
			}
		} else {
			// 없다 !
			return false;
		}
	}

	::ZeroMemory ( &sITEM, sizeof(sITEM) );
	sITEM.m_cType   = (BYTE) ( iDropITEM / 1000 );
	sITEM.m_nItemNo = iDropITEM % 1000;

	int iTEMP;
	if ( sITEM.GetTYPE() >= ITEM_TYPE_ETC && sITEM.GetTYPE() != ITEM_TYPE_RIDE_PART ) { // ITEM_TYPE_RIDE_PART은 제외해야 함
		// 기타 아이템일경우 갯수 계산, 그외는 무조건 1
		// DROP_ITEM_Q = 1 + { (MOP_LV+10)/8 + RAN(1~100)/4 } * 1 / { DROP_VAR + 4 } 
		sITEM.m_uiQuantity = 1 + ( (pMobCHAR->Get_LEVEL()+10)/9 + ( 1+RANDOM(20) ) + iDropRate ) / ( iDrop_VAR+4 );

		if ( sITEM.m_uiQuantity > 10 )
			sITEM.m_uiQuantity = 10;
	} else
	if ( sITEM.GetTYPE() >= ITEM_TYPE_USE && sITEM.GetTYPE() != ITEM_TYPE_RIDE_PART ) { // ITEM_TYPE_RIDE_PART은 제외해야 함
		sITEM.m_uiQuantity = 1;
	} else {
		// 소켓 갯수 :: 05.05.25 대만 오베이후에는 드롭 아이템에 소켓 없다.
		// short nRareType = IsTAIWAN() ? 0 : ITEM_RARE_TYPE( sITEM.GetTYPE(), sITEM.GetItemNO() );
		short nRareType = ITEM_RARE_TYPE( sITEM.GetTYPE(), sITEM.GetItemNO() );
		if ( IsTAIWAN() && nRareType < 3 ) {
			nRareType = 0;
		}

		switch( nRareType ) {
			case 3 :	// 대만 레어 아이템 
				sITEM.m_nGEM_OP = 100 + RANDOM(41);
				break;
			case 1 :	// 무조건
				sITEM.m_bHasSocket = 1;
				sITEM.m_bIsAppraisal = 0;
				break;
			case 2 :	// 계산
				if ( ITEM_QUALITY( sITEM.GetTYPE(), sITEM.GetItemNO() ) + 60 - RANDOM(400) > 0 ) {
					sITEM.m_bHasSocket = 1;
					sITEM.m_bIsAppraisal = 0;
					break;
				}
			case 0 :
			{
				iTEMP = 1+RANDOM(100);
				if ( sITEM.GetTYPE() != ITEM_TYPE_JEWEL ) {
					// 장비는 각 stb의 기본 품질 값을 설정.
					int iITEM_OP = (int)( ( ( pMobCHAR->Get_LEVEL()*0.4f + ( NPC_DROP_ITEM( pMobCHAR->Get_CharNO() )-35 )*4 + 80 - iTEMP + iCharm ) * 24 / ( iTEMP + 13 ) ) - 100 );
					if ( iITEM_OP > 0 ) {
						if ( IsTAIWAN() ) {
							int iOption = RANDOM(100);
							if ( iOption <= 35 ) {	// 1-6
								iOption = 1 + RANDOM(7-1);
							} else
							if ( iOption <= 70 ) {	// 7-48
								iOption = 7 + RANDOM(49-7);
							} else
							if ( iOption <= 85 ) {	// 49-64
								iOption = 49 + RANDOM(65-49);
							} else
							if ( iOption <= 95 ) {	// 65-72
								iOption = 65 + RANDOM(73-65);
							} else {				// 73-80
								iOption = 73 + RANDOM(81-73);
							}
							sITEM.m_nGEM_OP = iOption;
							sITEM.m_bIsAppraisal = 1;
						} else {
							if ( pMobCHAR->Get_LEVEL() < 230 )
								sITEM.m_nGEM_OP = iITEM_OP % ( pMobCHAR->Get_LEVEL()+70 );
							else
								sITEM.m_nGEM_OP = iITEM_OP % 301;

							sITEM.m_bIsAppraisal = sITEM.m_nGEM_OP ? 0 : 1;
						}
					}
				}
				break;
			}
		}

		// 내구도 결정
		iTEMP = (int)( ITEM_DURABITY( sITEM.GetTYPE(), sITEM.GetItemNO() ) * ( pMobCHAR->Get_LEVEL()*0.3f+NPC_DROP_ITEM( pMobCHAR->Get_CharNO() )*2 + 320 ) * 0.5f / ( RANDOM(100)+201 ) );
		if ( iTEMP > 100 ) iTEMP = 100;
		sITEM.m_cDurability = iTEMP;	

		// 수명 결정
		iTEMP = (int) ( ( NPC_DROP_ITEM( pMobCHAR->Get_CharNO() )+200 ) * 80 / ( 31 + RANDOM(100) ) );
		if ( iTEMP > MAX_ITEM_LIFE ) iTEMP = MAX_ITEM_LIFE;
		sITEM.m_nLife = iTEMP;

		if ( IsTAIWAN() ) {
			// 05.05.25 대만 오베 이후에는 재련 없다
			sITEM.m_cGrade = 0;
		} else {
			switch( sITEM.GetTYPE() ) {
				case ITEM_TYPE_WEAPON	:
				case ITEM_TYPE_SUBWPN	:
				case ITEM_TYPE_HELMET	:
				case ITEM_TYPE_ARMOR	:
				case ITEM_TYPE_GAUNTLET	:
				case ITEM_TYPE_BOOTS	:
				case ITEM_TYPE_RIDE_PART:
				{
					// 제련등급 결정.
					iTEMP = 1+RANDOM(100);
					// ITEM_GRADE = [ { (DROP_ITEM-5)*3 + 150 - MOB_LV*1.5 - TEMP + CHA } * 0.4 / ( TEMP + 30 ) ] - 1
					int iITEM_GRADE = (int)( ( (NPC_DROP_ITEM( pMobCHAR->Get_CharNO() ) - 5 ) * 3 + 150 - pMobCHAR->Get_LEVEL()*1.5f - iTEMP + iCharm ) * 0.4f / ( iTEMP + 30 ) ) - 1;
					if ( iITEM_GRADE >= 1 ) {
						if ( iITEM_GRADE >=3 )
							sITEM.m_cGrade = 3;
						else
							sITEM.m_cGrade = iITEM_GRADE;
					}
					break;
				}
			}
		}
	}
	return true;
}

//-------------------------------------------------------------------------------------------------
// iGiveDamage = 공격자가준 데미지
__int64 CCal::Get_EXP (CObjCHAR *pAtkCHAR, CObjCHAR *pDefCHAR, int iGiveDamage) // JDOEBOY: Changed to __int64 for high EXP support
{
	int iGAB, iEXP;

	iGAB = pAtkCHAR->Get_LEVEL() - pDefCHAR->Get_LEVEL();
	if( IsTAIWAN() ) // 대만 6-13 kchs
	{
		if( iGiveDamage  > pDefCHAR->Get_MaxHP() * 1.15f )
			iGiveDamage  = (int)(pDefCHAR->Get_MaxHP() * 1.15f); 

		if ( iGAB <= 3 ) {
			iEXP = (int)( (float)( ( pDefCHAR->Get_LEVEL() + 3 ) * pDefCHAR->Get_GiveEXP() * 
				( iGiveDamage + pDefCHAR->Get_MaxHP()/15.f+30 ) ) * ::Get_WorldEXP() / (pDefCHAR->Get_MaxHP()) / 370.f );
		} else if( iGAB >= 4 && iGAB < 9 ) {
			iEXP = (int)( (float)( ( pDefCHAR->Get_LEVEL() + 3 ) * pDefCHAR->Get_GiveEXP() * 
				( iGiveDamage + pDefCHAR->Get_MaxHP()/15.f+30 ) ) * ::Get_WorldEXP() / pDefCHAR->Get_MaxHP() / (iGAB+3) / 60.f );
		} else {
			// 대만 6-13 kchs
			iEXP = (int)( (float)( ( pDefCHAR->Get_LEVEL() + 3 ) * pDefCHAR->Get_GiveEXP() * 
				( iGiveDamage + pDefCHAR->Get_MaxHP()/15.f+30 ) ) * ::Get_WorldEXP() / pDefCHAR->Get_MaxHP() / (iGAB+3) / 180.f );
		}		
	}
	else
	{
		if ( iGAB <= 3 ) {
			iEXP = (int)( (float)( ( pDefCHAR->Get_LEVEL() + 3 ) * pDefCHAR->Get_GiveEXP() * 
				( iGiveDamage + pDefCHAR->Get_MaxHP()/15.f+30 ) ) * ::Get_WorldEXP() / (pDefCHAR->Get_MaxHP()) / 370.f );
		} else {
			iEXP = (int)( (float)( ( pDefCHAR->Get_LEVEL() + 3 ) * pDefCHAR->Get_GiveEXP() * 
				( iGiveDamage + pDefCHAR->Get_MaxHP()/15.f+30 ) ) * ::Get_WorldEXP() / pDefCHAR->Get_MaxHP() / (iGAB+3) / 60.f );
		}

	}

	if( iEXP < 1 ) 
		iEXP = 1;

	return iEXP;
}


//-------------------------------------------------------------------------------------------------
// 공격 성공도
int CCal::Get_SuccessRATE( CObjCHAR *pATK, CObjCHAR *pDEF )	// , int &iCriticalSUC )
{
	int iRAND1, iRAND2;
	int iSuccess;

	if ( pATK->IsUSER() ) {
		if ( pDEF->IsUSER() ) {
			// PVP일경우 성공 확률...
			iRAND1 = 1+RANDOM(100);
			if( IsTAIWAN() )
			{
				// 대만 2005.06.08 kchs
				iSuccess = (int)( 90 - ((pATK->Get_HIT()+pDEF->Get_AVOID() )/pATK->Get_HIT() ) * 40.f + iRAND1 );
			}
			else
			{
				// 2005-08-09 kchs : 잘못된 공식 수정
				iSuccess = (pATK->Get_HIT() - pDEF->Get_AVOID() * 0.7f + 1 + RANDOM(180) + 10 )* 0.2f;
			}
			
		} else {
			iRAND1 = 1+RANDOM(50);//1+RANDOM(100) * 0.6f;
			iRAND2 = 1+RANDOM(60);//1+RANDOM(100) * 0.7f;
			iSuccess = (int)( (pATK->Get_LEVEL()+10) - pDEF->Get_LEVEL()*1.1f + (iRAND1/* *0.6f */) );
			if ( iSuccess <= 0 )
				return 0;

			return (int)( iSuccess * (pATK->Get_HIT()*1.1f - pDEF->Get_AVOID()*0.93f + iRAND2 /* *0.7f */ + 5 + pATK->Get_LEVEL()*0.2f ) / 80.f );
		}
	} else {
		if ( IsTAIWAN() ) {
			iRAND1 = 1+RANDOM(100);
			iSuccess = 138 - ( (float)( pATK->Get_HIT() + pDEF->Get_AVOID() ) / pATK->Get_HIT() ) * 75.0f + iRAND1; // 2005-7-13 kchs 
		} else {
			iRAND1 = 1+RANDOM(50);//1+RANDOM(100) * 0.6f;
			iRAND2 = 1+RANDOM(60);//1+RANDOM(100) * 0.7f;
			iSuccess = (int)( (pATK->Get_LEVEL()+10) - pDEF->Get_LEVEL()*1.1f + (iRAND1/* *0.6f */) );
			if ( iSuccess <= 0 )
				return 0;

			return (int)( iSuccess * (pATK->Get_HIT()*1.1f - pDEF->Get_AVOID()*0.93f + iRAND2 /* *0.7f */ + 5 + pATK->Get_LEVEL()*0.2f ) / 80.f );
		}
	}

	return iSuccess;
}

//-------------------------------------------------------------------------------------------------
// 크리티컬 Success 값 공식 2005-7-13 추가
int CCal::Get_CriSuccessRATE( CObjCHAR *pATK )
{
	int iCriSuc = 0;
	if ( IsTAIWAN() ) {
		// 대만적용 (4/23~) * CRIT_SUC = { ( RAN(1~100) * 3 + A_LV + 60 } * 25 / (CRITICAL + 150)
		// CRIT_SUC = (28-{( CRITICAL /2+ A_LV)/( A_LV +8)}*20)+ RAN(1~100) (대만 2005.06.01)
		if( pATK->IsUSER() )
			iCriSuc = (int)(28-(( pATK->Get_CRITICAL()/2.f + pATK->Get_LEVEL() ) / (pATK->Get_LEVEL()+8))*20 ) + 1+RANDOM(100);
		else
			iCriSuc = 1 + RANDOM( 100 );
	} else {
		iCriSuc = (int)( ( (1+RANDOM(100))*3 + pATK->Get_LEVEL() + 30 ) * 16 / (pATK->Get_CRITICAL()+70) );
	}

	return iCriSuc;
}

//-------------------------------------------------------------------------------------------------
// 일반 물리 데미지 계산식...
WORD CCal::Get_BasicDAMAGE (CObjCHAR *pATK, CObjCHAR *pDEF, WORD wHitCNT, int iSuc)
{
	int iDamage, iCriSuc;

	iCriSuc = Get_CriSuccessRATE( pATK );

	// 맞는 동작 확률.
	int iHitActRATE  = ( 28 - iCriSuc ) * ( pATK->Get_ATK()+20 ) / ( pDEF->Get_DEF()+5 );

	if ( iCriSuc < 20 ) {
		// Critical damage !!!
		if ( pATK->IsUSER() && pDEF->IsUSER() ) {
			// PVP :: 크리티컬 물리 데미지 
			if ( IsTAIWAN() ) {
				iDamage = (int)( pATK->Get_ATK() * ((float)pDEF->Get_LEVEL()/pATK->Get_LEVEL()) * ( iSuc*0.05f+29)*(2.4f * pATK->Get_ATK() - pDEF->Get_DEF() + 180 ) / (1.1f * pDEF->Get_DEF() + pDEF->Get_AVOID() * 0.3f + 50 ) / 85.f );
			} else {
				iDamage = (int)( pATK->Get_ATK() * ( iSuc*0.05f + 35 ) * ( pATK->Get_ATK() - pDEF->Get_DEF() + 430 ) / ( ( pDEF->Get_DEF()+pDEF->Get_AVOID()*0.4f+10)*300 ) + 25 );
			}
		} else {
			// 몬스터 :: 크리티컬 물리 데미지
			if ( IsTAIWAN() ) {
				iDamage = (int)( pATK->Get_ATK() * ( iSuc*0.05f + 29 ) * ( 2.4f * pATK->Get_ATK() - pDEF->Get_DEF() + 180 ) / ( 1.1f * pDEF->Get_DEF() + pDEF->Get_AVOID() * 0.3f + 50 )/ 85.f );
			} else {
				iDamage = (int)( pATK->Get_ATK() * ( iSuc*0.05f + 29 ) * ( pATK->Get_ATK() - pDEF->Get_DEF() + 230 ) / ( ( pDEF->Get_DEF()+pDEF->Get_AVOID()*0.3f+5 )*100 ) );
			}
		}
		// 추가 데미지 스킬 ...
		if ( FLAG_ING_DUMMY_DAMAGE & pATK->GetIngDurationStateFLAG() ) {
			iDamage += (int)( iDamage * SKILL_POWER( pATK->GetIngDurationStateSKILL( ING_DUMMY_DAMAGE ) ) / 100.f );
			// iDamage |= DMG_BIT_DUMMY;
		}


		iDamage *= wHitCNT;

		if ( iDamage < 10 ) iDamage = 10;
		else {
			if ( pATK->IsUSER() && pDEF->IsUSER() ) {
				int iMaxDmg = (int)( pDEF->Get_MaxHP()*0.35f );
				if ( iDamage > iMaxDmg ) iDamage = iMaxDmg;
			} else
				if ( iDamage > MAX_DAMAGE )	iDamage = MAX_DAMAGE;
		}

		iDamage |= DMG_BIT_CRITICAL;
	} else {
		// Normal damage
		if ( pATK->IsUSER() && pDEF->IsUSER() ) {
			// PVP :: 일반 물리 데미지
			if ( IsTAIWAN() ) {
				iDamage = (int)( pATK->Get_ATK() * ( (float)pDEF->Get_LEVEL()/pATK->Get_LEVEL() ) * ( iSuc*0.03f + 26 ) * ( 1.8f * pATK->Get_ATK() - pDEF->Get_DEF() +150 ) / 
								(1.1f * pDEF->Get_DEF() + pDEF->Get_AVOID() * 0.4f + 50 ) / 126.f);
			} else {
				iDamage = (int)( pATK->Get_ATK() * ( iSuc*0.05f + 25 ) * ( pATK->Get_ATK() - pDEF->Get_DEF() + 400 ) / ( ( pDEF->Get_DEF()+pDEF->Get_AVOID()*0.4f+5 )*420 ) + 20 );
			}
		} else {
			// 몬스터 :: 일반 물리 데미지
			if ( IsTAIWAN() ) {
				// [일반 데미지 계산식] * DMG = A_ATT * (SUC*0.03 + 26) * (A_ATT - D_DEF + 300) / (D_DEF+ D_AVO *0.3+30) /180 
				// DMG = A_ATT * ( SUC*0.03+26)*(1.8* A_ATT - D_DEF +150) / (1.1* D_DEF + D_AVO *0.4+50)/145  (대만 2005.06.01)
				iDamage = (int)( pATK->Get_ATK() * ( iSuc*0.03f+26)*(1.8f* pATK->Get_ATK() - pDEF->Get_DEF() +150) / (1.1f* pDEF->Get_DEF() + pDEF->Get_AVOID() * 0.4f+50)/145.f );
			} else {
				iDamage = (int)( pATK->Get_ATK() * ( iSuc*0.03f + 26 ) * ( pATK->Get_ATK() - pDEF->Get_DEF() + 250 ) / ( ( pDEF->Get_DEF()+pDEF->Get_AVOID()*0.4f+5 )*145 ) );
			}
		}
		// 추가 데미지 스킬 ...
		if ( FLAG_ING_DUMMY_DAMAGE & pATK->GetIngDurationStateFLAG() ) {
			iDamage += (int)( iDamage * SKILL_POWER( pATK->GetIngDurationStateSKILL( ING_DUMMY_DAMAGE ) ) / 100.f );
			// iDamage |= DMG_BIT_DUMMY;
		}

		iDamage *= wHitCNT;

		if ( iDamage < 5 ) iDamage = 5;
		else {
			if ( pATK->IsUSER() && pDEF->IsUSER() ) {
				int iMaxDmg = (int)( pDEF->Get_MaxHP()*0.25f );
				if ( iDamage > iMaxDmg )
					iDamage = iMaxDmg;
			} else
				if ( iDamage > MAX_DAMAGE ) iDamage = MAX_DAMAGE;
		}
	}

	return ( iHitActRATE >= 10 ) ? ( DMG_BIT_HITTED | iDamage ) : iDamage;
}


//-------------------------------------------------------------------------------------------------
// 마법 데미지
WORD CCal::Get_MagicDAMAGE (CObjCHAR *pATK, CObjCHAR *pDEF, WORD wHitCNT, int iSuc)
{
	int iDamage, iCriSuc;
	
	iCriSuc = Get_CriSuccessRATE( pATK );

	// 맞는 동작 확률.
	int iHitActRATE  = ( 28 - iCriSuc ) * ( pATK->Get_ATK()+20 ) / ( pDEF->Get_RES()+5 );

	if ( iCriSuc < 20 ) {
		// Critical damage !!!
		if ( pATK->IsUSER() && pDEF->IsUSER() ) {	// 마법 크리티컬 PVP
			// PVP :: 크리티컬 마법 데미지
			if ( IsTAIWAN() ) {
				// DMG = A_ATT* (D_LV/A_LV) *( SUC *0.05+25)*(2.4* A_ATT - D_DEF +180)/(1.1* D_RES + D_AVO *0.3+50)/110  (대만 2005.06.01)
				iDamage = (int)( pATK->Get_ATK() * ((float)pDEF->Get_LEVEL()/pATK->Get_LEVEL()) * ( iSuc*0.05f+25 ) * (2.4f * pATK->Get_ATK() - pDEF->Get_DEF() + 180 ) / ( 1.1f * pDEF->Get_RES() + pDEF->Get_AVOID() * 0.3f + 50 ) / 90.f );	//  (IROSE 2005.05.13)
			} else {
				iDamage = (int)( pATK->Get_ATK() * (iSuc*0.08f+33) * ( pATK->Get_ATK() - pDEF->Get_DEF()+340 ) / ( ( pDEF->Get_RES()+pDEF->Get_AVOID()*0.3f+20) * 360 ) + 25 );
			}
		} else {
			// 몬스터 :: 크리티컬 마법 데미지
			if ( IsTAIWAN() ) {
				iDamage = (int)( pATK->Get_ATK() * ( iSuc * 0.05f + 25 ) * (2.4f* pATK->Get_ATK() - pDEF->Get_DEF() +180)/(1.1f* pDEF->Get_RES() + pDEF->Get_AVOID() *0.3f+50 ) / 90.f );
			} else {
				iDamage = (int)( pATK->Get_ATK() * (iSuc*0.05f+33) * ( pATK->Get_ATK() - pDEF->Get_DEF()*0.8f+310 ) / ( ( pDEF->Get_RES()+pDEF->Get_AVOID()*0.3f+5 ) * 200 ) );
			}
		}

		// 추가 데미지 ...
		if ( FLAG_ING_DUMMY_DAMAGE & pATK->GetIngDurationStateFLAG() ) {
			iDamage += (int)( iDamage * SKILL_POWER( pATK->GetIngDurationStateSKILL( ING_DUMMY_DAMAGE ) ) / 100.f );
			// iDamage |= DMG_BIT_DUMMY;
		}

		iDamage *= wHitCNT;

		if ( iDamage < 10 ) iDamage = 10;
		else {
			if ( pATK->IsUSER() && pDEF->IsUSER() ) {
				int iMaxDmg = (int)( pDEF->Get_MaxHP()*0.35f );
				if ( iDamage > iMaxDmg )
					iDamage = iMaxDmg;
			} else
				if ( iDamage > MAX_DAMAGE ) iDamage = MAX_DAMAGE;
		}

		iDamage |= DMG_BIT_CRITICAL;
	} else {
		// Normal damage
		if ( pATK->IsUSER() && pDEF->IsUSER() ) {	// 마법 일반 PVP
			// PVP :: 일반 마법 데미지
			if ( IsTAIWAN() ) {
				iDamage = (int)( (float)pATK->Get_ATK() * (pDEF->Get_LEVEL()/pATK->Get_LEVEL() ) * ( iSuc*0.03f+25 )*( 1.8f * pATK->Get_ATK() - pDEF->Get_DEF() + 150 ) / ( 1.1f* pDEF->Get_RES() + pDEF->Get_AVOID() * 0.4f + 50 ) / 120.f );	// kchs 수정 : 2005-08-12 D_LEV - A_LEV ==> D_LEV / A_LEV
			} else {
				iDamage = (int)( (float)pATK->Get_ATK() * (iSuc*0.06f+29) * ( pATK->Get_ATK() - pDEF->Get_DEF()*0.8f+350 ) / ( ( pDEF->Get_RES()+pDEF->Get_AVOID()*0.3f+5 ) * 640 ) + 20 );
			}
		} else {
			// 몬스터 :: 일반 마법 데미지
			if ( IsTAIWAN() ) {
				// [일반 데미지 계산식] * DMG = A_ATT * (SUC*0.03 + 25) * (A_ATT - D_DEF + 300) / (D_RES+ D_AVO * 0.3 + 30) /200 
				// DMG = A_ATT *( SUC *0.03+25)*(1.8 *A_ATT - D_DEF +150)/(1.1* D_RES + D_AVO *0.4+50)/120  (대만 2005.06.01)
				iDamage = (int)( pATK->Get_ATK() * ( iSuc *0.03f+25) * (1.8f *pATK->Get_ATK() - pDEF->Get_DEF() +150)/(1.1f* pDEF->Get_RES() + pDEF->Get_AVOID() * 0.4f + 50 ) / 120.f );
			} else {
				iDamage = (int)( pATK->Get_ATK() * (iSuc*0.03f+30) * ( pATK->Get_ATK() - pDEF->Get_DEF()*0.8f+280 ) / ( ( pDEF->Get_RES()+pDEF->Get_AVOID()*0.3f+5 ) * 280 ) );
			}
		}

		// 추가 데미지 ...
		if ( FLAG_ING_DUMMY_DAMAGE & pATK->GetIngDurationStateFLAG() ) {
			iDamage += (int)( iDamage * SKILL_POWER( pATK->GetIngDurationStateSKILL( ING_DUMMY_DAMAGE ) ) / 100.f );
			// iDamage |= DMG_BIT_DUMMY;
		}

		iDamage *= wHitCNT;

		if ( iDamage < 5 ) iDamage = 5;
		else {
			if ( pATK->IsUSER() && pDEF->IsUSER() ) {
				int iMaxDmg = (int)( pDEF->Get_MaxHP()*0.25f );
				if ( iDamage > iMaxDmg )
					iDamage = iMaxDmg;
			} else
				if ( iDamage > MAX_DAMAGE ) iDamage = MAX_DAMAGE;
		}
	}

	return ( iHitActRATE >= 10 ) ? ( DMG_BIT_HITTED | iDamage ) : iDamage;
}

//-------------------------------------------------------------------------------------------------
WORD CCal::Get_DAMAGE (CObjCHAR *pATK, CObjCHAR *pDEF, WORD wHitCNT)
{
	int iSuc = Get_SuccessRATE( pATK, pDEF );

	if ( iSuc < 20 ) {
		if ( (int)( 1+RANDOM(100) + (pATK->Get_LEVEL()-pDEF->Get_LEVEL() ) * 0.6f ) < 94 )
			return 0;
	}

	if ( IsTAIWAN() ) {
		// 공격 모션에 타점은 무조건 1개로 취급~
		wHitCNT = 1;
	}

	if ( pATK->IsMagicDAMAGE () )
		return Get_MagicDAMAGE( pATK, pDEF, wHitCNT, iSuc );

	return Get_BasicDAMAGE( pATK, pDEF, wHitCNT, iSuc );
}

//-------------------------------------------------------------------------------------------------
int  CCal::Get_WeaponSkillDAMAGE (CObjCHAR *pATK, CObjCHAR *pDEF, short nSkillIDX, int iSuccess)
{
	int iDamage;

	// [무기 스킬형] 데미지 계산식 
	if ( pATK->IsUSER() && pDEF->IsUSER() ) {
		// DMG= (SKILL_POW/100) *(D_LV/A_LV) * A_ATT * ( SUC*0.03+26)*(1.8* A_ATT - D_DEF +150) / (1.1* D_DEF + D_AVO *0.4+50)/145
		iDamage = (int)(	( SKILL_POWER(nSkillIDX) / 100.f ) * 
							( (float)pDEF->Get_LEVEL()/pATK->Get_LEVEL() ) *
							( pATK->Get_ATK() ) *
							( iSuccess * 0.03f+26 ) *
							( 1.8f * pATK->Get_ATK() - pDEF->Get_DEF() + 150 ) /
							( 1.1f * pDEF->Get_DEF() + pDEF->Get_AVOID() * 0.4f + 50 ) / 126.f );
	} else {
		// DMG = (SKILL_POW/100 ) * A_ATT * ( SUC*0.03+26)*(1.8* A_ATT - D_DEF +150) / (1.1* D_DEF + D_AVO *0.4+50)/145
		iDamage = (int)(	( SKILL_POWER(nSkillIDX) / 100.f ) *
							( pATK->Get_ATK() ) *
							( iSuccess * 0.03f + 26 ) *
							( 1.8f * pATK->Get_ATK() - pDEF->Get_DEF() + 150 ) /
							( 1.1f * pDEF->Get_DEF() + pDEF->Get_AVOID() * 0.4f + 50 ) / 126.f );
	}

	return iDamage;
}
int  CCal::Get_MagicSkillDAMAGE (CObjCHAR *pATK, CObjCHAR *pDEF, short nSkillIDX, int iSuccess)
{
	int iDamage;

	// [마법 스킬형] 데미지 계산식 
	if ( pATK->IsUSER() && pDEF->IsUSER() ) {
		// DMG = (SKILL_POW/100) *(D_LV/A_LV) * A_ATT *( SUC *0.03+25)*(1.8 *A_ATT - D_DEF +150)/(1.1* D_RES + D_AVO *0.4+50)/140
		iDamage = (int)(	( SKILL_POWER(nSkillIDX)/100.f ) *
							( (float)pDEF->Get_LEVEL() / pATK->Get_LEVEL() ) *
							( pATK->Get_ATK() ) *
							( iSuccess * 0.03f + 25 ) *
							( 1.8f * pATK->Get_ATK() - pDEF->Get_DEF() + 150 ) /
							( 1.1f * pDEF->Get_RES() + pDEF->Get_AVOID() * 0.4f + 50 ) / 120.f );
	} else {
		// DMG = (SKILL_POW/100)  * A_ATT *( SUC *0.03+25)*(1.8 *A_ATT - D_DEF +150)/(1.1* D_RES + D_AVO *0.4+50)/120
		iDamage = (int)(	( SKILL_POWER(nSkillIDX)/100.f ) *
							( pATK->Get_ATK() ) *
							( iSuccess * 0.03f + 25 ) *
							( 1.8f * pATK->Get_ATK() - pDEF->Get_DEF() + 150 ) /
							( 1.1f * pDEF->Get_RES() + pDEF->Get_AVOID() * 0.4f + 50 ) / 120.f );
	}

	return iDamage;
}

WORD CCal::Get_SkillDAMAGE (CObjCHAR *pATK, CObjCHAR *pDEF, short nSkillIDX, WORD wHitCNT)
{
	int iDamage;
	
	if ( IsTAIWAN() ) {
		// 공격 모션에 타점은 무조건 1개로 취급~
		wHitCNT = 1;
	}

	// int iSuccess  = Get_SuccessRATE (pATK, pDEF); // 2005-7-26 kchs 수정. 잘못되서 다시 옛날로 복원
	switch( SKILL_DAMAGE_TYPE( nSkillIDX ) ) {
		case 1 :	// 무기 스킬
		{
			int iSuccess;
			int iRAND1 = 1+RANDOM(60);
			int iRAND2 = 1+RANDOM(70);
			iSuccess = (int)( ( (pATK->Get_LEVEL()+20) - pDEF->Get_LEVEL() + (iRAND1 /* *0.6f */) ) * (pATK->Get_HIT() - pDEF->Get_AVOID()*0.6f + iRAND2 /* *0.7f */ + 10 ) / 110.f );
			if ( IsTAIWAN() ) {
				if ( iSuccess < 20 )
					return 0;

				iDamage = Get_WeaponSkillDAMAGE( pATK, pDEF, nSkillIDX, iSuccess );
			} else {
				if ( iSuccess < 20 ) {
					if ( iSuccess < 10 )
						return 0;

					iDamage = (int)( (	( SKILL_POWER(nSkillIDX)*0.4f ) * 
										( pATK->Get_ATK()+50 ) *
										( (1+RANDOM(30)) + pATK->Get_SENSE()*1.2f + 340 ) ) /
										( pDEF->Get_DEF() + pDEF->Get_RES() + 20 ) / 
										( 250+pDEF->Get_LEVEL()-pATK->Get_LEVEL() ) + 20 );
				} else {
					if ( pATK->IsUSER() && pDEF->IsUSER() ) {
						iDamage = (int)( (	( SKILL_POWER(nSkillIDX)+pATK->Get_ATK()*0.2f ) * 
											( pATK->Get_ATK()+60 ) *
											( (1+RANDOM(30)) + pATK->Get_SENSE()*0.7 + 370 ) ) * 0.01 * ( 320-pDEF->Get_LEVEL()+pATK->Get_LEVEL() ) /
											( pDEF->Get_DEF() + pDEF->Get_RES()*0.8f + pDEF->Get_AVOID()*0.4f + 40 ) / 1600 + 60 );
					} else {
						// [무기 스킬형] 데미지 계산식 
						iDamage = (int)( (	( SKILL_POWER(nSkillIDX)+pATK->Get_ATK()*0.2f ) * 
											( pATK->Get_ATK()+60 ) *
											( (1+RANDOM(30)) + pATK->Get_SENSE()*0.7f + 370 ) ) * 0.01 * ( 120-pDEF->Get_LEVEL()+pATK->Get_LEVEL() ) /
											( pDEF->Get_DEF() + pDEF->Get_RES()*0.8f + pDEF->Get_AVOID()*0.4f + 20 ) / 270 + 20 );
					}
				}
			}
			break;
		}
		case 2 :	// 마법 스킬
		{
			int iSuccess;
			int iRAND1 = 1+RANDOM(50);
			int iRAND2 = 1+RANDOM(70);
			iSuccess = (int)( ( (pATK->Get_LEVEL()+30) - pDEF->Get_LEVEL() + (iRAND1) ) * (pATK->Get_HIT() - pDEF->Get_AVOID()*0.56f + iRAND2 + 10 ) / 110.f );
			if ( IsTAIWAN() ) {
				iDamage = Get_MagicSkillDAMAGE( pATK, pDEF, nSkillIDX, iSuccess );
			} else {
				if ( iSuccess < 20 ) {
					if ( iSuccess < 8 )
						return 0;
					iDamage = (int)(	( SKILL_POWER(nSkillIDX)*(pATK->Get_ATK()*0.8f+pATK->Get_INT()+80) * 
										( (1+RANDOM(30)) + pATK->Get_SENSE()*1.3f + 280 ) * 0.2f ) /
										( pDEF->Get_DEF()*0.3f + pDEF->Get_RES() + 30 ) / 
										( 250+pDEF->Get_LEVEL()-pATK->Get_LEVEL() ) + 20 );
				} else {
					if ( pATK->IsUSER() && pDEF->IsUSER() ) {
						iDamage = (int)(	( (SKILL_POWER(nSkillIDX)+50)*(pATK->Get_ATK()*0.8f+ pATK->Get_INT()*1.2f +100) * 
											( (1+RANDOM(30)) + pATK->Get_SENSE()*0.7f + 350 ) * 0.01f ) * ( 380-pDEF->Get_LEVEL()+pATK->Get_LEVEL() ) /
											( pDEF->Get_DEF()*0.4f + pDEF->Get_RES() + pDEF->Get_AVOID()*0.3f + 60 ) / 2500.f + 60 );
					} else {
						iDamage = (int)(	( SKILL_POWER(nSkillIDX)*(pATK->Get_ATK()*0.8f+ pATK->Get_INT()*1.2f +100) * 
											( (1+RANDOM(30)) + pATK->Get_SENSE()*0.7f + 350 ) * 0.01f ) * ( 150-pDEF->Get_LEVEL()+pATK->Get_LEVEL() ) /
											( pDEF->Get_DEF()*0.3f + pDEF->Get_RES() + pDEF->Get_AVOID()*0.3f + 60 ) / 350.f + 20 );
					}
				}
			}
			break;
		}
		case 3 :	// 맨손 스킬
		{
			int iSuccess;
			int iRAND1 = 1+RANDOM(80);
			int iRAND2 = 1+RANDOM(50);
			iSuccess = (int)( ( (pATK->Get_LEVEL()+10) - pDEF->Get_LEVEL() + (iRAND1) ) * (pATK->Get_HIT() - pDEF->Get_AVOID()*0.5f + iRAND2 + 50 ) / 90.f );
			if ( iSuccess < 20 ) {
				if ( iSuccess < 6 )
					return 0;
				iDamage = (int)(	( SKILL_POWER(nSkillIDX)*(SKILL_POWER(nSkillIDX)+pATK->Get_INT()+80) * 
									( (1+RANDOM(30)) + pATK->Get_SENSE() * 2 + 290 ) * 0.2f ) / 
									( pDEF->Get_DEF()*0.2f + pDEF->Get_RES() + 30 ) / 
									( 250+pDEF->Get_LEVEL()-pATK->Get_LEVEL() ) + 20 );
			} else {
				if ( pATK->IsUSER() && pDEF->IsUSER() ) {
					iDamage = (int)(	( ( 35+SKILL_POWER(nSkillIDX) ) * ( SKILL_POWER(nSkillIDX)+pATK->Get_INT()+140 ) * 
										( (1+RANDOM(30)) + pATK->Get_SENSE() + 380 ) * 0.01f ) * ( 400-pDEF->Get_LEVEL()+pATK->Get_LEVEL() ) /
										( pDEF->Get_DEF()*0.5f + pDEF->Get_RES()*1.2f + pDEF->Get_AVOID()*0.4f + 20 ) / 3400 + 40 );
				} else {
					iDamage = (int)(	( ( 35+SKILL_POWER(nSkillIDX) ) *( SKILL_POWER(nSkillIDX)+pATK->Get_INT()+140 ) * 
										( (1+RANDOM(30)) + pATK->Get_SENSE() + 380 ) * 0.01f ) * ( 150-pDEF->Get_LEVEL()+pATK->Get_LEVEL() ) /
										( pDEF->Get_DEF()*0.35f + pDEF->Get_RES()*1.2f + pDEF->Get_AVOID()*0.4f + 10 ) / 730 + 20 );
				}
			}
			break;
		}
		default :
		{
			// 일반 공격 데미지로 계산...
			int iSuccess;
			int iRAND1 = 1+RANDOM(80);
			int iRAND2 = 1+RANDOM(50);
			iSuccess = (int)( ( (pATK->Get_LEVEL()+8) - pDEF->Get_LEVEL() + (iRAND1/* *0.8 */) ) * (pATK->Get_HIT() - pDEF->Get_AVOID()*0.6f + iRAND2/* *0.5 */ + 50 ) / 90 );
			if ( iSuccess < 20 ) {
				if ( iSuccess < 10 )
					return 0;
				iDamage = (int)( (	( SKILL_POWER(nSkillIDX)+40 ) * 
									( pATK->Get_ATK()+40 ) *
									( (1+RANDOM(30)) + pATK->Get_CRITICAL()*0.2f + 40 ) ) * 0.4f /
									( pDEF->Get_DEF() + pDEF->Get_RES()*0.3f + pDEF->Get_AVOID()*0.4f + 10 ) / 80 + 5 );
			} else {
				if ( pATK->IsUSER() && pDEF->IsUSER() ) {
					iDamage = (int)( (	( SKILL_POWER(nSkillIDX)+pATK->Get_CRITICAL()*0.15f+40 ) * 
										( pATK->Get_ATK() ) *
										( (1+RANDOM(30)) + pATK->Get_CRITICAL()*0.32f + 35 ) ) * 0.01f * ( 350-pDEF->Get_LEVEL()+pATK->Get_LEVEL() ) /
										( pDEF->Get_DEF() + pDEF->Get_RES()*0.3f + pDEF->Get_AVOID()*0.4f + 35 ) / 400 + 20 );
				} else {
					if ( IsTAIWAN() ) {
						// [모션 스킬형] 데미지 계산식
							//* DMG = { (SKILL_POW+A_CRI*0.15 + 40) * (A_ATT+70) * { RAN(1~30) + A_CRI*0.34 + 35 } * 0.01
							//			* (200 + A_LV - D_LV ) } / (D_DEF + D_RES*0.5 + D_AVO*0.4 + 40) / 200 + 20
						iDamage = (int)( (	( SKILL_POWER(nSkillIDX)+pATK->Get_CRITICAL()*0.15f+40 ) * 
											( pATK->Get_ATK()+70 ) *
											( (1+RANDOM(30)) + pATK->Get_CRITICAL()*0.34f + 35 ) ) * 0.01f * ( 200-pDEF->Get_LEVEL()+pATK->Get_LEVEL() ) /
											( pDEF->Get_DEF() + pDEF->Get_RES()*0.5f + pDEF->Get_AVOID()*0.4f + 40 ) / 200.f + 20 );
					} else {
						iDamage = (int)( (	( SKILL_POWER(nSkillIDX)+pATK->Get_CRITICAL()*0.15f+40 ) * 
											( pATK->Get_ATK() ) *
											( (1+RANDOM(30)) + pATK->Get_CRITICAL()*0.32f + 35 ) ) * 0.01f * ( 120-pDEF->Get_LEVEL()+pATK->Get_LEVEL() ) /
											( pDEF->Get_DEF() + pDEF->Get_RES()*0.3f + pDEF->Get_AVOID()*0.4f + 10 ) / 100.f + 20 );
					}
				}
			}
		}
	}

	// 추가 데미지 ...
	if ( FLAG_ING_DUMMY_DAMAGE & pATK->GetIngDurationStateFLAG() ) {
		iDamage += (int)( iDamage * SKILL_POWER( pATK->GetIngDurationStateSKILL( ING_DUMMY_DAMAGE ) ) / 100.f );
		// iDamage |= DMG_BIT_DUMMY;
	}

#ifdef	__APPLY_2ND_JOB
	if ( IsTAIWAN() ) {
		// 속성 테이블 참조하여 재계산
		short nSA = SKILL_ATTRIBUTE( nSkillIDX );
		short nNA = pDEF->Get_ATTRIBUTE ();
		int iNewDamage = (int) ( iDamage * ATTRIBUTE_FORM( nSA, nNA ) / 100.f );
		iDamage = iNewDamage;
	}
#endif

	if( iDamage < 5 )
		iDamage = 5;

	iDamage *= wHitCNT;
	if ( pATK->IsUSER() && pDEF->IsUSER() ) {
		int iMaxDmg = (int)( pDEF->Get_MaxHP()*0.45f );
		if ( iDamage > iMaxDmg ) iDamage = iMaxDmg;
	} // else  계산식 pDEF->Get_MaxHP()*0.45f 의 결과가 MAX_DAMAGE보다 클수 있으므로 else 삭제...

	if ( iDamage > MAX_DAMAGE ) iDamage = MAX_DAMAGE;

	// 히트 동작 확률...
	int iHitActRATE;

	iHitActRATE = iDamage * ( 1+(RANDOM(100)) + 100 ) / ( pDEF->Get_AVOID()+40 ) / 14;

	return ( iHitActRATE >= 10 ) ? ( DMG_BIT_HITTED | iDamage ) : iDamage;
}


//-------------------------------------------------------------------------------------------------
int CCal::Get_SkillAdjustVALUE(	CObjUSER *pCHAR, short nSkillIDX, short nCol, int iSpellerINT )
{
	int iAbilityValue;
	
#ifndef	__SERVER
	iAbilityValue = pCHAR->GetCur_AbilityValue( SKILL_INCREASE_ABILITY(nSkillIDX, nCol) );
#else
	iAbilityValue = pCHAR->Get_AbilityValue   ( SKILL_INCREASE_ABILITY(nSkillIDX, nCol) );
#endif

	return (short)( iAbilityValue * SKILL_CHANGE_ABILITY_RATE(nSkillIDX, nCol) / 100.f + SKILL_INCREASE_ABILITY_VALUE(nSkillIDX, nCol) * ( iSpellerINT + 300 ) / 315.f );
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
