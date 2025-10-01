# HP Recovery Math Discrepancy Report (JDOEBOY)

## Overview
This document compares the HP recovery logic and related constants between the client and server implementations of `CObjAI.h` and `CObjAI.cpp` in the iRose2025 codebase. The goal is to identify any discrepancies in the definition or use of `nRecoverMODE` and the HP regeneration formulas, which may cause desynchronization or inconsistent gameplay.

---

## 1. File Locations
- **Client:** `src/client/Common/CObjAI.h` and `src/client/Common/CObjAI.cpp`
- **Server:** `src/sho_gs/SHO_GS_LIB/Common/CObjAI.h` and `src/sho_gs/SHO_GS_LIB/Common/CObjAI.cpp`

---

## 2. Recovery State Constants (`nRecoverMODE`)

### Client (`CObjAI.h`):
```cpp
// Sitting on chair
#define RECOVER_STATE_SIT_ON_CHAIR      30
// Sitting on ground (old value)
#define RECOVER_STATE_SIT_ON_GROUND_OLD 20
// Sitting on ground (new value)
#define RECOVER_STATE_SIT_ON_GROUND_NEW 25
// Stopped or walking (old value)
#define RECOVER_STATE_STOP_OR_WALK_OLD   8
// Stopped or walking (new value)
#define RECOVER_STATE_STOP_OR_WALK_NEW   5
```
- Multiple values for sitting/standing, with both `_OLD` and `_NEW` variants.
- Comments in code indicate these are for different regions/versions (e.g., Taiwan, Philippines applied).

### Server (`CObjAI.h`):
```cpp
#define RECOVER_STATE_SIT_ON_CHAIR   30
#define RECOVER_STATE_SIT_ON_GROUND  20
#define RECOVER_STATE_STOP_OR_WALK    8
```
- Only one value for each state, no `_OLD`/`_NEW` variants.
- No mention of region-specific values.

**Discrepancy:**
- The client has both old and new values for ground sitting and stop/walk, while the server only uses a single value for each.
- The client may use different `nRecoverMODE` values depending on region or configuration, but the server does not.

---

## 3. HP Recovery Formula (`Get_RecoverHP`)

### Client (`CObjAI.cpp`):
```cpp
short CObjAI::Get_RecoverHP( short nRecoverMODE )
{
    short nRecoverHP = 0;
    switch ( Get_COMMAND() ) 
    {
        case CMD_SIT :
            nRecoverHP = this->GetAdd_RecoverHP() + ( this->Get_CON() + 30 ) / 8 * ( nRecoverMODE + 3 ) / 10;
            break;
        default:
            nRecoverHP = ( this->GetAdd_RecoverHP() + ( this->Get_CON() + 40 ) / 6 ) / 6;
            break;  
    }
    return nRecoverHP;
}
```

### Server (`CObjAI.h`):
```cpp
short Get_RecoverHP( short nRecoverMODE ) { return ( this->GetAdd_RecoverHP() + ( this->Get_CON()+30 ) / 8 * (nRecoverMODE+3) / 10 ); }
```
- Server uses only the 'sit' formula for all cases.
- No `switch` on command; always uses the same formula.

**Discrepancy:**
- The client distinguishes between sitting and other states, using a different formula for standing/walking.
- The server always uses the 'sit' formula, regardless of state.
- This can cause the client and server to calculate different HP recovery values for the same character state.

---

## 4. Recovery Interval Timing

### Client (`CObjAI.h`):
```cpp
#define RECOVER_STATE_CHECK_TIME_OLD 4000
#define RECOVER_STATE_CHECK_TIME_NEW 8000
```
### Server (`CObjAI.h`):
```cpp
#define RECOVER_STATE_CHECK_TIME 8000
```
- Client has both 4s and 8s intervals, server uses only 8s.

**Discrepancy:**
- If the client is configured to use the 4s interval, it will recover HP twice as often as the server expects.

---

## 5. Summary Table
| Aspect                | Client (Common/CObjAI.h)         | Server (SHO_GS_LIB/Common/CObjAI.h) |
|-----------------------|-----------------------------------|--------------------------------------|
| Sitting on Chair      | 30                                | 30                                   |
| Sitting on Ground     | 20 (old), 25 (new)                | 20                                   |
| Stop/Walk             | 8 (old), 5 (new)                  | 8                                    |
| Recovery Interval     | 4000ms (old), 8000ms (new)        | 8000ms                               |
| HP Recovery Formula   | Switch on command (sit/other)     | Always 'sit' formula                 |

---

## 6. Implications
- **Desync Risk:** If the client and server use different `nRecoverMODE` or recovery intervals, HP regeneration will not match, causing desync.
- **Math Mismatch:** The server does not distinguish between sitting and standing, but the client does. This can cause the client to show more or less HP regen than the server actually applies.
- **Configuration Drift:** The presence of `_OLD` and `_NEW` values in the client but not the server suggests region/configuration drift that is not mirrored server-side.

---

## 7. Recommendations
- **Unify Constants:** Ensure both client and server use the same set of constants for `nRecoverMODE` and recovery intervals.
- **Align Formulas:** Make sure both client and server use the same HP recovery formula for each state.
- **Audit Configurations:** Check for region-specific or legacy code paths in the client that are not present on the server.
- **Document All Differences:** Keep this document updated as changes are made.

---

## 8. Next Steps
- Review all usages of `nRecoverMODE` and recovery interval in both client and server code.
- Decide on a single set of values and formulas to use for all regions.
- Update code and documentation to reflect the unified logic.

---

*Generated by Copilot (JDOEBOY), 2025-10-01*
