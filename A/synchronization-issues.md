# Synchronization Issues Documentation

This document details known synchronization problems between client and server components in the Rose Online codebase, including health desync, EXP issues, and visual glitches from buffer overflows.

## Overview

Synchronization issues occur when client and server states become inconsistent, leading to:
- **Health desync**: Character health bars showing incorrect values
- **EXP issues**: Experience gain/loss not properly synchronized (especially late game)
- **Visual glitches**: Buffer overflow artifacts and rendering problems (confirmed with FRAROSE)
- **Character corruption**: Buffer overflow causing character name corruption (confirmed with FRAROSE)
- **Combat inconsistencies**: Damage calculations not matching between client/server

## Root Causes

Based on analysis, synchronization issues stem from:

### 1. Math Discrepancies
- Different calculation formulas between client and server
- Floating point precision differences
- Rounding errors in damage/health calculations

### 2. Packet Handling Issues
- Incorrect packet parsing or serialization
- Missing or malformed network messages
- Race conditions in packet processing
- **Buffer overflow in packet structures** (confirmed with FRAROSE - causes character name corruption)

### 3. STB File Column Issues (Possible)
- Wrong column indices when reading game data files
- Inconsistent data interpretation between client/server
- STB file format mismatches

### 4. Database Field Issues
- Incorrect field sizes or data types
- Format mismatches in stored procedures
- Data truncation or overflow in database operations

## Known Issues

### EXP Calculation Discrepancies

**Symptoms:**
- Experience gain/loss not properly synchronized between client and server
- Different EXP requirements for leveling up
- Late-game EXP calculations become unreliable

**Root Cause:**
- **Conditional compilation differences**: Server uses `#ifdef _PRE_EVO` to switch between old and new EXP formulas
- **Client has both formulas**: Client version contains both old and new EXP calculation implementations in the same file
- **Formula mismatches**: Different mathematical formulas used for EXP calculation between client prediction and server validation

**Code Evidence:**
```cpp
// Server version (sho_gs/SHO_GS_LIB/Common/Calculation.cpp)
#ifdef _PRE_EVO
    // Old EXP formula
    if (iLevel <= 15)
        return (__int64)((iLevel + 3) * (iLevel + 5) * (iLevel + 10) * 0.7);
    // ... old formulas ...
#endif

// Korean formula (always used in current builds)
if (iLevel <= 15) {
    return (__int64)(((iLevel + 3) * (iLevel + 5) * (iLevel + 10) * 0.7f));
}
// ... new formulas ...
```

**Client version contains both implementations**, leading to potential mismatches when client predicts EXP vs server validates.

**Affected Files:**
- `src/client/Common/Calculation.cpp` - Contains both old and new EXP formulas
- `src/sho_gs/SHO_GS_LIB/Common/Calculation.cpp` - Uses conditional compilation
- `src/common/Calculation.cpp` - May have different implementation

### Damage Calculation Inconsistencies

**Symptoms:**
- Combat damage not matching between client display and server calculation
- Critical hits showing different values
- Skill damage discrepancies

**Root Cause:**
- **Conditional compilation differences**: Server switches damage formulas based on `_PRE_EVO` define
- **PVP vs PVE differences**: Different formulas for player vs monster combat
- **Magic vs Physical differences**: Separate calculation paths with different implementations

**Code Evidence:**
```cpp
// Server critical damage calculation
#ifdef _PRE_EVO
    iDamage = (int)(pATK->Get_ATK() * (iSuc * 0.05f + 29) * (2.4f * pATK->Get_ATK() - pDEF->Get_DEF() + 180) / (1.1f * pDEF->Get_DEF() + pDEF->Get_AVOID() * 0.3f + 50) / 85.f);
#else
    iDamage = (int)(pATK->Get_ATK() * (iSuc * 0.05f + 29) * (pATK->Get_ATK() - pDEF->Get_DEF() + 230) / ((pDEF->Get_DEF() + pDEF->Get_AVOID() * 0.3f + 5) * 100));
#endif
```

**Client may be using different formula versions** than server, causing visual discrepancies.

### Health Desync

**Symptoms:**
- Character health bars show incorrect values
- Players appear to have more/less health than actual
- Combat becomes unpredictable
- **Client HP values do not match server HP values**
- **When consuming HP potions, health bar does not properly represent current player health**

**Potential Causes:**
- **Damage calculation mismatches**: Client and server using different damage formulas
- **Health update packet timing**: Race conditions in health synchronization
- **Buffer overflow in health values**: Memory corruption affecting health storage
- **Potion effect synchronization**: Client not properly receiving or displaying health restoration updates

**Affected Files:**
- `Calculation.*` (damage/health formulas)
- `CObjCHAR.*` (character object management)
- Network packet handlers for health updates

### Zone Change EXP Overflow

