# Packet Opcode Size Mapping Guide

This document outlines the opcodes used in the game server protocol and provides a step-by-step guide to mapping their sizes for runtime validation.

## Purpose
The goal is to build a system for validating packet sizes based on opcodes. This ensures data - 0x7e6 (CLI_CLANMARK_SET): Size = variable (min 8 bytes: t_PACKETHEADER + WORD m_wMarkCRC16; max includes BYTE m_btMARK[] for clanmark data; client sends clanmark data to set/update)
- 0x7e5 (CLI_MEMO): Size = variable (min 7 bytes: t_PACKETHEADER + BYTE m_btTYPE; for MEMO_REQ_SEND: + strlen(targetName) + 1 + strlen(memoContent) + 1; client sends memo requests)
- 0x7e5 (WSV_MEMO): Size = variable (for MEMO_REPLY_RECEIVED_CNT: 8 bytes t_PACKETHEADER + BYTE + short; for MEMO_REPLY_CONTENTS: variable with appended date, sender, content; world server sends memo responses)ntegrity, prevents errors from malformed packets, and helps debug protocol issues in the game server (e.g., SHO_GS, SHO_LS, SHO_WS).

## Instructions for Contributors (AI and Human)
- **Accuracy Requirement**: This document must be 100% accurate and serve as the Source of Truth. All information must be derived from the source code in `D:\TestSefy2025\src\*`. Cross-reference ALL relevant files, as there may be multiple copies used by different project parts.
- **Workflow**: Work with ONE opcode at a time. After processing each opcode, provide a summary of changes or findings.
- **Documentation Expansion**: For opcodes requiring extensive documentation (e.g., complex structures), create links from this file to new dedicated files (e.g., `opcodes/0x7B2_details.md`) for detailed opcode information.
- **Preserve Opcode List**: The full opcode list below must remain in this file for documentation purposes. Do not remove it.
- **Build/Environment Questions**: If any build, environment, or tooling questions arise during work, ASK THE USER immediately.
- **Validation**: After each edit, ensure the document remains parseable and accurate.
- **Summary per Opcode**: When documenting an opcode, include a brief summary in the Packet Size Mapping section, noting key findings (e.g., size, implementation status, source references).

## How to Use This Document
1. **Understand the Protocol**: Opcodes are categorized by sender/receiver (e.g., CLI for client-to-server, GSV for game server to client).
2. **Build a PacketInfo Table**: Create a struct or map with opcode, name, minSize, and maxSize (for variable packets).
3. **Determine Sizes**: Analyze each packet's structure (e.g., shorts, ints) to calculate expected sizes. Use the example below as a template.
4. **Add Validation**: Before sending/receiving, check if packet.size matches expectedSize. Log errors if not.
5. **Reference the Opcode List**: The list below includes all known opcodes with their names.

Step-by-step recipe to map sizes

1. Build a table from your list:

struct PacketInfo {
    uint16_t opcode;
    const char* name;
    int minSize;
    int maxSize; // if variable
};

2. Start filling it in:

case 0x7B2: // GSV_SELF_SKILL
    short skillId = readShort();
    int targetId  = readInt();
    break;


3. Add runtime validation:
Before sending or after receiving:

if (packet.size != expectedSize) {
    logError(opcode, packet.size, expectedSize);
}


CLI = Packets sent from the client to any server
LSV = Packets sent from the login server to the login server
WSV = Packets sent from the login server to the char server
GSV = Packets sent from the login server to the world server
(0x700) CLI_ALIVE
(0x700) SVR_ERROR
(0x701) SVR_ANNOUNCE_TEXT
(0x702) GSV_ANNOUNCE_CHAT
(0x703) CLI_ACCEPT_REQ
(0x704) CLI_CHANNEL_LIST_REQ
(0x704) SRV_CHANNEL_LIST_REPLY
(0x705) CLI_CHECK_AUTH
(0x705) SRV_CHECK_AUTH
(0x707) SRV_LOGOUT_REQ
(0x707) GSV_LOGOUT_REPLY
(0x708) SRV_LOGIN_REQ
(0x708) SRV_LOGIN_REPLY
(0x709) GSV_LOGIN_REPLY
(0x70a) SRV_SELECT_SERVER
(0x70a) LSV_SELECT_SERVER
(0x70b) CLI_JOIN_SERVER_REQ
(0x70c) SRV_JOIN_SERVER_REPLY
(0x70d) GSV_GM_COMMAND
(0x70e) GSV_SET_GLOBAL_VAR
(0x70f) GSV_SET_GLOBAL_FLAG
(0x711) WSV_MOVE_SERVER
(0x712) CLI_CHAR_LIST
(0x712) WSV_CHAR_LIST
(0x713) CLI_CREATE_CHAR
(0x713) WSV_CREATE_CHAR
(0x714) CLI_DELETE_CHAR
(0x714) WSV_DELETE_CHAR
(0x715) CLI_SELECT_CHAR
(0x715) GSV_SELECT_CHAR
(0x716) GSV_INVENTORY_DATA
(0x717) GSV_SET_MONEYnINV
(0x718) GSV_SET_INV_ONLY
(0x719) GSV_SERVER_DATA
(0x71a) GSV_RELAY_REQ
(0x71a) CLI_RELAY_REPLY
(0x71b) GSV_QUEST_DATA
(0x71c) CLI_CHAR_CHANGE
(0x71c) WSV_CHAR_CHANGE
(0x71d) GSV_SET_MONEY_ONLY
(0x71e) GSV_REWARD_MONEY
(0x71f) GSV_REWARD_ITEM
(0x720) GSV_REWARD_ADD_ABILITY
(0x721) GSV_REWARD_SET_ABILITY
(0x722) CLI_LOGOUT_CANCLE
(0x723) GSV_QUEST_ONLY
(0x724) GSV_WISH_LIST
(0x730) CLI_QUEST_REQ
(0x730) GSV_QUEST_REPLY
(0x731) GSV_CHECK_NPC_EVENT
(0x751) GSV_CHEAT_CODE
(0x753) CLI_JOIN_ZONE
(0x753) GSV_JOIN_ZONE
(0x754) GSV_INIT_DATA
(0x755) CLI_REVIVE_REQ
(0x755) GSV_REVIVE_REPLY
(0x756) SET_REVIVE_POS
(0x757) CLI_SET_VAR_REQ
(0x757) GSV_SET_VAR_REPLY
(0x761) CLI_CHAR_INFO_REQ
(0x761) GSV_CHAR_INFO_REPLY
(0x762) CLI_SET_WEIGHT_RATE
(0x762) GSV_SET_WEIGHT_RATE
(0x770) GSV_ADJUST_POS
(0x771) CLI_CANTMOVE
(0x771) GSV_CANTMOVE
(0x772) CLI_ATTACK_START
(0x772) GSV_ATTACK_START
(0x773) GSV_ATTACK_STOP
(0x774) GSV_CHANGE_NPC
(0x775) CLI_SUMMON_CMD
(0x775) GSV_SUMMON_CMD
(0x781) CLI_SET_MOTION
(0x781) GSV_SET_MOTION
(0x782) CLI_TOGGLE
(0x782) GSV_TOGGLE
(0x783) CLI_CHAT
(0x783) GSV_CHAT
(0x784) CLI_WHISPER
(0x784) GSV_WHISPER
(0x785) CLI_SHOUT
(0x785) GSV_SHOUT
(0x786) CLI_PARTY_CHAT
(0x786) GSV_PARTY_CHAT
(0x787) CLI_CLAN_CHAT
(0x787) WSV_CLAN_CHAT
(0x788) CLI_ALLIED_CHAT
(0x788) GSV_ALLIED_CHAT
(0x789) CLI_ALLIED_SHOUT
(0x789) GSV_ALLIED_SHOUT
(0x790) GSV_SET_EVENT_STATUS
(0x791) GSV_NPC_CHAR
(0x792) GSV_MOB_CHAR
(0x793) GSV_AVT_CHAR
(0x794) GSV_SUB_OBJECT
(0x795) CLI_SETPOS
(0x796) CLI_STOP
(0x796) GSV_STOP
(0x797) GSV_MOVE
(0x798) CLI_ATTACK
(0x798) GSV_ATTACK
(0x799) CLI_DAMAGE
(0x799) GSV_DAMAGE
(0x79a) CLI_MOUSECMD
(0x79a) GSV_MOUSECMD
(0x79b) GSV_SETEXP
(0x79e) GSV_LEVELUP
(0x79f) CLI_HP_REQ
(0x79f) GSV_HP_REPLY
(0x7a0) GSV_SET_HPnMP
(0x7a1) CLI_STORE_TRADE_REQ
(0x7a1) GSV_STORE_TRADE_REPLY
(0x7a2) CLI_CREATE_ITEM_EXP_REQ
(0x7a3) CLI_USE_ITEM
(0x7a3) GSV_USE_ITEM
(0x7a4) CLI_DROP_ITEM
(0x7a5) CLI_EQUIP_ITEM
(0x7a5) GSV_EQUIP_ITEM
(0x7a6) GSV_ADD_FIELDITEM
(0x7a6) GSV_SUB_FIELDITEM
(0x7a7) CLI_GET_FIELDITEM_REQ
(0x7a7) GSV_GET_FIELDITEM_REPLY
(0x7a8) CLI_TELEPORT_REQ
(0x7a8) GSV_TELEPORT_REPLY
(0x7a9) CLI_USE_BPOINT_REQ
(0x7a9) GSV_USE_BPOINT_REPLY
(0x7aa) CLI_SET_HOTICON
(0x7aa) GSV_SET_HOTICON
(0x7ab) CLI_SET_BULLET
(0x7ab) GSV_SET_BULLET
(0x7ac) CLI_CHANGE_SKIN
(0x7ac) GSV_CHANGE_SKIN
(0x7ad) CLI_BANK_LIST_REQ
(0x7ad) CLI_BANK_LIST_REPLY
(0x7ae) CLI_MOVE_ITEM
(0x7ae) GSV_MOVE_ITEM
(0x7af) CLI_CREATE_ITEM_REQ
(0x7af) GSV_CREATE_ITEM_REPLY
(0x7b0) GSV_SKILL_LEARN_REPLY
(0x7b1) CLI_SKILL_LEVELUP_REQ
(0x7b1) GSV_SKILL_LEVELUP_REPLY
(0x7b2) CLI_SELF_SKILL
(0x7b2) GSV_SELF_SKILL
(0x7b3) CLI_TARGET_SKILL
(0x7b3) GSV_TARGET_SKILL
(0x7b4) CLI_POSITION_SKILL
(0x7b4) GSV_POSITION_SKILL
(0x7b5) GSV_EFFECT_OF_SKILL
(0x7b6) GSV_DAMAGE_OF_SKILL
(0x7b7) GSV_CLEAR_STATUS
(0x7b8) GSV_SPEED_CHANGED
(0x7b9) GSV_RESULT_OF_SKILL
(0x7ba) CLI_APPRAISAL_REQ
(0x7ba) GSV_APPRAISAL_REPLY
(0x7bb) GSV_SKILL_START
(0x7bc) CLI_CRAFT_ITEM_REQ
(0x7bc) GSV_CRAFT_ITEM_REPLY
(0x7bd) GSV_SKILL_CANCEL
(0x7bf) VLI_SET_WISHITEM
(0x7c0) CLI_TRADE_P2P
(0x7c0) GSV_TRADE_P2P
(0x7c1) CLI_TRADE_P2P_ITEM
(0x7c1) GSV_TRADE_P2P_ITEM
(0x7c2) CLI_P_STORE_OPEN
(0x7c2) GSV_P_STORE_OPENED
(0x7c3) CLI_P_STORE_CLOSE
(0x7c3) GSV_P_STORE_CLOSED
(0x7c4) CLI_P_STORE_LIST_REQ
(0x7c4) GSV_P_STORE_LIST_REPLY
(0x7c5) CLI_P_STORE_BUY_REQ
(0x7c6) CLI_P_STORE_SELL_REQ
(0x7c6) GSV_P_STORE_RESULT
(0x7c7) GSV_P_STORE_MONEYnINV
(0x7ca) CLI_ASSEMBLE_RIDE_ITEM
(0x7ca) GSV_ASSEMBLE_RIDE_ITEM
(0x7cb) CLI_USE_ITEM_TO_REPAIR
(0x7cb) GSV_USED_ITEM_TO_REPAIR
(0x7cd) CLI_REPAIR_FROM_NPC
(0x7cd) GSV_REPAIRED_FROM_NPC
(0x7ce) GSV_SET_ITEM_LIFE
(0x7d0) CLI_PARTY_REQ
(0x7d0) GSV_PARTY_REQ
(0x7d1) CLI_PARTY_REPLY
(0x7d1) GSV_PARTY_REPLY
(0x7d2) GSV_PARTY_MEMBER
(0x7d3) GSV_PARTY_ITEM
(0x7d4) GSV_PARTY_LEVnEXP
(0x7d5) GSV_CHANGE_OBJIDX
(0x7d6) GSV_ADD_EVENTOBJ
(0x7d7) CLI_PARTY_RULE
(0x7d7) GSV_PARTY_RULE
(0x7d8) CLI_ITEM_RESULT_REPORT
(0x7d8) GSV_ITEM_RESULT_REPORT
(0x7d9) CLI_MALL_ITEM_REQ
(0x7d9) GSV_MALL_ITEM_REPLY
(0x7da) CLI_MOVE_ZULY
(0x7da) GSV_MOVE_ZULY
(0x7db) GSV_SET_NPC_SHOW
(0x7dc) GSV_GODDNESS_MODE
(0x7dd) CLI_CART_RIDE
(0x7dd) GSV_CART_RIDE
(0x7de) GSV_BILLING_MESSAGE_EXT
(0x7df) GSV_BILLING_MESSAGE
(0x7e0) CLI_CLAN_COMMAND
(0x7e0) WSV_CLAN_COMMAND
(0x7e1) CLI_MESSENGER
(0x7e1) WSV_MESSENGER
(0x7e2) CLI_MESSENGER_CHAT
(0x7e2) WSV_MESSENGER_CHAT
(0x7e3) CLI_CHATROOM
(0x7e3) WSV_CHATROOM
(0x7e4) CLI_CHATROOM_MSG
(0x7e4) WSV_CHATROOM_MSG
(0x7e5) CLI_MEMO
(0x7e5) WSV_MEMO
(0x7e6) CLI_CLANMARK_SET
(0x7e7) CLI_CLANMARK_REQ
(0x7e7) WSV_CLANMARK_REPLY
(0x7e8) CLI_CLANMARK_REG_TIME
(0x7e8) WSV_CLANMARK_REG_TIME
(0x7e9) GSV_PATSTATE_CHANGE
(0x7ea) GSV_CHARSTATE_CHANGE
(0x7eb) CLI_SCREEN_SHOT_TIME
(0x7eb) GSV_SCREEN_SHOT_TIME
(0x808) CLI_FAKE_GAMEGUARD_REQ
(0x808) GSV_FAKE_GAMEGUARD_REPLY

