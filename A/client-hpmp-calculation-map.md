# Client-Side HP/MP Calculation and Modification Points (JDOEBOY)

This document enumerates all locations in the client codebase where HP or MP is calculated, predicted, or modified outside of direct server updates. This includes battle, skills, potions, buffs, and periodic recovery. The goal is to identify all such logic so the client can be refactored to rely 100% on server HP/MP updates.

---

## 1. Core HP/MP Modification Functions

- **CObjCHAR.cpp / CObjCHAR.h**
  - `Set_HP(int iHP)`
  - `Set_MP(int iMP)`
  - `Add_HP(int iAdd)`
  - `Add_MP(int iAdd)`
  - `Sub_HP(int iSub)`
  - `Sub_MP(int iSub)`
  - Directly modify `m_iHP`, `m_iMP`

## 2. Battle and Damage Handling

- **CObjCHAR.cpp**
  - `Apply_DAMAGE(CObjCHAR *pAtkOBJ, WORD wDamage)`
    - Calls `Sub_HP`, modifies HP immediately on client
  - Skill and attack handlers call `Apply_DAMAGE` and related functions

## 3. Skill and Buff Effects

- **CObjCHAR.cpp**
  - `ApplyEffectOfSkill(int iSkillIDX, int iObjIDX, CObjCHAR* pEffectedChar, stEFFECT_OF_SKILL* pEffectOfSkill)`
    - Calls `Add_HP`, `Add_MP` for instant skill/buff effects
  - `ProcEffectOfSkillInDamageOfSkill(...)`
    - May call `Add_HP`, `Add_MP` for skill results

## 4. Potion and Consumable Use

- **Network/RecvPACKET.cpp**
  - `Recv_gsv_USE_ITEM()`
    - Calls `Add_HP`, `Add_MP`, `Set_HP`, `Set_MP` for potion/consumable effects

## 5. Periodic Recovery (Regen)

- **Common/CObjAI.cpp**
  - `RecoverHP()` / `RecoverMP()`
    - Periodically increases HP/MP on the client
  - `Get_RecoverHP()` / `Get_RecoverMP()`
    - Calculates recovery amount

## 6. Server Sync/Correction (Authoritative)

- **Network/RecvPACKET.cpp**
  - `Recv_gsv_SET_HPnMP()`
    - Sets HP/MP to server value (authoritative)
  - `Recv_gsv_JOIN_ZONE()`, `Recv_gsv_AVT_CHAR()`, etc.
    - Set HP/MP on zone join, character load, etc.

## 7. Other Notable Locations

- **Network/RecvPACKET.cpp**
  - Skill, buff, and status effect packet handlers may call `Set_HP`, `Add_HP`, etc.
- **Util/CCheat.cpp**
  - Debug/cheat functions may set HP/MP directly

---

## Summary Table

| Location                        | Function(s)                | Type                |
|----------------------------------|----------------------------|---------------------|
| CObjCHAR.cpp / .h               | Set_HP, Add_HP, Sub_HP     | Core modification   |
| CObjCHAR.cpp                    | Apply_DAMAGE               | Battle              |
| CObjCHAR.cpp                    | ApplyEffectOfSkill         | Skill/Buff          |
| Network/RecvPACKET.cpp          | Recv_gsv_USE_ITEM          | Potion/Consumable   |
| Common/CObjAI.cpp               | RecoverHP, Get_RecoverHP   | Periodic Recovery   |
| Network/RecvPACKET.cpp          | Recv_gsv_SET_HPnMP, etc.   | Server Sync         |
| Util/CCheat.cpp                 | Set_HP, Set_MP             | Debug/Cheat         |

---

**Next Step:**
- Refactor the client to remove/disable all HP/MP calculation and prediction logic, except for direct application of server-provided values.
- Ensure the client only updates HP/MP when receiving authoritative data from the server.

*All locations and functions above must be reviewed and updated for full server authority over HP/MP.*
