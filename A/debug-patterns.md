# Debug Patterns and Logging Systems

This document describes the debug logging and monitoring systems used in the Rose Online codebase.

## Client Debug Systems

### Console Logging
- **Location**: Client executable (`trose.exe`)
- **Current Output**: Raw player movement speed
- **Access**: Console window attached to client
- **Purpose**: Real-time player state monitoring

### Debug Build Warnings
- **Important**: Never run `trose.exe` in debug build due to unreliable asserts
- **Reason**: Many asserts may be "faked" for compilation or serve as breakpoints only

## Server Debug Systems

### Debug Windows
- **Location**: All three servers (LS, WS, GS)
- **Current Output**: Connection logging and data monitoring
- **Access**: Server console windows
- **Purpose**: Real-time server state and connection monitoring

### Logging Levels
- **_FORCE_LOG**: Forces logging in release builds
- **_D_RELEASE**: Enables debug features in release builds
- **Standard Debug**: Only active in debug builds

## Network Debugging

### Wireshark Integration
- **Tool**: Wireshark network analyzer
- **Purpose**: Packet capture and protocol analysis
- **Usage**: Monitor opcode traffic between client and servers
- **Reference**: Use `A/opcodes.md` for packet structure reference

## Debug Code Patterns

### Conditional Debug Blocks
```cpp
#if defined(_DEBUG) || defined(_D_RELEASE) || defined(_FORCE_LOG)
// Debug logging code here
#endif
```

### Assert Usage
- **Caution**: Many existing asserts are unreliable
- **Purpose**: May serve as breakpoints rather than actual error checking
- **Recommendation**: Do not rely on assert failures for error detection

## Current Synchronization Issues

### Known Debug Targets
- **Health Desync**: Client and server health values out of sync
- **EXP Issues**: Experience gain problems in late game
- **Visual Glitches**: Buffer overflow related rendering issues

### Debug Monitoring Points
- Client console for movement speed
- Server debug windows for connections
- Wireshark for packet validation
- Database logs for transaction integrity

## Debug Build Configuration

### VSCode Setup
- **Primary Debugger**: VSCode with configured tasks.json/launch.json
- **Working Directory**: Set to `game/` folder for client debugging
- **Server Debugging**: Attach to running server processes

### Build Considerations
- **Release Only**: Production code runs in release configuration
- **Debug Builds**: Used only for development, not production
- **Assert Handling**: Debug builds may have different assert behavior

## Logging Best Practices

### Code Comments
- **Format**: Append `JDOEBOY` to any added/modified comments
- **Language**: English only for new comments
- **Translation**: Korean text translated to English only in edited files

### Incremental Changes
- **Small Steps**: Work in small increments to avoid token limits
- **Validation**: Test each change before proceeding
- **Documentation**: Document debug findings in appropriate files

## Debug File Locations

### Client Debug Files
- `src/client/Util/classLOG.h` - Logging system header
- `src/client/System/CGame.cpp` - Game state debugging
- `src/client/CKeyboard.cpp` - Input debugging

### Server Debug Files
- `src/sho_gs/SHO_GS_LIB/stdAFX.h` - Game server debug defines
- `src/sho_ws/SHO_WS_LIB/StdAfx.h` - World server debug defines
- Various GS_ThreadSQL.cpp files - Database operation logging

## Troubleshooting Common Issues

### Connection Problems
- Check server debug windows for connection attempts
- Use Wireshark to verify packet flow
- Validate ODBC connections and database access

### Synchronization Bugs
- Monitor client console for state changes
- Check server logs for state update transmissions
- Compare client and server state values

### Performance Issues
- Enable _FORCE_LOG for detailed operation logging
- Monitor server debug windows for performance metrics
- Use Wireshark to identify packet bottlenecks