## Packet Size Mapping

Based on source code analysis from `D:\TestSefy2025\src\*`:

- 0x700 (CLI_ALIVE / SRV_ERROR): Size = 6 bytes CLI (t_PACKETHEADER; client sends keep-alive packet to server to maintain connection; sent from client, received in GS_USER.cpp Recv_cli_ALIVE), 8 bytes SRV (t_PACKETHEADER + WORD m_wErrorCODE; server sends error codes to client for various failure conditions; sent from server, received in client RecvPACKET.cpp)
- 0x701 (SRV_ANNOUNCE_TEXT): Size = variable (min 6 bytes: t_PACKETHEADER; max includes null-terminated announcement text string; server sends announcement text to client for system messages or notifications; sent from server, received in client RecvPACKET.cpp)
- 0x702 (GSV_ANNOUNCE_CHAT): Size = variable (min 6 bytes: t_PACKETHEADER; max includes null-terminated chat message string; server sends announcement chat messages to client for global announcements or system chat; sent from GS_USER.cpp, received in RecvPACKET.cpp)
- 0x703 (CLI_ACCEPT_REQ): Size = 6 bytes (t_PACKETHEADER; client sends acceptance request for pending operations like party invites or trade requests; sent from client, received in RecvPACKET.cpp)
- 0x704 (CLI_CHANNEL_LIST_REQ / LSV_CHANNEL_LIST_REPLY): Size = 10 bytes CLI (t_PACKETHEADER + DWORD m_dwServerID; client requests list of available channels for a server; sent from client, received in login server), variable LSV min 11 bytes (t_PACKETHEADER + DWORD m_dwServerID + BYTE m_btChannelCNT; max includes tagCHANNEL_SRV array for channel details; login server replies with channel list including user counts and age restrictions; sent from login server, received in client)
- 0x705 (CLI_CHECK_AUTH / SRV_CHECK_AUTH): Size = 68 bytes CLI (t_PACKETHEADER + char m_szAccount[31] + char m_szPassword[31]; client sends login credentials for authentication; sent from client, received in login server), 7 bytes SRV (t_PACKETHEADER + BYTE m_btResult; login server replies with authentication result code; sent from login server, received in client)
- 0x707 (SRV_LOGOUT_REQ / GSV_LOGOUT_REPLY): Size = 7 bytes (t_PACKETHEADER + BYTE m_btLogoutTYPE for SRV; t_PACKETHEADER + BYTE m_btResult for GSV; login server requests logout from game server with logout type, game server replies with result code; sent from login server to game server, reply from game server)
- 0x708 (SRV_LOGIN_REQ / SRV_LOGIN_REPLY): Size = 72 bytes SRV (t_PACKETHEADER + char m_szAccount[31] + char m_szPassword[31] + DWORD m_dwServerID; login server sends login credentials to world server for authentication; sent from login server, received in world server), 7 bytes SRV (t_PACKETHEADER + BYTE m_btResult; world server replies with authentication result code; sent from world server, received in login server)
- 0x709 (GSV_LOGIN_REPLY): Size = 15 bytes (t_PACKETHEADER + BYTE m_btResult + DWORD m_dwServerID + DWORD m_dwUserTAG; game server replies to login server with login result, server ID, and user tag; sent from game server, received in login server)
- 0x70a (SRV_SELECT_SERVER / LSV_SELECT_SERVER): Size = 10 bytes SRV (t_PACKETHEADER + DWORD m_dwServerID; world server selects server for login; sent from world server, received in login server), 7 bytes LSV (t_PACKETHEADER + BYTE m_btResult; login server replies with selection result code; sent from login server, received in world server)
- 0x70b (CLI_JOIN_SERVER_REQ): Size = 10 bytes (t_PACKETHEADER + DWORD m_dwServerID; client requests to join a specific server; sent from client, received in login server)
- 0x70c (SRV_JOIN_SERVER_REPLY): Size = 7 bytes (t_PACKETHEADER + BYTE m_btResult; login server replies to client with join server result code; sent from login server, received in client)
- 0x70d (GSV_GM_COMMAND): Size = variable (min 9 bytes: t_PACKETHEADER + BYTE m_btCMD + WORD m_wGMObjIDX; max includes null-terminated command text string; game server sends GM command to client for execution; sent from game server, received in client)
- 0x70e (GSV_SET_GLOBAL_VAR): Size = 11 bytes (t_PACKETHEADER + BYTE m_btVarTYPE + int m_iValue; game server sets global variable value for client; sent from game server, received in client)
- 0x70f (GSV_SET_GLOBAL_FLAG): Size = 11 bytes (t_PACKETHEADER + BYTE m_btFlagTYPE + DWORD m_dwFlagVALUE; game server sets global flag value for client; sent from game server, received in client)
- 0x711 (WSV_MOVE_SERVER): Size = variable min 16 bytes (t_PACKETHEADER + WORD m_wPortNO + WORD m_wZoneNO + DWORD m_dwGServerIP + DWORD m_dwGSID; max includes null-terminated IP string; world server notifies client to move to zone server with port, IDs, and IP address; sent from world server CWS_Client.cpp, received in client RecvPACKET.cpp)
- 0x712 (CLI_CHAR_LIST): Size = 6 bytes (t_PACKETHEADER; client requests character list from world server; sent from client SendPACKET.cpp, received in world server WS_ThreadSQL.cpp)
- 0x712 (WSV_CHAR_LIST): Size = variable min 7 bytes: t_PACKETHEADER + BYTE m_btCharCNT; max includes per-character data: null-terminated name string + tagCHARINFO (11 bytes) + tagPartITEM[10] (80 bytes); world server sends character list with race, level, job, delete timer, zone, equipped items; sent from world server WS_ThreadSQL.cpp, received in client RecvPACKET.cpp)
- 0x713 (CLI_CREATE_CHAR): Size = variable min 7 bytes: t_PACKETHEADER + BYTE m_btCharRACE + char m_cBoneSTONE + char m_cHairIDX + char m_cFaceIDX + char m_cWeaponTYPE + short m_nZoneNO; max includes null-terminated szCharName string; client sends character creation data including race, appearance options, weapon type, zone, and character name; sent from client SendPACKET.cpp, received in world server CWS_Client.cpp)
- 0x713 (WSV_CREATE_CHAR): Size = 8 bytes: t_PACKETHEADER + BYTE m_btResult + BYTE m_btIsPlatinumCHAR; world server replies with creation result code and platinum character flag; sent from world server CWS_Client.cpp, received in client RecvPACKET.cpp)
- 0x714 (CLI_DELETE_CHAR): Size = 8 bytes fixed + variable string: t_PACKETHEADER + BYTE m_btCharNO + bool m_bDelete; max includes null-terminated szCharName string; client sends character deletion request with character slot number, delete flag (true for delete, false for revive), and character name; sent from client SendPACKET.cpp, received in world server CWS_Client.cpp, processed in WS_ThreadSQL.cpp with SQL injection filtering, clan membership checks (masters cannot delete), and database update setting dwDelTIME; response WSV_DELETE_CHAR 10 bytes fixed + variable string with remaining deletion time (0 for revive, wait time for delete, 0xffffffff for clan masters)
- 0x714 (WSV_DELETE_CHAR): Size = 10 bytes fixed + variable string: t_PACKETHEADER + DWORD m_dwDelRemainTIME; max includes null-terminated szCharName string; world server replies to character deletion request with remaining deletion time and character name; sent from WS_ThreadSQL.cpp, received in client RecvPACKET.cpp)
- 0x715 (CLI_SELECT_CHAR): Size = 9 bytes fixed + variable string: t_PACKETHEADER + BYTE m_btCharNO + BYTE m_btRunMODE + BYTE m_btRideMODE; max includes null-terminated szCharName string; client sends character selection request with character slot, run mode, ride mode, and character name; sent from client SendPACKET.cpp, received in world server CWS_Client.cpp)
- 0x715 (GSV_SELECT_CHAR): Size = variable min 7 bytes: t_PACKETHEADER + BYTE m_btCharRACE + short m_nZoneNO + tPOINTF m_PosSTART + short m_nReviveZoneNO; max includes tagPartITEM[10] (80 bytes), tagBasicINFO, tagBasicAbility, tagGrowAbility, tagSkillAbility, CHotICONS, DWORD m_dwUniqueTAG, and null-terminated szUserID string; game server sends selected character data to client when entering zone; sent from game server GS_USER.cpp, received in client RecvPACKET.cpp)
- 0x716 (GSV_INVENTORY_DATA): Size = 1118 bytes: t_PACKETHEADER + __int64 m_i64Money + tagBaseITEM m_ItemLIST[138]; game server sends player's inventory data including money and all inventory items to client when entering zone; sent from game server, received in client RecvPACKET.cpp)
- 0x717 (GSV_SET_MONEYnINV): Size = variable min 15 bytes (t_PACKETHEADER + __int64 m_i64Money + BYTE m_btItemCNT; max includes tag_SET_INVITEM[m_btItemCNT] where each is BYTE m_btInvIDX + tagBaseITEM m_ITEM (9 bytes); game server sends updated money and inventory items to client after changes like buying/selling or trading; sent from game server GS_USER.cpp, received in client RecvPACKET.cpp)
- 0x718 (GSV_SET_INV_ONLY): Size = variable min 7 bytes (t_PACKETHEADER + BYTE m_btItemCNT; max includes tag_SET_INVITEM[m_btItemCNT] where each is BYTE m_btInvIDX + tagBaseITEM m_ITEM (9 bytes); game server sends updated inventory items to client when inventory changes like equipping/unequipping items or item consumption; sent from game server GS_USER.cpp, received in client RecvPACKET.cpp)
- 0x719: GSV_SERVER_DATA (variable min 7 bytes: t_PACKETHEADER + BYTE m_btDataTYPE; max includes appended data based on m_btDataTYPE like SERVER_DATA_NPC or SERVER_DATA_ECONOMY; game server sends server-specific data to client such as NPC information or economy settings; sent from SHO_GS_LIB\CheatCMD.cpp, received in RecvPACKET.cpp)
- 0x71a: GSV_RELAY_REQ (16 bytes fixed: t_PACKETHEADER + WORD m_wRelayTYPE + union { BYTE m_pBODY[0]; struct { short m_nCallZoneNO; tPOINTF m_PosCALL; } }; game server sends relay request to client, such as recall to zone with zone number and position; sent from GS_USER.cpp Send_gsv_RELAY_REQ, received in RecvPACKET.cpp (no direct Recv_gsv_RELAY_REQ, packet forwarded via Send_PACKET))
- 0x71b: GSV_QUEST_DATA (1486 bytes fixed: t_PACKETHEADER + tagQuestData m_Quests + tagWishLIST m_WishLIST; game server sends quest data and wish list to client; sent from GS_USER.cpp Send_gsv_QUEST_DATA, received in RecvPACKET.cpp Recv_gsv_QUEST_DATA)
- 0x751: GSV_CHEAT_CODE (variable min 9 bytes: t_PACKETHEADER + WORD m_wObjectIDX + null-terminated char m_szCheatCODE[]; game server sends cheat code string to client for execution or display; sent from GS_USER.cpp Send_gsv_CHEAT_CODE, received in RecvPACKET.cpp Recv_gsv_CHEAT_CODE)
- 0x753: CLI_JOIN_ZONE / GSV_JOIN_ZONE (9 bytes CLI, 60 bytes GSV, client sends join zone request with zone number and position, server replies with zone data including weather, time, and global variables)
- 0x761: CLI_CHAR_INFO_REQ / GSV_CHAR_INFO_REPLY (not implemented in source code, defined but commented out)
- 0x7ac (CLI_CHANGE_SKIN / GSV_CHANGE_SKIN): Size = 14 bytes (t_PACKETHEADER + WORD m_wObjectIDX + WORD m_wAbilityTYPE + int m_iValue; client requests skin change, server broadcasts skin change to nearby players with ability update; sent from GS_USER.cpp when handling skin change requests, received in RecvPACKET.cpp for client updates)
- 0x7ad (CLI_BANK_LIST_REQ / GSV_BANK_LIST_REPLY): Size = variable (CLI min 7 bytes: t_PACKETHEADER + BYTE m_btREQ; max includes null-terminated password string; GSV min 8 bytes: t_PACKETHEADER + BYTE m_btREPLY + BYTE m_btItemCNT; max includes tag_SET_INVITEM array for bank items; client requests bank access with password, server replies with bank item list or status; handled in GS_USER.cpp for bank operations, client processes in RecvPACKET.cpp)
- 0x7ae (CLI_MOVE_ITEM / GSV_MOVE_ITEM): Size = variable (CLI min 16 bytes: t_PACKETHEADER + BYTE m_btMoveTYPE + BYTE m_btFromIDX + tagBaseITEM m_MoveITEM; max + BYTE m_btUseSpecialTAB if platinum; GSV min 26 bytes: t_PACKETHEADER + short m_nInvIDX + short m_nBankIDX + tagBaseITEM m_InvItem + tagBaseITEM m_BankITEM; max + __int64 m_iCurMoney; client moves item between inventory and bank, server confirms with updated item data and optional money; implemented in GS_USER.cpp Send_gsv_MOVE_ITEM, client receives in RecvPACKET.cpp)
- 0x7b0 (GSV_SKILL_LEARN_REPLY): Size = 12 bytes (t_PACKETHEADER + BYTE m_btResult + BYTE m_btSkillSLOT + short m_nSkillIDX + short m_nSkillPOINT; server replies to skill learning request with result codes like success, need job, need ability, out of points; sent from GS_USER.cpp after skill learning attempts)
- 0x7b1 (CLI_SKILL_LEVELUP_REQ / GSV_SKILL_LEVELUP_REPLY): Size = 10 bytes CLI (t_PACKETHEADER + BYTE m_btSkillSLOT + short m_nNextLevelSkillIDX), 12 bytes GSV (t_PACKETHEADER + BYTE m_btResult + BYTE m_btSkillSLOT + short m_nSkillINDEX + short m_nSkillPOINT; client requests skill level up, server replies with result codes like success, need ability, out of points; handled in GS_USER.cpp for skill leveling)
- 0x7b2 (CLI_SELF_SKILL / GSV_SELF_SKILL): Size = 7 bytes CLI (t_PACKETHEADER + BYTE m_btSkillSLOT), variable GSV min 8 bytes (t_PACKETHEADER + WORD m_wSourObjIDX + short m_nSkillIDX; max includes char cNpcSkillMOTION[]; client uses self-targeting skill, server broadcasts skill use to nearby players with optional NPC motion data; skill execution in GS_USER.cpp, broadcast via Send_gsv_SELF_SKILL)
- 0x7b3 (CLI_TARGET_SKILL / GSV_TARGET_SKILL): Size = 9 bytes CLI (t_PACKETHEADER + WORD m_wDestObjIDX + BYTE m_btSkillSLOT), variable GSV min 22 bytes (t_PACKETHEADER + WORD m_wSourObjIDX + WORD m_wDestObjIDX + short m_nSkillIDX + WORD m_wSrvDIST + tPOINTF m_PosTO; max includes char cNpcSkillMOTION[]; client uses skill on target object, server broadcasts skill use to nearby players with position correction and optional NPC motion data; target skill processing in GS_USER.cpp)
- 0x7b4 (CLI_POSITION_SKILL / GSV_POSITION_SKILL): Size = 15 bytes CLI (t_PACKETHEADER + BYTE m_btSkillSLOT + tPOINTF m_PosTARGET), variable GSV min 18 bytes (t_PACKETHEADER + WORD m_wObjectIDX + short m_nSkillIDX + tPOINTF m_PosTARGET; max includes char cNpcSkillMOTION[]; client uses skill on ground position, server broadcasts skill use to nearby players with optional NPC motion data; position skills handled in GS_USER.cpp)
- 0x7b5 (GSV_EFFECT_OF_SKILL): Size = 12-15 bytes (t_PACKETHEADER + WORD m_wObjectIDX + WORD m_wSpellObjIDX + union { unsigned short m_nSkillIDX:12 + m_btSuccessBITS:2 + BYTE m_tmp1; or BYTE m_tmp2 + unsigned short m_tmp3:6 + m_nINT:10 }; server broadcasts skill effect result to nearby players with success bits and caster intelligence; sent after skill calculations in GS_USER.cpp)
- 0x7b6 (GSV_DAMAGE_OF_SKILL): Size = variable (min 14-17 bytes: inherits gsv_EFFECT_OF_SKILL + uniDAMAGE m_Damage; max includes tag_DROPITEM array for drops on death; server broadcasts skill damage to nearby players with damage amount and optional drop items; damage application in GS_USER.cpp)
- 0x7b7 (GSV_CLEAR_STATUS): Size = variable (min 12 bytes: t_PACKETHEADER + WORD m_wObjectIDX + DWORD m_dwStatusFLAG; max includes short m_nAdjVALUE[] for adjusted values; server broadcasts status effect removal to nearby players with remaining flags and adjusted stat values; status management in GS_USER.cpp)
- 0x7b8 (GSV_SPEED_CHANGED): Size = 14 bytes (t_PACKETHEADER + WORD m_wObjectIDX + short m_nRunSPEED + short m_nPsvAtkSPEED + BYTE m_btWeightRate + bool force; server broadcasts speed change to nearby players with run speed, attack speed, weight rate, and force flag; speed updates from GS_USER.cpp)
- 0x7b9 (GSV_RESULT_OF_SKILL): Size = 10 bytes (t_PACKETHEADER + WORD m_wObjectIDX + short m_nSkillIDX; server broadcasts skill result to nearby players; final skill outcome in GS_USER.cpp)
- 0x7ba (CLI_APPRAISAL_REQ / GSV_APPRAISAL_REPLY): Size = 8 bytes (t_PACKETHEADER + WORD m_wInventoryIndex for CLI; t_PACKETHEADER + WORD m_wInventoryIndex:12 + m_btResult:4 for GSV; client requests item appraisal from NPC, server replies with result code; appraisal logic in GS_USER.cpp)
- 0x7bb (GSV_SKILL_START): Size = 8 bytes (t_PACKETHEADER + WORD m_wObjectIDX; server broadcasts skill casting start to nearby players; sent at skill initiation in GS_USER.cpp)
- 0x7bc (CLI_CRAFT_ITEM_REQ / GSV_CRAFT_ITEM_REPLY): Size = 7 bytes CLI (t_PACKETHEADER + BYTE m_btTYPE), variable GSV min 8 bytes (t_PACKETHEADER + BYTE m_btRESULT + BYTE m_btOutCNT; max includes tag_SET_INVITEM array; client requests crafting operation, server replies with result and updated inventory items; crafting handled in GS_USER.cpp)
- 0x7bd (GSV_SKILL_CANCEL): Size = 9 bytes (t_PACKETHEADER + WORD m_wObjectIDX + BYTE m_btWHY; server broadcasts skill cancel to nearby players with reason code; cancel conditions in GS_USER.cpp)
- 0x790 (GSV_SET_EVENT_STATUS): Size = 10 bytes (t_PACKETHEADER + WORD m_wObjectIDX + short m_nEventSTATUS; server broadcasts event status change to nearby players for dynamic NPC behavior; event updates in GS_USER.cpp)
- 0x791 (GSV_NPC_CHAR): Size = variable (inherits from gsv_MOB_CHAR which inherits from tag_ADD_CHAR with variable m_nAdjSTATUS[]; server sends NPC character data to client when entering zone or spawning; zone entry in GS_USER.cpp)
- 0x792 (GSV_MOB_CHAR): Size = variable (inherits from tag_ADD_CHAR with variable m_nAdjSTATUS[]; server sends monster character data to client when entering zone or spawning; mob spawning in GS_USER.cpp)
- 0x793 (GSV_AVT_CHAR): Size = variable (inherits from tag_ADD_CHAR with variable m_nAdjSTATUS[] and additional data like PartITEM, RidingITEM, szUserID; server sends player character data to client when entering zone or spawning; player data in GS_USER.cpp)
- 0x794 (GSV_SUB_OBJECT): Size = 8 bytes (t_PACKETHEADER + WORD m_wObjectIDX; server removes objects from client view; object removal in GS_USER.cpp)
- 0x795 (CLI_SETPOS): Size = 14 bytes (t_PACKETHEADER + tPOINTF m_PosCUR; client sets position; position updates in RecvPACKET.cpp)
- 0x796 (CLI_STOP / GSV_STOP): Size = 16 bytes CLI (t_PACKETHEADER + tPOINTF m_PosCUR + short m_nPosZ), 18 bytes GSV (t_PACKETHEADER + WORD m_wObjectIDX + tPOINTF m_PosCUR + short m_nPosZ; client stops movement, server broadcasts stop; movement handling in GS_USER.cpp)
- 0x797 (GSV_MOVE): Size = 23 bytes (inherits from gsv_MOUSECMD + BYTE m_btMoveMODE; server broadcasts movement to nearby players; continuous movement updates in GS_USER.cpp)
- 0x798 (CLI_ATTACK / GSV_ATTACK): Size = 8 bytes CLI (t_PACKETHEADER + WORD m_wTargetObjectIndex), 20 bytes GSV (t_PACKETHEADER + WORD m_wAtkObjIDX + WORD m_wDefObjIDX + WORD m_wSrvDIST + tPOINTF m_PosTO; client attacks target, server broadcasts attack; combat logic in GS_USER.cpp)
- 0x799 (CLI_DAMAGE / GSV_DAMAGE): Size = 10 bytes CLI (t_PACKETHEADER + WORD m_wDefObjIDX + WORD m_wRandomIDX), variable GSV min 14 bytes (t_PACKETHEADER + WORD m_wAtkObjIDX + WORD m_wDefObjIDX + uniDAMAGE m_Damage; max includes tag_DROPITEM array; client sends damage request, server broadcasts damage with optional drops; damage calculation in GS_USER.cpp)
- 0x79a (CLI_MOUSECMD / GSV_MOUSECMD): Size = 18 bytes CLI (t_PACKETHEADER + WORD m_wTargetObjectIDX + tPOINTF m_PosTO + short m_nPosZ), 22 bytes GSV (t_PACKETHEADER + WORD m_wSourObjIDX + WORD m_wDestObjIDX + WORD m_wSrvDIST + tPOINTF m_PosTO + short m_nPosZ; client sends mouse command, server broadcasts; mouse interactions in GS_USER.cpp)
- 0x79b (GSV_SETEXP): Size = 18 bytes (t_PACKETHEADER + __int64 m_lCurEXP + short m_nCurSTAMINA + WORD m_wFromObjIDX; server sends experience gain to player; XP rewards in GS_USER.cpp)
- 0x79e (GSV_LEVELUP): Size = 22 bytes (t_PACKETHEADER + WORD m_wObjectIDX + short m_nCurLevel + __int64 m_lCurEXP + short m_nBonusPoint + short m_nSkillPoint; server broadcasts level up to nearby players; leveling in GS_USER.cpp)
- 0x79f (CLI_HP_REQ / GSV_HP_REPLY): Size = 8 bytes CLI (t_PACKETHEADER + WORD m_wObjectIDX), 12 bytes GSV (t_PACKETHEADER + WORD m_wObjectIDX + int m_iHP; client requests HP of object, server replies; HP queries in GS_USER.cpp)
- 0x7a0 (GSV_SET_HPnMP): Size = 12 bytes (t_PACKETHEADER + WORD m_wObjectIDX + short m_nHP + short m_nMP; server broadcasts HP/MP change to nearby players; health updates in GS_USER.cpp)
- 0x7bf (CLI_SET_WISHITEM): Size = 15 bytes (t_PACKETHEADER + BYTE m_btWishSLOT + tagBaseITEM m_ITEM; client sets wish item in personal store; wish list management in RecvPACKET.cpp)
- 0x7c0 (GSV_TRADE_P2P): Size = 10 bytes (t_PACKETHEADER + BYTE m_btRESULT + WORD m_wObjectIDX + char m_cTradeSLOT; server sends P2P trade response with result code, object index, and trade slot for trade initiation/acceptance/rejection; sent from GS_USER.cpp Send_gsv_TRADE_P2P, received in RecvPACKET.cpp for trade UI updates)
- 0x7c1 (GSV_TRADE_P2P_ITEM): Size = 15 bytes (t_PACKETHEADER + char m_cTradeSLOT + tagBaseITEM m_ITEM; server sends when an item is added to a P2P trade, including slot and item data; trade item handling in GS_USER.cpp)
- 0x7c2 (CLI_P_STORE_OPEN): Size = variable (min 8 bytes: t_PACKETHEADER + BYTE m_btSellItemCNT + BYTE m_btWishItemCNT; max includes tagPS_ITEM array for sell/wish items and null-terminated store title string; client opens personal store with items for sale and wish list; store opening in RecvPACKET.cpp)
- 0x7c2 (GSV_P_STORE_OPENED): Size = variable (min 8 bytes: t_PACKETHEADER + WORD m_wObjectIDX; max includes null-terminated store title string; server broadcasts personal store opened to nearby players; store broadcasts in GS_USER.cpp)
- 0x7c3 (CLI_P_STORE_CLOSE): Size = 6 bytes (t_PACKETHEADER only; client closes personal store; store closing in RecvPACKET.cpp)
- 0x7c3 (GSV_P_STORE_CLOSED): Size = 8 bytes (t_PACKETHEADER + WORD m_wObjectIDX; server broadcasts personal store closed to nearby players; store close broadcasts in GS_USER.cpp)
- 0x7c4 (CLI_P_STORE_LIST_REQ): Size = 8 bytes (t_PACKETHEADER + WORD m_wStoreObjectIDX; client requests personal store item list; list requests in RecvPACKET.cpp)
- 0x7c4 (GSV_P_STORE_LIST_REPLY): Size = variable (min 8 bytes: t_PACKETHEADER + BYTE m_btSellItemCNT + BYTE m_btWishItemCNT; max includes tagPS_SLOT_PRICE array for sell/wish items with prices; server replies with personal store item list; list replies in GS_USER.cpp)
- 0x7c5 (CLI_P_STORE_BUY_REQ): Size = variable (min 9 bytes: t_PACKETHEADER + WORD m_wStoreObjectIDX + BYTE m_btItemCNT; max includes tagPS_SLOT_ITEM array for items to buy; client requests to buy items from personal store; buy requests in RecvPACKET.cpp)
- 0x7c6 (CLI_P_STORE_SELL_REQ): Size = variable (min 9 bytes: t_PACKETHEADER + WORD m_wStoreObjectIDX + BYTE m_btItemCNT; max includes tagSELL_ITEM array for items to sell; client requests to sell items to personal store; sell requests in RecvPACKET.cpp)
- 0x7c6 (GSV_P_STORE_RESULT): Size = variable (min 10 bytes: t_PACKETHEADER + WORD m_wStoreObjectIDX + BYTE m_btResult + BYTE m_btItemCNT; max includes tagPS_SLOT_ITEM array for updated items; server sends result of personal store buy/sell transaction; transaction results in GS_USER.cpp)
- 0x7c7 (GSV_P_STORE_MONEYnINV): Size = variable (min 14 bytes: t_PACKETHEADER + __int64 m_i64Money; max includes tag_SET_INVITEM array for updated inventory items; server updates money and inventory after personal store transaction; inventory updates in GS_USER.cpp)
- 0x7ca (CLI_ASSEMBLE_RIDE_ITEM): Size = 6 bytes (t_PACKETHEADER only; client assembles ride item; assembly requests in RecvPACKET.cpp)
- 0x7ca (GSV_ASSEMBLE_RIDE_ITEM): Size = 6 bytes (t_PACKETHEADER only; server responds to ride item assembly; assembly responses in GS_USER.cpp)
- 0x7cb (CLI_USE_ITEM_TO_REPAIR): Size = variable (min 6 bytes: t_PACKETHEADER; max includes item data; client uses item to repair equipment; repair requests in RecvPACKET.cpp)
- 0x7cb (GSV_USED_ITEM_TO_REPAIR): Size = variable (min 6 bytes: t_PACKETHEADER; max includes tag_SET_INVITEM array; server updates inventory after item repair; repair results in GS_USER.cpp)
- 0x7cd (CLI_REPAIR_FROM_NPC): Size = 8 bytes (t_PACKETHEADER + short m_nNpcIDX; client requests repair from NPC; NPC repair requests in RecvPACKET.cpp)
- 0x7cd (GSV_REPAIRED_FROM_NPC): Size = variable (min 14 bytes: t_PACKETHEADER + __int64 m_i64Money; max includes tag_SET_INVITEM array; server updates money and inventory after NPC repair; NPC repair in GS_USER.cpp)
- 0x7ce (GSV_SET_ITEM_LIFE): Size = 16 bytes (t_PACKETHEADER + WORD m_wObjectIDX + tagBaseITEM m_ITEM; server updates item durability for equipped items; durability updates in GS_USER.cpp)
- 0x7d2 (GSV_PARTY_MEMBER): Size = variable (min 16 bytes for member removal: t_PACKETHEADER + BYTE m_btPartyRULE + char m_cUserCNT + DWORD[2] m_dwObjectTAG; for member addition: + m_cUserCNT * (sizeof(tag_PARTY_MEMBER) + strlen(name) + 1); server sends party member updates to party members; party management in GS_USER.cpp)
- 0x7d1 (GSV_PARTY_REPLY): Size = 11 bytes (t_PACKETHEADER + BYTE m_btREPLY + DWORD m_dwFromIDXorTAG; server sends reply to party requests with status codes like accept/reject/busy/not found; party replies in GS_USER.cpp)
- 0x7d3 (GSV_PARTY_ITEM): Size = 16 bytes (t_PACKETHEADER + WORD m_wObjectIDX + tagBaseITEM m_ITEM; server broadcasts party item pickup to party members in ordered distribution mode; party item distribution in GS_USER.cpp)
- 0x7d4 (GSV_PARTY_LEVnEXP): Size = 11 bytes (t_PACKETHEADER + BYTE m_btLEVEL + unsigned int m_iEXP:31 + m_bitLevelUP:1; server sends party level and experience updates to party members; party XP sharing in GS_USER.cpp)
- 0x7d5 (GSV_CHANGE_OBJIDX): Size = 26 bytes (t_PACKETHEADER + tag_PARTY_MEMBER; server updates party member object index and status info when member changes zones or reconnects; party member changes in GS_USER.cpp)
- 0x7d6 (GSV_ADD_EVENTOBJ): Size = 14 bytes (t_PACKETHEADER + WORD m_wObjectIDX + char m_cMapX + char m_cMapY + WORD m_wMapEventID + short m_nEventSTATUS; server adds event objects to zones with position, event ID, and status for dynamic zone events; event objects in GS_USER.cpp)
- 0x7d7 (CLI_PARTY_RULE): Size = 7 bytes (t_PACKETHEADER + BYTE m_btPartyRUEL; client sends party rule change request to server; party rule changes in RecvPACKET.cpp)
- 0x7d7 (GSV_PARTY_RULE): Size = 7 bytes (t_PACKETHEADER + BYTE m_btPartyRULE; server broadcasts party rule change to party members; party rule broadcasts in GS_USER.cpp)
- 0x7d8 (CLI_ITEM_RESULT_REPORT): Size = 7 bytes (t_PACKETHEADER + BYTE m_btREPORT + BYTE m_btItemType + short m_nItemNO; client reports item creation/upgrade results to server; item reports in RecvPACKET.cpp)
- 0x7d8 (GSV_ITEM_RESULT_REPORT): Size = 10 bytes (t_PACKETHEADER + WORD m_wObjectIDX + BYTE m_btREPORT + BYTE m_btItemType + short m_nItemNO; server broadcasts item creation/upgrade results to nearby players; item result broadcasts in GS_USER.cpp)
- 0x7d9 (CLI_MALL_ITEM_REQ): Size = variable (min 9 bytes: t_PACKETHEADER + BYTE m_btReqTYPE + short m_nDupCnt; max includes appended strings for character names, descriptions, and inventory indices; client sends mall operations like list items, bring to inventory, find character for gifting, or give item; mall requests in RecvPACKET.cpp)
- 0x7d9 (GSV_MALL_ITEM_REPLY): Size = variable (min 8 bytes: t_PACKETHEADER + BYTE m_btReplyTYPE + BYTE m_btCntOrIdx; max includes tag_SET_INVITEM arrays for inventory updates or tagBaseITEM + strings for item lists; server responds to mall operations with results and updated data; mall responses in GS_USER.cpp)
- 0x7da (CLI_MOVE_ZULY): Size = 15 bytes (t_PACKETHEADER + BYTE m_btMoveTYPE + __int64 m_i64MoveZuly; client sends zuly movement request between inventory and bank; zuly moves in RecvPACKET.cpp)
- 0x7da (GSV_MOVE_ZULY): Size = 22 bytes (t_PACKETHEADER + __int64 m_i64InvZuly + __int64 m_i64BankZuly; server responds with updated balances after zuly movement; zuly updates in GS_USER.cpp)
- 0x7db (GSV_SET_NPC_SHOW): Size = 9 bytes (t_PACKETHEADER + WORD m_wObjectIDX + bool m_bShow; server broadcasts to sectors when NPC visibility changes, client shows/hides NPC based on flag for dynamic NPC behavior; NPC visibility in GS_USER.cpp)
- 0x7dc (GSV_GODDNESS_MODE): Size = 9 bytes (t_PACKETHEADER + BYTE m_btOnOff + WORD m_wObjectIDX; server broadcasts goddess mode status to nearby players for fairy aura system that provides stat bonuses; goddess mode in GS_USER.cpp)
- 0x7dd (CLI_CART_RIDE / GSV_CART_RIDE): Size = 11 bytes (t_PACKETHEADER + BYTE m_btType + WORD m_wOwnerObjIDX + WORD m_wGuestObjIDX; client sends cart ride requests/responses, server broadcasts cart ride status to nearby players for 2-person cart riding system; cart riding in GS_USER.cpp)
- 0x7de (GSV_BILLING_MESSAGE_EXT): Size = variable (min 28 bytes: t_PACKETHEADER + WORD m_wMsgTYPE + DWORD m_dwPayType + DWORD m_dwPlayingFlag[4]; max includes null-terminated message string; game server sends extended billing notifications like payment type, expiration dates/times, alerts, and kickout messages for different regions; billing messages in GS_USER.cpp)
- 0x7df (GSV_BILLING_MESSAGE): Size = variable (min 7 bytes: t_PACKETHEADER + BYTE m_btTYPE; max includes null-terminated message string; game server sends billing notifications like payment type, expiration warnings, kickout messages; billing in GS_USER.cpp)
- 0x7e0 (CLI_CLAN_COMMAND): Size = variable (min 7 bytes: t_PACKETHEADER + BYTE m_btCMD; max includes appended strings for member names, clan names, etc.; client sends clan management commands like create, invite, remove, promote, demote, quit, roster, leader; clan commands in RecvPACKET.cpp)
- 0x7e0 (WSV_CLAN_COMMAND): Size = variable (min 7 bytes: t_PACKETHEADER + BYTE m_btRESULT; max includes appended clan data, member lists, strings for names/masters; world server sends clan command responses like create/join results, member updates, kick notifications; clan responses in SHO_WS)
- 0x7e1 (CLI_MESSENGER): Size = variable (tag_MCMD_HEADER 7 bytes + command-specific data; cli_MCMD_APPEND_REQ min 7 bytes + name string; cli_MCMD_APPEND_REPLY min 9 bytes + optional name string; cli_MCMD_TAG 11 bytes; cli_MCMD_STATUS_REQ 8 bytes; client sends friend management commands like add, delete, status change; messenger commands in RecvPACKET.cpp)
- 0x7e1 (WSV_MESSENGER): Size = variable (responses like wsv_MCMD_APPEND_REQ min 9 bytes + name string; wsv_MCMD_APPEND_ACCEPT min 12 bytes + name string; wsv_MCMD_STATUS_REPLY 12 bytes; wsv_MCMD_LIST min 8 bytes + variable friend data; world server sends friend management responses; messenger responses in SHO_WS)
- 0x7e2 (CLI_MESSENGER_CHAT): Size = variable (min 10 bytes: t_PACKETHEADER + DWORD m_dwUserTAG; max includes null-terminated message string; client sends private message to friend; private messages in RecvPACKET.cpp)
- 0x7e2 (WSV_MESSENGER_CHAT): Size = variable (min 10 bytes: t_PACKETHEADER + DWORD m_dwUserTAG; max includes null-terminated message string; world server forwards private message to recipient; message forwarding in SHO_WS)
- 0x7e3 (CLI_CHATROOM): Size = variable (min 7 bytes: tag_CHAT_HEADER for CHAT_REQ_LEFT; cli_CHAT_ROOM_MAKE min 9 bytes + strings; cli_CHAT_ROOM_JOIN min 10 bytes + optional string; cli_CHAT_ROOM_LIST 10 bytes; cli_CHAT_ROOM_KICK 11 bytes; client sends chatroom commands like make, join, list, kick; chatroom commands in RecvPACKET.cpp)
- 0x7e3 (WSV_CHATROOM): Size = variable (responses like wsv_CHAT_ROOM_LIST: t_PACKETHEADER + BYTE m_btCMD + char m_cRoomCNT + variable tag_CHAT_ROOM[] with strings; world server sends chatroom responses; chatroom responses in SHO_WS)
- 0x7e5 (CLI_MEMO): Size = variable (min 7 bytes: t_PACKETHEADER + BYTE m_btTYPE; for MEMO_REQ_SEND: + strlen(targetName) + 1 + strlen(memoContent) + 1; client sends memo requests; memo requests in RecvPACKET.cpp)
- 0x7e5 (WSV_MEMO): Size = variable (for MEMO_REPLY_RECEIVED_CNT: 8 bytes t_PACKETHEADER + BYTE + short; for MEMO_REPLY_CONTENTS: variable with appended date, sender, content; world server sends memo responses; memo responses in SHO_WS)
- 0x7e4 (CLI_CHATROOM_MSG): Size = variable (min 7 bytes: t_PACKETHEADER + null-terminated message string; client sends chat message to chatroom; chat messages in RecvPACKET.cpp)
- 0x7e4 (WSV_CHATROOM_MSG): Size = variable (min 9 bytes: t_PACKETHEADER + WORD m_wObjectID + null-terminated message string; game server broadcasts chat message in chatroom; chat broadcasts in GS_USER.cpp)
- 0x7e6 (CLI_CLANMARK_SET): Size = variable (min 8 bytes: t_PACKETHEADER + WORD m_wMarkCRC16; max includes BYTE m_btMARK[] for clanmark data; client sends clanmark data to set/update; clanmark sets in RecvPACKET.cpp)
- 0x7e7 (CLI_CLANMARK_REQ): Size = 8 bytes (t_PACKETHEADER + DWORD m_dwClanID; client requests clanmark data; clanmark requests in RecvPACKET.cpp)
- 0x7e7 (WSV_CLANMARK_REPLY): Size = variable (min 10 bytes: t_PACKETHEADER + DWORD m_dwClanID + WORD m_wMarkCRC16; max includes BYTE m_btMARK[] for clanmark data; world server sends clanmark; clanmark replies in SHO_WS)
- 0x7e8 (CLI_CLANMARK_REG_TIME): Size = 4 bytes (t_PACKETHEADER only; client requests clanmark registration time; reg time requests in RecvPACKET.cpp)
- 0x7e8 (WSV_CLANMARK_REG_TIME): Size = 11 bytes (t_PACKETHEADER + WORD m_wYear + BYTE m_btMon + BYTE m_btDay + BYTE m_btHour + BYTE m_btMin + BYTE m_btSec; world server sends registration time; reg time replies in SHO_WS)
- 0x7e9 (GSV_PATSTATE_CHANGE): Size = 13 bytes (t_PACKETHEADER + BYTE m_btOnOff + DWORD m_dwCoolTIME + short m_nMaxPatHP + WORD m_wObjectIDX; server notifies pet/cart state changes; pet state in GS_USER.cpp)
- 0x7ea (GSV_CHARSTATE_CHANGE): Size = 10 bytes (t_PACKETHEADER + WORD m_wObjectIDX + DWORD m_dwFLAG; server notifies character state changes; char state in GS_USER.cpp)
- 0x7eb (CLI_SCREEN_SHOT_TIME): Size = 6 bytes (t_PACKETHEADER + WORD wCnt; client requests server time for screenshot; screenshot requests in RecvPACKET.cpp)
- 0x7eb (GSV_SCREEN_SHOT_TIME): Size = 10 bytes (t_PACKETHEADER + WORD wYear + BYTE btMonth + BYTE btDay + BYTE btHour + BYTE btMin; server sends current time; time replies in GS_USER.cpp)
- 0x781 (CLI_SET_MOTION / GSV_SET_MOTION): Size = 8 bytes CLI (t_PACKETHEADER + short m_nMotionNO + WORD m_wValue), 10 bytes GSV (t_PACKETHEADER + WORD m_wObjectIDX + short m_nMotionNO + WORD m_wValue); client sends motion/animation change, server broadcasts to nearby players; implemented in GS_USER.cpp Recv_cli_SET_MOTION and Send_gsv_SET_MOTION, CObjCHAR.cpp Send_gsv_SET_MOTION, RecvPACKET.cpp Recv_gsv_SET_MOTION
- 0x782 (CLI_TOGGLE / GSV_TOGGLE): Size = 5 bytes CLI (t_PACKETHEADER + BYTE m_btTYPE), 8-10 bytes GSV (t_PACKETHEADER + WORD m_wObjectIDX + BYTE m_btTYPE; + short m_nRunSPEED[0] when riding pet); client sends toggle commands (run/walk, sit/stand, drive), server broadcasts to nearby players; implemented in GS_USER.cpp Recv_cli_TOGGLE, CObjCHAR.cpp Send_gsv_TOGGLE, RecvPACKET.cpp Recv_gsv_TOGGLE
- 0x783 (CLI_CHAT / GSV_CHAT): Size = variable min 4 bytes CLI (t_PACKETHEADER + null-terminated message string), variable min 6 bytes GSV (t_PACKETHEADER + WORD m_wObjectIDX + null-terminated message string); client sends chat message, server broadcasts to nearby players; implemented in GS_USER.cpp Recv_cli_CHAT, CObjCHAR.cpp Send_gsv_CHAT, RecvPACKET.cpp Recv_gsv_CHAT
- 0x784 (CLI_WHISPER / GSV_WHISPER): Size = variable min 4 bytes (t_PACKETHEADER + null-terminated target account + null-terminated message); client sends private message to target account, server forwards to recipient; implemented in GS_USER.cpp Recv_cli_WHISPER, CWS_Client.cpp Send_gsv_WHISPER and Recv_cli_WHISPER, client SendPACKET.cpp Send_cli_WHISPER, RecvPACKET.cpp Recv_gsv_WHISPER
- 0x785 (CLI_SHOUT / GSV_SHOUT): Size = variable min 4 bytes (t_PACKETHEADER + null-terminated message string); client sends shout message, server broadcasts to nearby players; implemented in GS_USER.cpp Recv_cli_SHOUT, CObjCHAR.cpp Send_gsv_SHOUT, RecvPACKET.cpp Recv_gsv_SHOUT
- 0x786 (CLI_PARTY_CHAT / GSV_PARTY_CHAT): Size = variable min 4 bytes CLI (t_PACKETHEADER + null-terminated message string), variable min 6 bytes GSV (t_PACKETHEADER + WORD m_wObjectIDX + null-terminated message string); client sends party chat message, server broadcasts to party members; implemented in GS_USER.cpp Recv_cli_PARTY_CHAT and Send_gsv_PARTY_CHAT, RecvPACKET.cpp Recv_gsv_PARTY_CHAT
- 0x787 (CLI_CLAN_CHAT / WSV_CLAN_CHAT): Size = variable min 4 bytes (t_PACKETHEADER + null-terminated message string); client sends clan chat message, world server broadcasts to clan members; implemented in GS_USER.cpp Recv_cli_CLAN_CHAT, CWS_Client.cpp Recv_cli_CLAN_CHAT, RecvPACKET.cpp Recv_wsv_CLAN_CHAT
- 0x808 (CLI_FAKE_GAMEGUARD_REQ / GSV_FAKE_GAMEGUARD_REPLY): Size = 4 bytes (t_PACKETHEADER only, no additional data implemented in source code; placeholder packets)

