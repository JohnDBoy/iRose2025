# Change History

This file tracks changes made to the Rose Online codebase and documentation.

## 2025-10-01 - Code Reversion and Calculation Focus Shift

### SQL Syntax Error Resolution
- **Issue**: "Incorrect syntax near '{'" and "Unclosed quotation mark after the character string ')}'" errors during client login
- **Root Cause**: Introduced by `_int64` conversions in multiple files, causing SQL parameter handling issues
- **Fix**: Reverted affected files to restore original code, resolving the SQL errors
- **Build**: Project compiles successfully in Release|x86 configuration after reversions
- **Status**: Login functionality restored

### Code Reversion and Lessons Learned
- **Reverted Changes**: Multiple files modified with `_int64` conversions were reverted due to breaking functionality
- **Performance Gain**: `_int64` changes sped up server loading significantly but stopped some functionality
- **Feature Testing**: FRAROSE does not work as-is; mounts do work; EXP/drop boosts not tested
- **Approach**: Avoided compounding errors from trying too much at once

### New Focus: HP Calculations
- **Priority**: Match HP calculations between client and server to resolve health desync issues
- **Scope**: Focus on HP/MP/EXP calculations without enabling additional features
- **Next Steps**: Investigate calculation mismatches in `Calculation.cpp` and related files

## 2025-09-30 - Synchronization Issues Documentation Started

### Documentation Updates
- **A/file-duplicates.md**: Updated with user clarifications - StdAfx.* files are project-specific precompiled headers (not problematic), do not consolidate existing duplicates, ensure copies agree, create new common files for shared data
- **A/synchronization-issues.md**: Created initial documentation framework for known sync bugs (health desync, EXP issues, buffer overflows)

### Key Insights on Duplicate Files
- **StdAfx.* files**: Project-specific precompiled headers containing defines - not problematic
- **Consolidation approach**: Do not consolidate existing duplicates (tried without success), but ensure all copies agree
- **New common files**: Can be created for shared data that needs to be moved out of existing duplicates

### Synchronization Issues Framework
- **Root causes identified**: Math discrepancies, packet handling issues, STB file column problems (possible), DB field sizes/format issues
- **Investigation areas**: Math calculations, packet processing, STB file reading, database operations
- **Debugging approach**: Enhanced logging, network monitoring, calculation comparison, database validation
- **Major findings**: Conditional compilation differences (`_PRE_EVO`) causing EXP/damage formula mismatches between client and server

## 2025-01-02 - Duplicate Files Documentation

### Documentation Created
- **A/file-duplicates.md**: Comprehensive mapping of all 78 duplicate file groups across the src/ directory
- **Updated .github/copilot-instructions.md**: Added reference to file-duplicates.md for synchronization guidelines

### Duplicate Files Analysis
- **Total duplicate groups**: 78 files with multiple copies
- **Most duplicated**: StdAfx.h (13 copies), StdAfx.cpp (12 copies), resource.h (6 copies)
- **Critical files identified**: DataTYPE.h, Calculation.*, CInventory.*, CItem.*, AI system files
- **Synchronization patterns documented**: Client ↔ Server shared code, Common ↔ Game Server utilities
- **Key insights**: StdAfx.* files are project-specific precompiled headers (not problematic), do not consolidate existing duplicates (tried without success), ensure copies agree, create new common files for shared data

### Key Findings
- Essential files appear in 2-4 locations across different components
- Core data structures (Calculation, CInventory, CItem) duplicated in client/Common/, common/, and sho_gs/SHO_GS_LIB/Common/
- Chat system files (CChatRoom.*) appear in 4 locations
- AI system files duplicated between client and server libraries
- Build system files (StdAfx.*, resource.h) duplicated across all major components

### Synchronization Guidelines Established
- **Critical update order**: Core data structures → Shared utilities → Communication systems → Game logic
- **Risk assessment**: High risk for DataTYPE.h, Calculation.*, I/O systems
- **Maintenance recommendations**: Consolidate shared code, automated synchronization scripts, version control strategy

## 2025-09-30 - Initial Documentation Setup

### Documentation Created
- **.github/copilot-instructions.md**: Main AI coding guidelines with architecture overview, build workflows, and coding patterns
- **A/INDEX.md**: Documentation index for navigation and organization
- **A/build-options.md**: Comprehensive catalog of build preprocessor defines and their effects
- **A/debug-patterns.md**: Documentation of debug logging systems and monitoring patterns

### Key Guidelines Established
- Release builds only, no debug builds for production
- VSCode debugging with tasks.json/launch.json
- English comments with JDOEBOY suffix
- Korean text translation only in edited files
- Small incremental changes to avoid token limits
- Thorough documentation with code snippets and file links

### Build Process Confirmed
- `msbuild All.sln /p:Configuration=Release /p:Platform=x86`
- Third party libraries already built
- Use .bat files for launching to ensure latest builds

### Development Environment
- VSCode debugger primary tool
- Wireshark for network debugging
- Google Test available for unit tests
- Doxygen available (mostly Korean documentation)

### Build Options Documented
- **FRAROSE**: Enables mount system and EXP/drop boosts
- **__KCHS_BATTLECART__**: Affects battle cart mechanics and data compatibility
- **_FORCE_LOG**: Forces logging in release builds
- **_D_RELEASE**: Enables debug features in release builds
- **__SERVER**: Server-specific compilation flags

### Debug Systems Documented
- Client console logging (currently movement speed)
- Server debug windows (currently connections and data)
- Wireshark network debugging
- Assert reliability warnings

## Future Changes

### Planned Documentation
- [ ] A/synchronization-issues.md - Document known sync bugs (health desync, EXP issues, buffer overflows)
- [x] A/file-duplicates.md - Map duplicate essential files across codebase
- [ ] A/testing-framework.md - Google Test integration patterns
- [ ] A/asset-formats.md - STB, STL, and other game asset formats

### Known Issues to Address
- Health desync between client and servers
- EXP issues in late game
- Visual glitches from buffer overflows
- [x] Multiple copies of essential files need synchronization (documented in A/file-duplicates.md)

### Build Options to Evaluate
- Need to decide which defines to enable for different asset configurations
- Document effects on database schema and stored procedures
- Test feature enablement (mounts, battle carts, etc.)