# Rose Online Server Copilot Instructions

## Architecture Overview

This is a **Rose Online MMORPG** codebase implementing a distributed server architecture:

- **SHO_LS** (Login Server): Handles authentication, account management
- **SHO_WS** (World Server): Manages character data, world state, inter-server communication
- **SHO_GS** (Game Server): Runs game logic, zones, combat, NPC AI
- **Client**: DirectX-based game client with 3D rendering

**Key Communication Patterns:**
- Servers communicate via TCP using custom binary protocol with opcodes (see `A/opcodes.md`)
- IOCP (I/O Completion Ports) for high-performance async networking
- SQL Server backend with ODBC connections for persistence

## Development Environment & Workflow

### Build Configuration
- **Release builds only**: We work exclusively with Release configurations
- **Build command**: `msbuild All.sln /p:Configuration=Release /p:Platform=x86`
- **Third party libraries**: Already built and do not need rebuilding
- **Individual project builds**: Can be built through All.sln as needed

### Launch Process
- **Always use .bat files**: `1-login.bat`, `2-world.bat`, `3-game.bat`, `local.bat` ensure latest builds
- **Server startup sequence**: Login → World → Game servers with proper timing

### Debugging Setup
- **VSCode debugger**: Primary debugging environment with `tasks.json` and `launch.json` configured
- **Client debugging**: Console added for debugging (currently logs raw movement speed)
- **Server debugging**: Debug windows in all servers (currently log connections and other data)
- **Wireshark**: Installed locally for network debugging

### Testing Framework
- **Google Test**: Installed and available for adding unit tests as needed

## Build System & Development Workflow

### Prerequisites Setup
```bash
# Required: Visual Studio 2019+ with Desktop C++ workload
# Individual components needed:
# - C++ MFC latest v142 build tools (x86 & x64)
# - MSVC v142 - VS 2017 C++ x64/x86 build tools
# - C++ ATL for v142 build tools with Spectre Mitigations
# - Windows 10 SDK (10.0.18362.0)
```

### Build Process
```bash
# Build main solution (thirdparty already built)
msbuild All.sln /p:Configuration=Release /p:Platform=x86

# Output: bin/release/ contains DLLs and executables
```

### Database Setup
```sql
-- Create 4 databases: seven_ORA, SHO, SHO_LOG, SHO_MALL
-- Run SQL scripts from database/ directory
-- Setup ODBC connections matching server config files
-- Default user: 'sho' (password: 'sho' for dev)
```

## Coding Patterns & Conventions

### Naming Conventions
- **Hungarian notation**: `CClassName`, `m_memberVar`, `pPointer`, `btByte`, `wWord`, `dwDword`
- **Global variables**: `g_VariableName` (avoid when possible)
- **File prefixes**: `@` for server-specific, `~` for backup/inactive files

### Code Comments & Documentation
- **English comments only**: All added comments must be in English
- **Comment format**: Append `JDOEBOY` to any comments we add/modify
- **Korean text translation**: Translate Korean dialog/output strings to English ONLY in files being edited
- **Doxygen**: Available locally, mostly Korean documentation exists

### Development Practices
- **Small incremental changes**: Work in small steps to avoid token limits
- **Follow existing patterns**: Do not modernize code, match existing style
- **Production code**: No stubs or example code, all changes must work
- **Synchronization issues**: Currently debugging health desync, EXP issues, visual glitches from buffer overflows

### Network Protocol
```cpp
// Packet structure example
struct t_PACKETHEADER {
    WORD  m_wType;     // Opcode
    WORD  m_wSize;     // Total packet size
};

// Usage in packet handlers
case 0x701: // SRV_ANNOUNCE_TEXT
    // Read packet data based on opcode
    break;
```

### Memory Management
```cpp
// Common patterns
#define SAFE_DELETE(p)      { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p){ if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)     { if(p) { (p)->Release(); (p)=NULL; } }

// IOCP buffer allocation
classDLLNODE<tagIO_DATA> *node = new classDLLNODE<tagIO_DATA>();
::ZeroMemory(&node->DATA.m_Overlapped, sizeof(OVERLAPPED));
```

### Database Integration
```cpp
// ODBC connection pattern
SQLHDBC hDBC = GetDBConnection();
SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt);

// Query execution with error handling
SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR*)query, SQL_NTS);
if (ret != SQL_SUCCESS) {
    LogError("DB query failed");
}
```

### Game Data Structures
```cpp
// STB (String Table Binary) files for game data
STBDATA g_TblWEAPON;    // Weapon definitions
STBDATA g_TblNPC;       // NPC templates
STBDATA g_TblZONE;      // Zone/map data

// Character data
CObjAVT* pAvatar = new CObjAVT();
pAvatar->Set_NAME(charName);
```

### Threading & Synchronization
```cpp
// Critical sections for thread safety
::InitializeCriticalSection(&m_csList);
::EnterCriticalSection(&m_csList);
// ... thread-safe operations ...
::LeaveCriticalSection(&m_csList);
```

## Important Notes

### Build Preprocessors & Defines
- Many unused `#define` and preprocessor options exist
- These may be important for different asset sets and database configurations
- Need to document and decide which to use (see `A/build-options.md`)
- **FRAROSE**: Mount/riding system and EXP/drop boosts preprocessor define - **NOT CURRENTLY USED**
  - All `#define FRAROSE` statements are commented out in header files
  - Conditional `#ifdef FRAROSE` blocks exist but are inactive
  - No FRAROSE definitions in project files, solution files, or build scripts
  - No FRAROSE environment variables set
  - Mount system, riding mechanics, and boost features are disabled in current build
  - **Known Issues**: When enabled, causes buffer overflow and character name corruption
  - **Asset Requirements**: Requires new STB/STL files for mount data (assets available but need integration)