**Symptoms:**
- **When changing zones, experience from Player Information box overflows to character's floating health bar briefly**
- EXP bar displays incorrect values during zone transitions
- Visual artifacts in UI during zone loading
- **Riding a cart while changing zones causes the same UI issue, but the error persists until disabling the cart**

**Potential Causes:**
- **UI update timing issues**: EXP bar not properly cleared/reset during zone changes
- **Packet ordering problems**: Zone change packets arriving before/after EXP update packets
- **Buffer reuse issues**: UI elements sharing memory that gets corrupted during transitions
- **Race conditions**: Zone change UI updates conflicting with EXP synchronization
- **Cart state persistence**: Cart riding state not properly reset during zone transitions

**Affected Files:**
- Zone change packet handlers
- UI rendering code for EXP bars
- Character floating health bar display logic
- Cart/riding state management code

### Higher Level Character EXP Accumulation

**Symptoms:**
- **Higher level characters not properly receiving experience**
- **Experience accumulates to 200%+ without leveling up**
- EXP bar shows inflated values beyond normal maximum
- Level progression blocked despite gaining experience

**Potential Causes:**
- **EXP calculation overflow**: Integer overflow in EXP calculation for high-level characters
- **Level-based EXP scaling issues**: EXP multipliers or thresholds incorrect for higher levels
- **Database persistence problems**: EXP values not properly saved/loaded for high-level characters
- **Packet size limitations**: EXP values exceeding packet data type limits
- **UI display vs server calculation mismatch**: Server calculates correctly but UI displays wrong values

**Affected Files:**
- EXP calculation functions (likely in Calculation.* files)
- Level progression logic
- EXP packet handlers (client and server)
- Database EXP storage/retrieval code
- UI EXP bar rendering code

### Soldier Self Buff Requirements Issue

**Symptoms:**
- **Soldier Self buff not working**
- **Shows "unfulfilled requirements" when all requirements are met**
- Buff activation fails despite meeting skill level, class, and stat requirements
- Other buffs may work correctly, issue specific to Soldier Self buff

**Potential Causes:**
- **Requirement validation bug**: Incorrect requirement checking logic for Soldier Self buff
- **Skill data corruption**: Soldier Self buff data corrupted in STB files or skill definitions
- **Class validation issues**: Incorrect class checking for Soldier class buffs
- **Stat requirement miscalculation**: HP/MP/STR/etc requirements not properly validated
- **Buff ID conflicts**: Soldier Self buff ID conflicting with other skills/buffs

**Affected Files:**
- Skill validation functions
- Buff requirement checking code
- Soldier class skill definitions
- STB files for skill data
- Buff activation packet handlers

### Item Drop Rate Inconsistencies

**Symptoms:**
- Item drop rates not matching expected probabilities
- Rare item drop calculations unreliable

**Root Cause:**
- **Conditional compilation differences**: Server switches drop formulas based on `_PRE_EVO`
- **Different multipliers**: Various drop rate calculations between versions

**Code Evidence:**
```cpp
// Server drop calculation
#ifdef _PRE_EVO
    if (iLevelDiff < 9)
        iDrop_VAR = (int)((::Get_WorldDROP() + NPC_DROP_ITEM(pMobCHAR->Get_CharNO()) - (1 + RANDOM(100)) - ((iLevelDiff + 16) * 3.5f) - 10 + iDropRate) * 0.38f);
    else
        iDrop_VAR = (int)((::Get_WorldDROP() + NPC_DROP_ITEM(pMobCHAR->Get_CharNO()) - (1 + RANDOM(100)) - ((iLevelDiff + 20) * 5.5f) - 10 + iDropRate) * 0.23f);
#else
    iDrop_VAR = (int)((::Get_WorldDROP() + NPC_DROP_ITEM(pMobCHAR->Get_CharNO()) - (1 + RANDOM(100)) - ((iLevelDiff + 16) * 3.5f) - 10 + iDropRate) * 0.38f);
#endif
```

### FRAROSE Build Option Effects

**Symptoms:** (Confirmed)
- **Buffer overflow causing character name corruption**
- **Potential packet structure problems** (opcode/packet size changes)
- **Mount riding synchronization issues** (when mounts are eventually implemented)
- **EXP/drop boost effects not properly applied** (requires STB/STL integration)

**Analysis:**
- **FRAROSE Implementation**: **CAUSES BUFFER OVERFLOW** - Character name corruption and potential packet structure problems
- **Mount System**: Code supports 20 mount types but **requires new STB/STL files** - current LIST_PAT.STB does not contain mount data
- **EXP/Drop Boosts**: Server has GetEXPBoost() and GetDropBoost() functions, but **asset integration required** for proper functionality
- **Ride Attributes**: m_btRideATTR expansion from BYTE to DWORD is implemented but **causes buffer overflow**
- **Database Impact**: No schema changes required, uses existing maintain status system
- **STB Compatibility**: **REQUIRES NEW STB/STL FILES** - no FRAROSE-specific files currently exist
- **Asset Status**: Mount 3D models available but need proper STB/STL integration
- **Runtime Issues**: **CONFIRMED BUFFER OVERFLOW** - Character names corrupted when FRAROSE enabled

