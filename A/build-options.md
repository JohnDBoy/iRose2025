# Build Options and Defines

This document catalogs all known build preprocessor defines and their effects on the Rose Online codebase. These options control various game features, asset sets, and database configurations.

## Major Build Defines

### FRAROSE
- **Status**: **NOT CURRENTLY ENABLED** - Causes buffer overflow issues and requires additional STB/STL files
- **Location**: `src/common/datatype.h` line 173, `src/client/StdAfx.h`, `src/sho_gs/SHO_GS_LIB/Common\CUserDATA.h`, `src/sho_gs/SHO_GS_LIB/Common\DataTYPE.h`, `src/sho_gs/SHO_GS_LIB\CObjAVT.h`, `src/lib_util/lib_util.vcxproj`
- **Effects**:
  - **Mount System**: Adds 20 mount types (PET_TYPE_MOUNT01 through PET_TYPE_MOUNT20)
  - **EXP/Drop Boosts**: Adds AT_EXP_BOOST, AT_DROP_BOOST, AT_EXP_NONE abilities
  - **Ride Attributes**: Expands m_btRideATTR from BYTE to DWORD to accommodate mount data
  - **Client Integration**: When defined, automatically enables _GBC define for extended ride mechanics
  - **Server Support**: Game server has matching mount attribute support and boost calculation functions
  - **Database Impact**: No schema changes required - uses existing maintain status system
  - **STB Compatibility**: **REQUIRES NEW STB/STL FILES** - Current LIST_PAT.STB does not contain mount data
  - **Asset Requirements**: Mount 3D models and textures available but need proper STB/STL integration
  - **Synchronization**: Client and server implementations appear properly synchronized
  - **Known Issues**: **CAUSES BUFFER OVERFLOW** - Character name corruption and potential packet structure problems (possible opcode/packet size changes)

### __KCHS_BATTLECART__
- **Status**: Defined in world server (`src/sho_ws/SHO_WS_LIB/StdAfx.h`)
- **Location**: Various files in game server and world server
- **Effects**:
  - Changes ride part enumeration (RIDE_PART_ABIL vs RIDE_PART_ARMS)
  - Affects battle cart mechanics and data compatibility
  - Marked as `__OLD_DATA_COMPATIBLE__` in some locations

## Logging and Debug Defines

### _FORCE_LOG
- **Status**: Defined in game server and client stdafx files
- **Location**: `src/sho_gs/SHO_GS_LIB/stdAFX.h`, `src/client/StdAfx.h`
- **Effects**:
  - Forces logging output even in release builds
  - Enables debug logging in client and server components

### _D_RELEASE
- **Status**: Used in conditional compilation
- **Location**: Various client files
- **Effects**:
  - Enables debug features in release builds
  - Controls keyboard input debugging, game state logging, etc.

## Server-Specific Defines

### __SERVER
- **Status**: Automatically defined for server builds
- **Location**: Various header files
- **Effects**:
  - Changes data structures (t_POSITION typedef)
  - Excludes client-specific code and DirectX dependencies
  - Enables server-specific inventory calculations

## Asset and Data Defines

### __OLD_DATA_COMPATIBLE__
- **Status**: Used in conjunction with __KCHS_BATTLECART__
- **Location**: SQL thread files and data type headers
- **Effects**:
  - Maintains backward compatibility with older data formats
  - Affects database query structures and parameter handling

## Undocumented/Unknown Defines

The following defines appear in the codebase but their effects are not fully documented:

- Various third-party library defines (zlib, lua, etc.)
- Platform-specific defines for Windows/DirectX
- Memory management and performance defines

## Build Configuration Decisions Needed

### Current Asset Set
- **Status**: Current assets tested and working
- **Decision Needed**: Which build defines to enable for different asset configurations
- **Impact**: Different defines may load different STB files, change game mechanics

### Database Compatibility
- **Status**: Multiple database versions exist (seven_ORA, SHO, SHO_LOG, SHO_MALL)
- **Decision Needed**: Which defines affect database schema and stored procedures
- **Impact**: Wrong defines may cause data corruption or load failures

### Feature Enablement
- **Status**: Many features conditionally compiled
- **Decision Needed**: Which features to enable (mounts, battle carts, etc.)
- **Impact**: Affects gameplay balance and server performance

## Build Testing Results

### FRAROSE Enablement Testing
- **Test Date**: Current session
- **Test Method**: Full project build with FRAROSE enabled across all components
- **Results**: 
  - **Compilation**: SUCCESS (0 errors, 1683 warnings)
  - **Warnings**: Mostly deprecated function warnings (strcmpi, strcpy, sprintf) and type conversion warnings
  - **Runtime Issues**: **BUFFER OVERFLOW DETECTED** - Character name corruption and potential packet structure problems
  - **Build Time**: ~2 minutes 29 seconds
  - **Configuration**: Release build, x86 platform
- **Files Modified**:
  - `src/client/StdAfx.h` - Enabled FRAROSE define
  - `src/common/datatype.h` - FRAROSE already enabled
  - `src/sho_gs/SHO_GS_LIB/Common\CUserDATA.h` - FRAROSE already enabled
  - `src/sho_gs/SHO_GS_LIB/Common\DataTYPE.h` - Enabled FRAROSE define
  - `src/sho_gs/SHO_GS_LIB\CObjAVT.h` - Enabled FRAROSE define
  - `src/lib_util/lib_util.vcxproj` - Added FRAROSE to preprocessor definitions
- **Asset Requirements**: Mount assets available but require new STB/STL files for proper integration
- **Verification**: Database schemas examined (no FRAROSE changes needed), STB structure analyzed (missing mount data), build compilation successful but runtime issues detected

## Recommendations

1. **Document Effects**: Before enabling any define, test in a development environment
2. **Version Control**: Track which defines are active for each build configuration
3. **Asset Compatibility**: Ensure STB files match the enabled defines
4. **Database Migration**: Plan database changes when enabling new features
5. **Testing**: Each define change requires full server restart and client reconnection
6. **FRAROSE Enablement**: **NOT SAFE TO ENABLE** - Causes buffer overflow and character name corruption, requires additional STB/STL files for mounts
7. **Build Verification**: Always run full project build after define changes to catch compilation errors
8. **Warning Management**: Deprecated function warnings are normal and don't indicate breaking changes

## Files to Monitor

When changing build defines, check these files for consistency:
- `src/common/datatype.h` - Core data type definitions
- `src/sho_gs/SHO_GS_LIB/stdAFX.h` - Game server defines
- `src/sho_ws/SHO_WS_LIB/StdAfx.h` - World server defines
- `src/client/StdAfx.h` - Client defines
- All SQL thread files for database compatibility