### Asserts & Debug Code
- Many asserts exist but may be unreliable or just for breakpoints
- Do not trust existing asserts - some may be "faked" for compilation
- Never run `trose.exe` in debug build due to assert issues

### File Duplication
- Multiple copies of essential files from different developers
- All copies must agree - changes need to be synchronized across duplicates
- **Critical files**: DataTYPE.h, Calculation.*, CInventory.*, CItem.*, AI system files
- **See `A/file-duplicates.md`** for complete mapping of all duplicate files and synchronization guidelines

### Documentation & Knowledge Base
- Thorough documentation required with code snippets, line numbers, file links
- Store detailed docs in `A/` folder with `INDEX.md` for navigation
- Use `History.md` for change tracking

## Common Development Tasks

### Adding New Opcodes
1. Define opcode constant in header files
2. Add handler case in packet processing functions
3. Update `A/opcodes.md` documentation (may be incomplete/inaccurate)
4. Test with Wireshark packet capture

### Database Schema Changes
1. Update SQL scripts in `database/` directory
2. Modify ODBC connection strings if needed
3. Update server config files (SHO_*.ini)
4. Test with SQL Server Management Studio

### Adding Game Features
1. Define data structures in appropriate headers
2. Implement logic in server libraries (sho_*_lib)
3. Update client if UI/network changes needed
4. Add database tables/migrations if required

## Debugging & Troubleshooting

### Common Issues
- **ODBC connection failures**: Verify SQL Server TCP enabled, port 1433 open
- **Build errors**: Check include paths, ensure Release configuration
- **Runtime crashes**: Check STB file loading, validate packet sizes
- **Client connection issues**: Verify server startup order, check firewall
- **Synchronization bugs**: Health desync, EXP issues, buffer overflow visual glitches, UI display inconsistencies (HP bars, zone change artifacts, potion effects)

### Debug Configuration
- Use VSCode debugger with configured tasks.json/launch.json
- Client debugging: Console logs raw movement speed
- Server debugging: Debug windows log connections and data
- Network debugging: Use Wireshark with opcode reference

## File Organization Reference

```
src/
├── common/           # Shared utilities, networking, data structures
├── client/           # Game client source
├── engine/           # 3D rendering engine
├── lib_util/         # Utility library
├── lib_server/      # Server common library
├── sho_ls/           # Login server
├── sho_ws/           # World server
├── sho_gs/           # Game server
└── [other]/          # Game-specific components

A/                    # Documentation and detailed analysis files
database/             # SQL setup scripts
thirdparty/           # External dependencies (already built)
bin/release/          # Build outputs
server/               # Server executables and configs
game/                 # Client files and assets
```

## Key Files for Understanding Architecture

- `All.sln` - Main Visual Studio solution
- `README.md` - Setup and build instructions
- `A/opcodes.md` - Network protocol documentation (may be incomplete)
- `A/INDEX.md` - Documentation index and navigation
- `History.md` - Change history and tracking
- `src/common/datatype.h` - Core data type definitions
- `src/sho_gs/SHO_GS_LIB/LIB_gsMAIN.cpp` - Game server entry point
- `database/sho.sql` - Main game database schema

## Client UI Layout


The Rose Online client interface consists of the following key UI elements:


1. **Upper Left - Character Information Box**: Displays player's name, HP (Health), MP (Magic Health), and EXP (Experience) bars
2. **Directly Under Character Information Box - Regen/Status Icons**: Small icons appear here to indicate when HP or MP is increasing (e.g., from regeneration, food, or potion). These icons are visually similar to the buff/debuff icons but are shown in a separate row directly beneath the Character Information box, not in the main buff area. They provide immediate feedback that HP/MP is actively being restored.
3. **Right of Character Info - Status Area**: Shows game hints, stat icons (buffs/debuffs), and fuel gauge
4. **Center - Character Viewport**: Main 3D view where the character model is rendered with attached name tag and health bar above the model
5. **Bottom Left - Chat Window**: Chat interface and system message output area
6. **Bottom of Character Info - Menu Button**: Opens menu with access to various client UI boxes, screenshot button, and unknown UI elements
7. **Upper Right - Mini Map**: Small map showing current zone layout
8. **Right Click - Camera Control**: Right-clicking allows camera angle manipulation
9. **Mouse Wheel - Zoom**: Scrolling zooms the camera view in and out
10. **Party Window**: Floating window that appears during party play, showing all party members' names, health bars, and buff status


**Additional UI Windows**: Character, Item, Skill, Quest, Community, Clan, Help, Info, Options, Exit

**UI Framework**: Handled in source code, possibly using tgamectrl (locations not fully documented)

**Debugging Access**: Can add outputs to client chat window, server debug windows, etc. through source code modifications

### Current UI Issues (without FRAROSE enabled)
1. **Client HP desync**: Client HP values do not match server HP values
2. **Zone change EXP overflow**: When changing zones, experience from Player Information box overflows to character's floating health bar briefly
3. **Health potion display**: When consuming HP potions, health bar does not properly represent current player health</content>

<parameter name="filePath">d:\TestSefy2025\.github\copilot-instructions.md