**Code Evidence:**
```cpp
// Client: FRAROSE enables _GBC for extended ride mechanics
#ifdef FRAROSE
#define _GBC
#endif

// Server: FRAROSE adds boost functions to tagGrowAbility
#ifdef FRAROSE
short GetEXPBoost  (DWORD dwCurAbsSEC) {
    // Returns 1-3x EXP multiplier - BUT CAUSES BUFFER OVERFLOW
}
short GetDropBoost (DWORD dwCurAbsSEC) {
    // Returns 1-3x drop multiplier - BUT CAUSES BUFFER OVERFLOW
}
#endif
```

**Status:** **FRAROSE CAUSES BUFFER OVERFLOW AND IS NOT SAFE TO ENABLE** - Requires complete STB/STL file integration for mounts before it can be safely used. Character name corruption and potential packet issues make it unusable in current state.

## Investigation Areas

### Math Discrepancies Investigation

Need to compare calculation formulas between:
- Client-side calculations in `src/client/`
- Server-side calculations in `src/sho_gs/`
- Common calculation libraries

**Key Files to Compare:**
- `src/client/Common/Calculation.cpp`
- `src/common/Calculation.cpp`
- `src/sho_gs/SHO_GS_LIB/Common/Calculation.cpp`

### Packet Handling Investigation

Examine packet processing for:
- Health update packets
- EXP gain packets
- Combat result packets

**Key Areas:**
- IOCP network code
- Packet opcode handlers
- Serialization/deserialization functions

### STB File Investigation

Check STB file reading for:
- Column index correctness
- Data type consistency
- File format validation

**Key Files:**
- `IO_STB.*` files
- STB loading functions
- Data validation code

### Database Investigation

Verify database operations for:
- Field size compatibility
- Data type matching
- Stored procedure correctness

**Key Areas:**
- ODBC connection code
- Database schema files
- Data persistence logic

## Debugging Approach

### Tools Available
- **VSCode debugger**: Primary debugging environment
- **Wireshark**: Network packet analysis
- **SQL Server Profiler**: Database query monitoring
- **Client console logging**: Currently logs movement speed
- **Server debug windows**: Currently logs connections

### Testing Methodology
1. Enable detailed logging with `_FORCE_LOG` define
2. Monitor network traffic with Wireshark
3. Compare client vs server calculation results
4. Validate database operations
5. Test edge cases (high levels, complex combat)

### Validation Steps
1. **Health sync testing**: Monitor health values during combat
2. **EXP testing**: Track EXP gain/loss across multiple scenarios
3. **Visual testing**: Check for buffer overflow artifacts
4. **Network testing**: Validate packet integrity and timing

## Code Examples

### Health Update Packet Structure
```cpp
// Example packet structure for health updates
struct t_packet_health {
    WORD wType;        // Packet opcode
    WORD wSize;        // Packet size
    DWORD dwObjectID;  // Character object ID
    long lCurrentHP;   // Current health value
    long lMaxHP;       // Maximum health value
};
```

### EXP Calculation Example
```cpp
// Example EXP calculation (may differ between client/server)
DWORD CalculateEXP(DWORD dwMonsterLevel, DWORD dwPlayerLevel, DWORD dwBaseEXP) {
    // Formula may have discrepancies
    return dwBaseEXP * (dwMonsterLevel - dwPlayerLevel + 10) / 10;
}
```

## Mitigation Strategies

### Immediate Fixes
1. **Logging enhancement**: Add detailed sync logging
2. **Validation checks**: Add client/server state validation
3. **Packet verification**: Implement packet checksums
4. **Bounds checking**: Add array bounds validation
5. **FRAROSE avoidance**: Do not enable FRAROSE until buffer overflow issues are resolved and STB/STL files are integrated

### Long-term Solutions
1. **Unified calculations**: Move all calculations to server-side
2. **State synchronization**: Implement periodic state sync packets
3. **Error correction**: Add automatic desync detection and correction
4. **Testing framework**: Implement automated sync testing

## Related Files

- `src/common/datatype.h` - Core data type definitions
- `src/sho_gs/SHO_GS_LIB/LIB_gsMAIN.cpp` - Game server entry point
- `database/sho.sql` - Main game database schema
- `A/opcodes.md` - Network protocol documentation
- `A/file-duplicates.md` - Duplicate file synchronization requirements

## Status

This document is under development. Specific code locations, line numbers, and detailed analysis will be added as investigations progress.</content>
<parameter name="filePath">d:\TestSefy2025\A\synchronization-issues.md