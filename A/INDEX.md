# Documentation Index

This index provides navigation to detailed documentation files for the Rose Online codebase.

## Core Documentation

- **[copilot-instructions.md](../.github/copilot-instructions.md)** - Main AI coding guidelines and workflow
- **[opcodes.md](opcodes.md)** - Network protocol opcodes (may be incomplete/inaccurate)
- **[History.md](../History.md)** - Change history and tracking

## Build & Configuration

- **[build-options.md](build-options.md)** - Build preprocessors, defines, and their effects on assets/database
- **[debug-patterns.md](debug-patterns.md)** - Debug code patterns and logging systems

## Architecture & Analysis

- **[server-architecture.md](server-architecture.md)** - Detailed server communication and synchronization
- **[file-duplicates.md](file-duplicates.md)** - Mapping of duplicate essential files across the codebase
- **[synchronization-issues.md](synchronization-issues.md)** - Known sync bugs (health desync, EXP issues, buffer overflows)

## Development Guidelines

- **[coding-standards.md](coding-standards.md)** - Detailed coding conventions and patterns
- **[testing-framework.md](testing-framework.md)** - Google Test integration and testing patterns

## Asset & Data Management

- **[asset-formats.md](asset-formats.md)** - STB, STL, and other game asset formats
- **[database-schema.md](database-schema.md)** - Database structure and relationships


## Quick References

- **File Structure**: See main copilot-instructions.md for directory layout
- **Build Command**: `msbuild All.sln /p:Configuration=Release /p:Platform=x86`
- **Launch Sequence**: `1-login.bat` → `2-world.bat` → `3-game.bat` → `local.bat`
- **Current Focus**: Matching HP/MP/EXP calculations between client and server to resolve health desync issues

### HP Sync Investigation (2025-10-01, JDOEBOY)
- Floating HP bar (`CNameBox.cpp`) and Character Info box both use `Get_HP()`/`Get_MaxHP()` from character objects.
- HP values are set by server packets; client should not do its own HP math.
- Tracing all HP update paths to ensure no local prediction or math is causing desync.
- Next: Audit all packet handlers and UI update code for HP.

## Adding New Documentation

When creating new documentation files:
1. Add them to this index with appropriate categorization
2. Include code snippets, line numbers, and file links where relevant
3. Keep files in the `A/` folder for organization
4. Update History.md when making changes