## Documented Packets Summary
- Total opcodes documented: 124
- 0x700: CLI_ALIVE / SRV_ERROR (6 bytes CLI, 8 bytes SRV, client sends keep-alive, server sends error codes)
- 0x701: SRV_ANNOUNCE_TEXT (variable min 6 bytes, server sends announcement text to client)
- 0x702: GSV_ANNOUNCE_CHAT (variable min 6 bytes, server sends announcement chat messages to client)
- 0x703: CLI_ACCEPT_REQ (6 bytes, client sends acceptance request)
- 0x704: CLI_CHANNEL_LIST_REQ / LSV_CHANNEL_LIST_REPLY (10 bytes CLI, variable LSV min 11 bytes, client requests channel list, login server replies with available channels)
- 0x705: CLI_CHECK_AUTH / SRV_CHECK_AUTH (68 bytes CLI, 7 bytes SRV, client sends login credentials, server replies with authentication result)
- 0x707: SRV_LOGOUT_REQ / GSV_LOGOUT_REPLY (7 bytes, login server requests logout from game server, game server replies with result)
- 0x708: SRV_LOGIN_REQ / SRV_LOGIN_REPLY (72 bytes SRV, 7 bytes SRV, login server sends login credentials to world server, world server replies with result)
- 0x709: GSV_LOGIN_REPLY (15 bytes, game server replies to login server with login result, server ID, and user tag)
- 0x70a: SRV_SELECT_SERVER / LSV_SELECT_SERVER (10 bytes SRV, 7 bytes LSV, world server selects server, login server replies with result)
- 0x70b: CLI_JOIN_SERVER_REQ (10 bytes, client requests to join a specific server)
- 0x70c: SRV_JOIN_SERVER_REPLY (7 bytes, login server replies to client with join server result)
- 0x70d: GSV_GM_COMMAND (variable min 9 bytes, game server sends GM command to client)
- 0x70e: GSV_SET_GLOBAL_VAR (11 bytes, game server sets global variable value for client)
- 0x70f: GSV_SET_GLOBAL_FLAG (11 bytes, game server sets global flag value for client)
- 0x711: WSV_MOVE_SERVER (variable min 16 bytes, world server notifies client to move to zone server with port, IDs, and IP address; sent from world server CWS_Client.cpp, received in client RecvPACKET.cpp)
- 0x712: CLI_CHAR_LIST (6 bytes, client requests character list from world server; sent from client SendPACKET.cpp, received in world server WS_ThreadSQL.cpp)
- 0x712: WSV_CHAR_LIST (variable min 7 bytes: t_PACKETHEADER + BYTE m_btCharCNT; max includes per-character data: null-terminated name string + tagCHARINFO (11 bytes) + tagPartITEM[10] (80 bytes); world server sends character list with race, level, job, delete timer, zone, equipped items; sent from world server WS_ThreadSQL.cpp, received in client RecvPACKET.cpp)
- 0x713: CLI_CREATE_CHAR (variable min 7 bytes: t_PACKETHEADER + BYTE m_btCharRACE + char m_cBoneSTONE + char m_cHairIDX + char m_cFaceIDX + char m_cWeaponTYPE + short m_nZoneNO; max includes null-terminated szCharName string; client sends character creation data including race, appearance options, weapon type, zone, and character name; sent from client SendPACKET.cpp, received in world server CWS_Client.cpp)
- 0x713: WSV_CREATE_CHAR (8 bytes: t_PACKETHEADER + BYTE m_btResult + BYTE m_btIsPlatinumCHAR; world server replies with creation result code and platinum character flag; sent from world server CWS_Client.cpp, received in client RecvPACKET.cpp)
- 0x714: CLI_DELETE_CHAR (7 bytes fixed + variable string: t_PACKETHEADER + BYTE m_btCharNO + bool m_bDelete; max includes null-terminated szCharName string; client sends character deletion request with character slot number, delete flag (true for delete, false for revive), and character name; sent from client SendPACKET.cpp, received in world server CWS_Client.cpp, processed in WS_ThreadSQL.cpp with SQL injection filtering, clan membership checks (masters cannot delete), and database update setting dwDelTIME; response WSV_DELETE_CHAR 10 bytes fixed + variable string with remaining deletion time (0 for revive, wait time for delete, 0xffffffff for clan masters))
- 0x715: CLI_SELECT_CHAR / GSV_SELECT_CHAR (CLI 9 bytes fixed + variable string: t_PACKETHEADER + BYTE m_btCharNO + BYTE m_btRunMODE + BYTE m_btRideMODE; max includes null-terminated szCharName string; client sends character selection request with character slot, run mode, ride mode, and character name; sent from client SendPACKET.cpp, received in world server CWS_Client.cpp; GSV variable min 7 bytes: t_PACKETHEADER + BYTE m_btCharRACE + short m_nZoneNO + tPOINTF m_PosSTART + short m_nReviveZoneNO; max includes tagPartITEM[10] (80 bytes), tagBasicINFO, tagBasicAbility, tagGrowAbility, tagSkillAbility, CHotICONS, DWORD m_dwUniqueTAG, and null-terminated szUserID string; game server sends selected character data to client when entering zone; sent from game server GS_USER.cpp, received in client RecvPACKET.cpp)
- 0x716: GSV_INVENTORY_DATA (1118 bytes: t_PACKETHEADER + __int64 m_i64Money + tagBaseITEM m_ItemLIST[138]; game server sends player's inventory data including money and all inventory items to client when entering zone; sent from game server, received in client RecvPACKET.cpp)
- 0x717: GSV_SET_MONEYnINV (variable min 15 bytes, game server sends updated money and inventory items to client after changes like buying/selling or trading)
- 0x718: GSV_SET_INV_ONLY (variable min 7 bytes, game server sends updated inventory items to client when inventory changes like equipping/unequipping items or item consumption)
- 0x719: GSV_SERVER_DATA (variable min 7 bytes, game server sends server-specific data to client such as NPC information or economy settings)
- 0x71a: GSV_RELAY_REQ (16 bytes fixed, game server sends relay request to client, such as recall to zone with zone number and position)
- 0x71b: GSV_QUEST_DATA (1486 bytes fixed, game server sends quest data and wish list to client)
- 0x751: GSV_CHEAT_CODE (variable min 9 bytes: t_PACKETHEADER + WORD m_wObjectIDX + null-terminated char m_szCheatCODE[]; game server sends cheat code string to client for execution or display; sent from GS_USER.cpp Send_gsv_CHEAT_CODE, received in RecvPACKET.cpp Recv_gsv_CHEAT_CODE)
- 0x753: CLI_JOIN_ZONE / GSV_JOIN_ZONE (9 bytes CLI, 60 bytes GSV, client sends join zone request with zone number and position, server replies with zone data including weather, time, and global variables)
- 0x761: CLI_CHAR_INFO_REQ / GSV_CHAR_INFO_REPLY (not implemented in source code, defined but commented out; client would request character info, server would reply)
- 0x762: CLI_SET_WEIGHT_RATE / GSV_SET_WEIGHT_RATE (5 bytes CLI, 7 bytes GSV, client sends weight rate, server relays to nearby)
- 0x770: GSV_ADJUST_POS (16 bytes, server adjusts position of an object for synchronization or anti-cheat)
- 0x771: CLI_CANTMOVE / GSV_CANTMOVE (14 bytes CLI, GSV not implemented; client sends can't move position to server)
- 0x772: CLI_ATTACK_START / GSV_ATTACK_START (not implemented in source code, defines commented out)
- 0x773: GSV_ATTACK_STOP (not implemented in source code, define commented out)
- 0x774: GSV_CHANGE_NPC (8 bytes: t_PACKETHEADER + WORD m_wObjectIDX + short m_nNPCNo; server broadcasts NPC change to nearby players, changing the NPC's appearance or type; sent from GS_USER.cpp, received in RecvPACKET.cpp)
- 0x762: CLI_SET_WEIGHT_RATE / GSV_SET_WEIGHT_RATE (5 bytes CLI: t_PACKETHEADER + BYTE btWeightRate; 7 bytes GSV: t_PACKETHEADER + WORD wObjectIDX + BYTE btWeightRate; client sends weight rate, server relays to nearby)
- 0x770: GSV_ADJUST_POS (16 bytes: t_PACKETHEADER + WORD wObjectIDX + tPOINTF PosCUR + short nPosZ; server adjusts position of an object for synchronization or anti-cheat)
- 0x771: CLI_CANTMOVE / GSV_CANTMOVE (14 bytes CLI: t_PACKETHEADER + tPOINTF m_PosCUR + short m_nPosZ; GSV not implemented in source code; client sends can't move position to server when collision or stuck; sent from client SendPACKET.cpp, received in GS_USER.cpp Recv_cli_CANTMOVE)
- 0x772: CLI_ATTACK_START / GSV_ATTACK_START (not implemented in source code, defines commented out)
- 0x773: GSV_ATTACK_STOP (not implemented in source code, define commented out)
- 0x774: GSV_CHANGE_NPC (8 bytes, server broadcasts NPC change to nearby players, changing the NPC's appearance or type)
- 0x775: CLI_SUMMON_CMD / GSV_SUMMON_CMD (5 bytes CLI, GSV not implemented; client sends summon command for pet/cart control)
- 0x775: 5 bytes (CLI_SUMMON_CMD - client sends summon command for pet/cart control) / not implemented (GSV_SUMMON_CMD)
- 0x781: CLI_SET_MOTION / GSV_SET_MOTION (8 bytes CLI, 10 bytes GSV, client sends motion/animation change, server broadcasts to nearby players)
- 0x782: CLI_TOGGLE / GSV_TOGGLE (5 bytes CLI, 8-10 bytes GSV, client sends toggle commands for movement modes, server broadcasts to nearby players)
- 0x783: CLI_CHAT / GSV_CHAT (variable min 4 bytes CLI, variable min 6 bytes GSV, client sends chat message, server broadcasts to nearby players)
- 0x784: CLI_WHISPER / GSV_WHISPER (variable min 4 bytes, client sends private message, server forwards to recipient)
- 0x785: CLI_SHOUT / GSV_SHOUT (variable min 4 bytes, client sends shout message, server broadcasts to nearby players)
- 0x786: CLI_PARTY_CHAT / GSV_PARTY_CHAT (variable min 4 bytes CLI, variable min 6 bytes GSV, client sends party chat, server broadcasts to party)
- 0x787: CLI_CLAN_CHAT / WSV_CLAN_CHAT (variable min 4 bytes, client sends clan chat, world server broadcasts to clan)
- 0x7ac: CLI_CHANGE_SKIN / GSV_CHANGE_SKIN (14 bytes, client requests skin change, server broadcasts skin change to nearby players with ability update)
- 0x7ad: CLI_BANK_LIST_REQ / GSV_BANK_LIST_REPLY (variable, client requests bank access with password, server replies with bank item list or status)
- 0x7ae: CLI_MOVE_ITEM / GSV_MOVE_ITEM (variable, client moves item between inventory and bank, server confirms with updated item data and optional money)
- 0x7b0: GSV_SKILL_LEARN_REPLY (12 bytes, server replies to skill learning request with result codes like success, need job, need ability, out of points)
- 0x7b1: CLI_SKILL_LEVELUP_REQ / GSV_SKILL_LEVELUP_REPLY (10 bytes CLI, 12 bytes GSV, client requests skill level up, server replies with result codes like success, need ability, out of points)
- 0x7b2: CLI_SELF_SKILL / GSV_SELF_SKILL (7 bytes CLI, variable GSV min 8 bytes, client uses self-targeting skill, server broadcasts skill use to nearby players with optional NPC motion data)
- 0x7b3: CLI_TARGET_SKILL / GSV_TARGET_SKILL (9 bytes CLI, variable GSV min 22 bytes, client uses skill on target object, server broadcasts skill use to nearby players with position correction and optional NPC motion data)
- 0x7b4: CLI_POSITION_SKILL / GSV_POSITION_SKILL (15 bytes CLI, variable GSV min 18 bytes, client uses skill on ground position, server broadcasts skill use to nearby players with optional NPC motion data)
- 0x7b5: GSV_EFFECT_OF_SKILL (12-15 bytes, server broadcasts skill effect result to nearby players with success bits and caster intelligence)
- 0x7b6: GSV_DAMAGE_OF_SKILL (variable min 14-17 bytes, server broadcasts skill damage to nearby players with damage amount and optional drop items)
- 0x7b7: GSV_CLEAR_STATUS (variable min 12 bytes, server broadcasts status effect removal to nearby players with remaining flags and adjusted stat values)
- 0x7b8: GSV_SPEED_CHANGED (14 bytes, server broadcasts speed change to nearby players with run speed, attack speed, weight rate, and force flag)
- 0x7b9: GSV_RESULT_OF_SKILL (10 bytes, server broadcasts skill result to nearby players)
- 0x7ba: CLI_APPRAISAL_REQ / GSV_APPRAISAL_REPLY (8 bytes, client requests item appraisal from NPC, server replies with result code)
- 0x7bb: GSV_SKILL_START (8 bytes, server broadcasts skill casting start to nearby players)
- 0x7bc: CLI_CRAFT_ITEM_REQ / GSV_CRAFT_ITEM_REPLY (7 bytes CLI, variable GSV min 8 bytes, client requests crafting operation, server replies with result and updated inventory items)
- 0x7bd: GSV_SKILL_CANCEL (9 bytes, server broadcasts skill cancel to nearby players with reason code)
- 0x790: GSV_SET_EVENT_STATUS (10 bytes, server broadcasts event status change to nearby players for dynamic NPC behavior)
- 0x791: GSV_NPC_CHAR (variable, server sends NPC character data to client when entering zone or spawning)
- 0x792: GSV_MOB_CHAR (variable, server sends monster character data to client when entering zone or spawning)
- 0x793: GSV_AVT_CHAR (variable, server sends player character data to client when entering zone or spawning)
- 0x794: GSV_SUB_OBJECT (8 bytes, server removes objects from client view)
- 0x795: CLI_SETPOS (14 bytes, client sets position)
- 0x796: CLI_STOP / GSV_STOP (16 bytes CLI, 18 bytes GSV, client stops movement, server broadcasts stop)
- 0x797: GSV_MOVE (23 bytes, server broadcasts movement to nearby players)
- 0x798: CLI_ATTACK / GSV_ATTACK (8 bytes CLI, 20 bytes GSV, client attacks target, server broadcasts attack)
- 0x799: CLI_DAMAGE / GSV_DAMAGE (10 bytes CLI, variable GSV min 14 bytes, client sends damage request, server broadcasts damage with optional drops)
- 0x79a: CLI_MOUSECMD / GSV_MOUSECMD (18 bytes CLI, 22 bytes GSV, client sends mouse command, server broadcasts)
- 0x79b: GSV_SETEXP (18 bytes, server sends experience gain to player)
- 0x79e: GSV_LEVELUP (22 bytes, server broadcasts level up to nearby players)
- 0x79f: CLI_HP_REQ / GSV_HP_REPLY (8 bytes CLI, 12 bytes GSV, client requests HP of object, server replies)
- 0x7a0: GSV_SET_HPnMP (12 bytes, server broadcasts HP/MP change to nearby players)
- 0x7bf: CLI_SET_WISHITEM (15 bytes, client sets wish item in personal store)
- 0x7c0: GSV_TRADE_P2P (10 bytes, server sends P2P trade response with result code, object index, and trade slot for trade initiation/acceptance/rejection)
- 0x7c1: GSV_TRADE_P2P_ITEM (15 bytes, server sends when an item is added to a P2P trade, including slot and item data)
- 0x7c2: CLI_P_STORE_OPEN (variable min 8 bytes, client opens personal store with items for sale and wish list) / GSV_P_STORE_OPENED (variable min 8 bytes, server broadcasts personal store opened to nearby players)
- 0x7c3: CLI_P_STORE_CLOSE (6 bytes, client closes personal store) / GSV_P_STORE_CLOSED (8 bytes, server broadcasts personal store closed to nearby players)
- 0x7c4: CLI_P_STORE_LIST_REQ (8 bytes, client requests personal store item list) / GSV_P_STORE_LIST_REPLY (variable min 8 bytes, server replies with personal store item list)
- 0x7c5: CLI_P_STORE_BUY_REQ (variable min 9 bytes, client requests to buy items from personal store)
- 0x7c6: CLI_P_STORE_SELL_REQ (variable min 9 bytes, client requests to sell items to personal store) / GSV_P_STORE_RESULT (variable min 10 bytes, server sends result of personal store buy/sell transaction)
- 0x7c7: GSV_P_STORE_MONEYnINV (variable min 14 bytes, server updates money and inventory after personal store transaction)
- 0x7ca: CLI_ASSEMBLE_RIDE_ITEM (6 bytes, client assembles ride item) / GSV_ASSEMBLE_RIDE_ITEM (6 bytes, server responds to ride item assembly)
- 0x7cb: CLI_USE_ITEM_TO_REPAIR (variable min 6 bytes, client uses item to repair equipment) / GSV_USED_ITEM_TO_REPAIR (variable min 6 bytes, server updates inventory after item repair)
- 0x7cd: CLI_REPAIR_FROM_NPC (8 bytes, client requests repair from NPC) / GSV_REPAIRED_FROM_NPC (variable min 14 bytes, server updates money and inventory after NPC repair)
- 0x7ce: GSV_SET_ITEM_LIFE (16 bytes, server updates item durability for equipped items)
- 0x7d1: GSV_PARTY_REPLY (11 bytes, server sends reply to party requests with status codes like accept/reject/busy/not found)
- 0x7d2: GSV_PARTY_MEMBER (variable min 16 bytes, server sends party member updates to party members)
- 0x7d3: GSV_PARTY_ITEM (16 bytes, server broadcasts party item pickup to party members in ordered distribution mode)
- 0x7d4: GSV_PARTY_LEVnEXP (11 bytes, server sends party level and experience updates to party members)
- 0x7d5: GSV_CHANGE_OBJIDX (26 bytes, server updates party member object index and status info when member changes zones or reconnects)
- 0x7d6: GSV_ADD_EVENTOBJ (14 bytes, server adds event objects to zones with position, event ID, and status for dynamic zone events)
- 0x7d7: CLI_PARTY_RULE (7 bytes, client sends party rule change request to server) / GSV_PARTY_RULE (7 bytes, server broadcasts party rule change to party members)
- 0x7d8: CLI_ITEM_RESULT_REPORT (7 bytes, client reports item creation/upgrade results to server) / GSV_ITEM_RESULT_REPORT (10 bytes, server broadcasts item creation/upgrade results to nearby players)
- 0x7d9: CLI_MALL_ITEM_REQ (variable min 9 bytes, client sends mall operations like list items, bring to inventory, find character for gifting, or give item) / GSV_MALL_ITEM_REPLY (variable min 8 bytes, server responds to mall operations with results and updated data)
- 0x7da: CLI_MOVE_ZULY (15 bytes, client sends zuly movement request between inventory and bank) / GSV_MOVE_ZULY (22 bytes, server responds with updated balances after zuly movement)
- 0x7db: GSV_SET_NPC_SHOW (9 bytes, server broadcasts to sectors when NPC visibility changes, client shows/hides NPC based on flag for dynamic NPC behavior)
- 0x7dc: GSV_GODDNESS_MODE (9 bytes, server broadcasts goddess mode status to nearby players for fairy aura system that provides stat bonuses)
- 0x7dd: CLI_CART_RIDE / GSV_CART_RIDE (11 bytes, client sends cart ride requests/responses, server broadcasts cart ride status to nearby players for 2-person cart riding system)
- 0x7de: GSV_BILLING_MESSAGE_EXT (variable min 28 bytes, game server sends extended billing notifications like payment type, expiration dates/times, alerts, and kickout messages for different regions)
- 0x7df: GSV_BILLING_MESSAGE (variable min 7 bytes, game server sends billing notifications like payment type, expiration warnings, kickout messages)
- 0x7e0: CLI_CLAN_COMMAND (variable min 7 bytes, client sends clan management commands like create, invite, remove, promote, demote, quit, roster, leader) / WSV_CLAN_COMMAND (variable min 7 bytes, world server sends clan command responses like create/join results, member updates, kick notifications)
- 0x7e1: CLI_MESSENGER (variable min 7 bytes, client sends friend management commands like add, delete, status change) / WSV_MESSENGER (variable, world server sends friend management responses)
- 0x7e2: CLI_MESSENGER_CHAT (variable min 10 bytes, client sends private message to friend) / WSV_MESSENGER_CHAT (variable min 10 bytes, world server forwards private message to recipient)
- 0x7e3: CLI_CHATROOM (variable min 7 bytes, client sends chatroom commands like make, join, list, kick) / WSV_CHATROOM (variable, world server sends chatroom responses)
- 0x7e5: CLI_MEMO (variable min 7 bytes, client sends memo requests) / WSV_MEMO (variable, world server sends memo responses)
- 0x7e4: CLI_CHATROOM_MSG (variable min 7 bytes, client sends chat message to chatroom) / WSV_CHATROOM_MSG (variable min 9 bytes, game server broadcasts chat message in chatroom)
- 0x7e6: CLI_CLANMARK_SET (variable min 8 bytes, client sends clanmark data to set/update)
- 0x7e7: CLI_CLANMARK_REQ (8 bytes, client request) / WSV_CLANMARK_REPLY (variable min 10 bytes, world server response)
- 0x7e8: CLI_CLANMARK_REG_TIME (4 bytes, client request) / WSV_CLANMARK_REG_TIME (11 bytes, world server response)
- 0x7e9: GSV_PATSTATE_CHANGE (13 bytes, server notifies pet/cart state changes)
- 0x7ea: GSV_CHARSTATE_CHANGE (10 bytes, server notifies character state changes)
- 0x7eb: CLI_SCREEN_SHOT_TIME (6 bytes, client request) / GSV_SCREEN_SHOT_TIME (10 bytes, server response)
- 0x808: CLI_FAKE_GAMEGUARD_REQ / GSV_FAKE_GAMEGUARD_REPLY (4 bytes, not implemented in source)
- 0x7ad: CLI_BANK_LIST_REQ / GSV_BANK_LIST_REPLY (variable, client requests bank access with password, server replies with bank item list or status)
- 0x7ae: CLI_MOVE_ITEM / GSV_MOVE_ITEM (variable, client moves item between inventory and bank, server confirms with updated item data and optional money)
- 0x7b0: GSV_SKILL_LEARN_REPLY (12 bytes, server replies to skill learning request with result codes like success, need job, need ability, out of points)
- 0x7b1: CLI_SKILL_LEVELUP_REQ / GSV_SKILL_LEVELUP_REPLY (10 bytes CLI, 12 bytes GSV, client requests skill level up, server replies with result codes like success, need ability, out of points)
- 0x7b2: CLI_SELF_SKILL / GSV_SELF_SKILL (7 bytes CLI, variable GSV min 8 bytes, client uses self-targeting skill, server broadcasts skill use to nearby players with optional NPC motion data)
- 0x7b3: CLI_TARGET_SKILL / GSV_TARGET_SKILL (9 bytes CLI, variable GSV min 22 bytes, client uses skill on target object, server broadcasts skill use to nearby players with position correction and optional NPC motion data)
- 0x7b4: CLI_POSITION_SKILL / GSV_POSITION_SKILL (15 bytes CLI, variable GSV min 18 bytes, client uses skill on ground position, server broadcasts skill use to nearby players with optional NPC motion data)
- 0x7b5: GSV_EFFECT_OF_SKILL (12-15 bytes, server broadcasts skill effect result to nearby players with success bits and caster intelligence)
- 0x7b6: GSV_DAMAGE_OF_SKILL (variable min 14-17 bytes, server broadcasts skill damage to nearby players with damage amount and optional drop items)
- 0x7b7: GSV_CLEAR_STATUS (variable min 12 bytes, server broadcasts status effect removal to nearby players with remaining flags and adjusted stat values)
- 0x7b8: GSV_SPEED_CHANGED (14 bytes, server broadcasts speed change to nearby players with run speed, attack speed, weight rate, and force flag)
- 0x7b9: GSV_RESULT_OF_SKILL (10 bytes, server broadcasts skill result to nearby players)
- 0x7ba: CLI_APPRAISAL_REQ / GSV_APPRAISAL_REPLY (8 bytes, client requests item appraisal from NPC, server replies with result code)
- 0x7bb: GSV_SKILL_START (8 bytes, server broadcasts skill casting start to nearby players)
- 0x7bc: CLI_CRAFT_ITEM_REQ / GSV_CRAFT_ITEM_REPLY (7 bytes CLI, variable GSV min 8 bytes, client requests crafting operation, server replies with result and updated inventory items)
- 0x7bd: GSV_SKILL_CANCEL (9 bytes, server broadcasts skill cancel to nearby players with reason code)
- 0x790: GSV_SET_EVENT_STATUS (10 bytes, server broadcasts event status change to nearby players for dynamic NPC behavior)
- 0x791: GSV_NPC_CHAR (variable, server sends NPC character data to client when entering zone or spawning)
- 0x792: GSV_MOB_CHAR (variable, server sends monster character data to client when entering zone or spawning)
- 0x793: GSV_AVT_CHAR (variable, server sends player character data to client when entering zone or spawning)
- 0x794: GSV_SUB_OBJECT (8 bytes, server removes objects from client view)
- 0x795: CLI_SETPOS (14 bytes, client sets position)
- 0x796: CLI_STOP / GSV_STOP (16 bytes CLI, 18 bytes GSV, client stops movement, server broadcasts stop)
- 0x797: GSV_MOVE (23 bytes, server broadcasts movement to nearby players)
- 0x798: CLI_ATTACK / GSV_ATTACK (8 bytes CLI, 20 bytes GSV, client attacks target, server broadcasts attack)
- 0x799: CLI_DAMAGE / GSV_DAMAGE (10 bytes CLI, variable GSV min 14 bytes, client sends damage request, server broadcasts damage with optional drops)
- 0x79a: CLI_MOUSECMD / GSV_MOUSECMD (18 bytes CLI, 22 bytes GSV, client sends mouse command, server broadcasts)
- 0x79b: GSV_SETEXP (18 bytes, server sends experience gain to player)
- 0x79e: GSV_LEVELUP (22 bytes, server broadcasts level up to nearby players)
- 0x79f: CLI_HP_REQ / GSV_HP_REPLY (8 bytes CLI, 12 bytes GSV, client requests HP of object, server replies)
- 0x7a0: GSV_SET_HPnMP (12 bytes, server broadcasts HP/MP change to nearby players)
- 0x7bf: CLI_SET_WISHITEM (15 bytes, client sets wish item in personal store)
- 0x7c0: GSV_TRADE_P2P (10 bytes, server sends P2P trade response with result code, object index, and trade slot for trade initiation/acceptance/rejection)
- 0x7c1: GSV_TRADE_P2P_ITEM (15 bytes, server sends when an item is added to a P2P trade, including slot and item data)
- 0x7c2: CLI_P_STORE_OPEN (variable min 8 bytes, client opens personal store with items for sale and wish list) / GSV_P_STORE_OPENED (variable min 8 bytes, server broadcasts personal store opened to nearby players)
- 0x7c3: CLI_P_STORE_CLOSE (6 bytes, client closes personal store) / GSV_P_STORE_CLOSED (8 bytes, server broadcasts personal store closed to nearby players)
- 0x7c4: CLI_P_STORE_LIST_REQ (8 bytes, client requests personal store item list) / GSV_P_STORE_LIST_REPLY (variable min 8 bytes, server replies with personal store item list)
- 0x7c5: CLI_P_STORE_BUY_REQ (variable min 9 bytes, client requests to buy items from personal store)
- 0x7c6: CLI_P_STORE_SELL_REQ (variable min 9 bytes, client requests to sell items to personal store) / GSV_P_STORE_RESULT (variable min 10 bytes, server sends result of personal store buy/sell transaction)
- 0x7c7: GSV_P_STORE_MONEYnINV (variable min 14 bytes, server updates money and inventory after personal store transaction)
- 0x7ca: CLI_ASSEMBLE_RIDE_ITEM (6 bytes, client assembles ride item) / GSV_ASSEMBLE_RIDE_ITEM (6 bytes, server responds to ride item assembly)
- 0x7cb: CLI_USE_ITEM_TO_REPAIR (variable min 6 bytes, client uses item to repair equipment) / GSV_USED_ITEM_TO_REPAIR (variable min 6 bytes, server updates inventory after item repair)
- 0x7cd: CLI_REPAIR_FROM_NPC (8 bytes, client requests repair from NPC) / GSV_REPAIRED_FROM_NPC (variable min 14 bytes, server updates money and inventory after NPC repair)
- 0x7ce: GSV_SET_ITEM_LIFE (16 bytes, server updates item durability for equipped items)
- 0x7d1: GSV_PARTY_REPLY (11 bytes, server sends reply to party requests with status codes like accept/reject/busy/not found)
- 0x7d2: GSV_PARTY_MEMBER (variable min 16 bytes, server sends party member updates to party members)
- 0x7d3: GSV_PARTY_ITEM (16 bytes, server broadcasts party item pickup to party members in ordered distribution mode)
- 0x7d4: GSV_PARTY_LEVnEXP (11 bytes, server sends party level and experience updates to party members)
- 0x7d5: GSV_CHANGE_OBJIDX (26 bytes, server updates party member object index and status info when member changes zones or reconnects)
- 0x7d6: GSV_ADD_EVENTOBJ (14 bytes, server adds event objects to zones with position, event ID, and status for dynamic zone events)
- 0x7d7: CLI_PARTY_RULE (7 bytes, client sends party rule change request to server) / GSV_PARTY_RULE (7 bytes, server broadcasts party rule change to party members)
- 0x7d8: CLI_ITEM_RESULT_REPORT (7 bytes, client reports item creation/upgrade results to server) / GSV_ITEM_RESULT_REPORT (10 bytes, server broadcasts item creation/upgrade results to nearby players)
- 0x7d9: CLI_MALL_ITEM_REQ (variable min 9 bytes, client sends mall operations like list items, bring to inventory, find character for gifting, or give item) / GSV_MALL_ITEM_REPLY (variable min 8 bytes, server responds to mall operations with results and updated data)
- 0x7da: CLI_MOVE_ZULY (15 bytes, client sends zuly movement request between inventory and bank) / GSV_MOVE_ZULY (22 bytes, server responds with updated balances after zuly movement)
- 0x7db: GSV_SET_NPC_SHOW (9 bytes, server broadcasts to sectors when NPC visibility changes, client shows/hides NPC based on flag for dynamic NPC behavior)
- 0x7dc: GSV_GODDNESS_MODE (9 bytes, server broadcasts goddess mode status to nearby players for fairy aura system that provides stat bonuses)
- 0x7dd: CLI_CART_RIDE / GSV_CART_RIDE (11 bytes, client sends cart ride requests/responses, server broadcasts cart ride status to nearby players for 2-person cart riding system)
- 0x7de: GSV_BILLING_MESSAGE_EXT (variable min 28 bytes, game server sends extended billing notifications like payment type, expiration dates/times, alerts, and kickout messages for different regions)
- 0x7df: GSV_BILLING_MESSAGE (variable min 7 bytes, game server sends billing notifications like payment type, expiration warnings, kickout messages)
- 0x7e0: CLI_CLAN_COMMAND (variable min 7 bytes, client sends clan management commands like create, invite, remove, promote, demote, quit, roster, leader) / WSV_CLAN_COMMAND (variable min 7 bytes, world server sends clan command responses like create/join results, member updates, kick notifications)
- 0x7e1: CLI_MESSENGER (variable min 7 bytes, client sends friend management commands like add, delete, status change) / WSV_MESSENGER (variable, world server sends friend management responses)
- 0x7e2: CLI_MESSENGER_CHAT (variable min 10 bytes, client sends private message to friend) / WSV_MESSENGER_CHAT (variable min 10 bytes, world server forwards private message to recipient)
- 0x7e3: CLI_CHATROOM (variable min 7 bytes, client sends chatroom commands like make, join, list, kick) / WSV_CHATROOM (variable, world server sends chatroom responses)
- 0x7e5: CLI_MEMO (variable min 7 bytes, client sends memo requests) / WSV_MEMO (variable, world server sends memo responses)
- 0x7e4: CLI_CHATROOM_MSG (variable min 7 bytes, client sends chat message to chatroom) / WSV_CHATROOM_MSG (variable min 9 bytes, game server broadcasts chat message in chatroom)
- 0x7e6: CLI_CLANMARK_SET (variable min 8 bytes, client sends clanmark data to set/update)
- 0x7e7: CLI_CLANMARK_REQ (8 bytes, client request) / WSV_CLANMARK_REPLY (variable min 10 bytes, world server response)
- 0x7e8: CLI_CLANMARK_REG_TIME (4 bytes, client request) / WSV_CLANMARK_REG_TIME (11 bytes, world server response)
- 0x7e9: GSV_PATSTATE_CHANGE (13 bytes, server notifies pet/cart state changes)
- 0x7ea: GSV_CHARSTATE_CHANGE (10 bytes, server notifies character state changes)
- 0x7eb: CLI_SCREEN_SHOT_TIME (6 bytes, client request) / GSV_SCREEN_SHOT_TIME (10 bytes, server response)
- 0x808: CLI_FAKE_GAMEGUARD_REQ / GSV_FAKE_GAMEGUARD_REPLY (4 bytes, not implemented in source)