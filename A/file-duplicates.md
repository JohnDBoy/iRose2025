# Duplicate Essential Files Mapping

This document maps all duplicate files across the D:\TestSefy2025\src codebase. These duplicates were created by different developers working on various components and need to be kept synchronized to maintain consistency.

## Overview

- **Total duplicate file groups**: 78
- **Most common duplicates**: StdAfx.h (13 copies), StdAfx.cpp (12 copies), resource.h (6 copies)
- **Primary duplication patterns**:
  - Client ↔ Server shared code (Calculation, CInventory, CItem, etc.)
  - Common ↔ Game Server shared utilities
  - Client AI ↔ Server AI libraries
  - Utility libraries across multiple components

**Important Notes:**
- **StdAfx.* files**: These are precompiled headers specific to different projects and are not problematic - they contain project-specific defines and configurations
- **Consolidation approach**: Do not consolidate existing duplicates (tried without success previously). Instead, ensure all copies agree, and create new common files for shared data when needed

## Critical Files Requiring Synchronization

### Core Data Structures (3 copies each)
| File | Locations |
|------|-----------|
| `Calculation.cpp/h` | `client\Common\`, `common\`, `sho_gs\SHO_GS_LIB\Common\` |
| `CInventory.cpp/h` | `client\Common\`, `common\`, `sho_gs\SHO_GS_LIB\Common\` |
| `CItem.cpp/h` | `client\Common\`, `common\`, `sho_gs\SHO_GS_LIB\Common\` |
| `DataTYPE.h` | `client\Common\`, `common\`, `sho_gs\SHO_GS_LIB\Common\` |
| `IO_PAT.cpp/H` | `client\Common\`, `common\`, `sho_gs\SHO_GS_LIB\Common\` |
| `IO_Skill.cpp/h` | `client\Common\`, `common\`, `sho_gs\SHO_GS_LIB\Common\` |
| `IO_STB.cpp` | `client\Common\`, `common\`, `sho_gs\SHO_GS_LIB\Common\` |

### Chat System (4 copies each)
| File | Locations |
|------|-----------|
| `CChatRoom.cpp/h` | `client\GameData\`, `common\`, `sho_gs\SHO_GS_LIB\`, `sho_gs\SHO_GS_LIB\srv_COMMON\` |

### Quest System (3 copies)
| File | Locations |
|------|-----------|
| `CQuest.h` | `client\Common\`, `common\`, `sho_gs\SHO_GS_LIB\Common\` |

### AI System (2 copies each)
| File | Locations |
|------|-----------|
| `AI_Action.cpp` | `client\AI_LIB\`, `sho_gs\SHO_GS_LIB\Ai_lib\` |
| `AI_Condition.cpp` | `client\AI_LIB\`, `sho_gs\SHO_GS_LIB\Ai_lib\` |
| `CAI_FILE.CPP/H` | `client\AI_LIB\`, `sho_gs\SHO_GS_LIB\Ai_lib\` |
| `CAI_LIB.H` | `client\AI_LIB\`, `sho_gs\SHO_GS_LIB\Ai_lib\` |

### Utility Libraries (2-4 copies each)
| File | Locations |
|------|-----------|
| `classCRC.cpp/h` | `client\Util\`, `lib_util\`, `triggervfs\` (4 copies) |
| `classTIME.cpp/h` | `client\Util\`, `lib_util\` |
| `CDSound.cpp` | `client\Util\`, `lib_util\` |
| `CSocket.cpp` | `client\Util\`, `lib_util\` |
| `CRawSOCKET.cpp` | `client\Util\`, `lib_util\` |
| `CWaveFILE.cpp/h` | `client\Util\`, `lib_util\` |

### Game Object System (2 copies each)
| File | Locations |
|------|-----------|
| `CGameOBJ.cpp/h` | `client\`, `sho_gs\SHO_GS_LIB\` |
| `CObjCHAR.cpp/h` | `client\`, `sho_gs\SHO_GS_LIB\` |
| `CObjAI.cpp/h` | `client\Common\`, `sho_gs\SHO_GS_LIB\Common\` |
| `OBJECT.cpp/h` | `client\`, `sho_gs\SHO_GS_LIB\` (OBJECT.h has 3 copies) |

### Economy & Hotbar System (2 copies each)
| File | Locations |
|------|-----------|
| `CEconomy.cpp/h` | `client\Common\`, `sho_gs\SHO_GS_LIB\Common\` |
| `CHotICON.cpp/h` | `client\Common\`, `sho_gs\SHO_GS_LIB\Common\` |

### User Data Management (2 copies each)
| File | Locations |
|------|-----------|
| `CUserDATA.cpp/h` | `client\Common\`, `sho_gs\SHO_GS_LIB\Common\` |

### I/O Systems (2-3 copies each)
| File | Locations |
|------|-----------|
| `IO_AI.CPP/H` | `client\Common\`, `sho_gs\SHO_GS_LIB\Common\` |
| `IO_Motion.cpp/h` | `client\Common\`, `sho_gs\SHO_GS_LIB\Common\` |
| `IO_Quest.cpp/h` | `client\Common\`, `sho_gs\SHO_GS_LIB\Common\` |
| `io_stb.h` | `client\Common\`, `sho_gs\SHO_GS_LIB\Common\` |

### Threading & Communication (2 copies each)
| File | Locations |
|------|-----------|
| `CThreadGUILD.CPP/H` | `common\`, `sho_gs\SHO_GS_LIB\srv_COMMON\` |
| `CThreadLOG.cpp/h` | `common\`, `sho_gs\SHO_GS_LIB\srv_COMMON\` |
| `CThreadMSGR.cpp/h` | `common\`, `sho_gs\SHO_GS_LIB\srv_COMMON\` |

### Guild & Messenger System (2 copies each)
| File | Locations |
|------|-----------|
| `@CGuildLIST.CPP/H` | `common\`, `sho_gs\SHO_GS_LIB\` |
| `@CMessenger.cpp/h` | `common\`, `sho_gs\SHO_GS_LIB\` |

### Database & Socket (2 copies each)
| File | Locations |
|------|-----------|
| `CDB_Socket.cpp/h` | `common\`, `sho_gs\SHO_GS_LIB\srv_COMMON\` |

### Math & ETC (3 copies each)
| File | Locations |
|------|-----------|
| `ETC_Math.cpp/h` | `common\`, `sho_gs\SHO_GS_LIB\`, `sho_ws\SHO_WS_LIB\` |

### Regen Areas (2 copies each)
| File | Locations |
|------|-----------|
| `CRegenAREA.cpp/h` | `client\Common\`, `sho_gs\SHO_GS_LIB\Common\` |

## Build System Duplicates

### Standard Files (12-13 copies each)
| File | Count | Primary Locations |
|------|-------|-------------------|
| `StdAfx.cpp` | 12 | All major components (client, servers, libraries) |
| `StdAfx.h` | 13 | All major components (client, servers, libraries) |

### Resource Files (6 copies)
| File | Count | Locations |
|------|-------|-----------|
| `resource.h` | 6 | `client\`, `engine\`, `lib_server\IOCP\*`, `tgamectrl\*`, `triggerinfo\*` |

### DLL Conversion Scripts (3 copies)
| File | Locations |
|------|-----------|
| `dll2lib.bat` | `sho_gs\`, `sho_ls\`, `sho_ws\` |

## TriggerVFS System (2 copies each)
| File | Locations |
|------|-----------|
| `BlankInfo.h` | `triggervfs\include\triggervfs\`, `triggervfs\src\` |
| `DllMain.h` | `triggervfs\include\triggervfs\`, `triggervfs\src\` |
| `ErrorEtc.h` | `triggervfs\include\triggervfs\`, `triggervfs\src\` |
| `GlobalData.h` | `triggervfs\include\triggervfs\`, `triggervfs\src\` |
| `libCrc.h` | `triggervfs\include\triggervfs\`, `triggervfs\src\` |
| `libFile.h` | `triggervfs\include\triggervfs\`, `triggervfs\src\` |
| `libString.h` | `triggervfs\include\triggervfs\`, `triggervfs\src\` |
| `Macro.h` | `triggervfs\include\triggervfs\`, `triggervfs\src\` |
| `TriggerVFS.h` | `triggervfs\include\triggervfs\`, `triggervfs\src\` |
| `VFS_Manager.h` | `triggervfs\include\triggervfs\`, `triggervfs\src\` |
| `VFS.h` | `triggervfs\include\triggervfs\`, `triggervfs\src\` |

## Client-Specific Duplicates

### Interface Components (2 copies each)
| File | Locations |
|------|-----------|
| `WarningDlg.cpp/h` | `client\`, `client\Interface\ExternalUI\` |
| `export_System.lua` | `client\`, `client\System\` |

### Cursor Files (2 copies each)
| File | Locations |
|------|-----------|
| `cursor1.cur` | `client\`, `client\Res\` |
| `cursor2.cur` | `client\`, `client\Res\` |

### Utility Libraries (2 copies each)
| File | Locations |
|------|-----------|
| `ijl.h` | `client\Util\`, `lib_util\` |
| `ijl15.lib` | `client\Util\`, `lib_util\` |
| `SystemInfo.h` | `client\Util\`, `triggerinfo\include\triggerinfo\` |
| `Util_r.lib` | `client\Util\`, `client\Util\2010\` |

## Synchronization Guidelines

### Critical Update Order
1. **Core data structures** (DataTYPE.h, Calculation.*, CInventory.*, CItem.*)
2. **Shared utilities** (IO_*.cpp/h, ETC_Math.*)
3. **Communication systems** (CThread*.cpp/h, CDB_Socket.*)
4. **Game logic** (CGameOBJ.*, CObjCHAR.*, AI system)

### Change Tracking
- Always update all copies of modified files
- Test compilation across all affected projects
- Update History.md with synchronization changes
- Verify functionality in both client and server contexts

### Risk Assessment
- **High Risk**: DataTYPE.h, Calculation.*, core I/O systems
- **Medium Risk**: AI systems, utility libraries
- **Low Risk**: Build system files (StdAfx.*), resource files

## Maintenance Recommendations

1. **Do not consolidate existing duplicates** - Previous consolidation attempts failed
2. **Ensure all copies agree** - Changes must be synchronized across all duplicate locations
3. **Create new common files** - For shared data that needs to be moved out of existing duplicates
4. **Automated synchronization** - Consider scripts to copy changes across duplicates
5. **Version control strategy** - Track changes to all duplicate groups together
6. **Testing protocol** - Validate changes across all affected components
7. **Documentation updates** - Keep this file current with any new duplicates

## Files Modified During Synchronization

When updating duplicate files, check these locations for consistency:
- All listed locations for each file group
- Include directories in build paths
- Precompiled header dependencies
- Project file references

This mapping ensures that changes made to any duplicate file are properly synchronized across the entire codebase to maintain stability and prevent inconsistencies between client and server components.