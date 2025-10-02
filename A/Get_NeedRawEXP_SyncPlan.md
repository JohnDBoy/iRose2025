## 2.1. Note on EXP Decreases at Chunk Boundaries (JDOEBOY)

It is intentional that some level chunks may require less EXP than the previous chunk. This design provides more control over leveling speed and progression, and allows for easier tuning of the game experience without modifying asset files. These transitions are not considered bugs, but a feature of the chunked EXP system. Future maintainers should be aware that this is by design. JDOEBOY

# Get_NeedRawEXP Synchronization & Extension Plan (JDOEBOY)

## Overview
This document outlines the plan to synchronize and extend the Get_NeedRawEXP function across all Calculation.cpp/.h files for levels up to MAX_LEVEL (255). The goal is to ensure consistent EXP progression, prevent overflow, and provide maintainable, well-documented logic for future updates.

---

## 1. Return Type Standardization
- All Get_NeedRawEXP functions will use `__int64` as the return type in both .cpp and .h files.
- This prevents overflow for high-level EXP values and ensures consistency between client, server, and shared code.

---


## 2. Level Ranges & Finalized Formulas (JDOEBOY)

**Original formulas (levels 1–150) are preserved. For 151+, new formulas are used, with the original math kept in comments for reference/reversion.**

### Level Ranges and Formulas

- **Level 1–15:**
	```cpp
	EXP = (LV + 3) * (LV + 5) * (LV + 10) * 0.7;
	// Original formula, do not modify. JDOEBOY
	```
- **Level 16–60:**
	```cpp
	EXP = (LV - 5) * (LV + 2) * (LV + 2) * 2.2;
	// Original formula, do not modify. JDOEBOY
	```
- **Level 61–113:**
	```cpp
	EXP = (LV - 11) * (LV) * (LV + 4) * 2.5;
	// Original formula, do not modify. JDOEBOY
	```
- **Level 114–150:**
	```cpp
	EXP = (LV - 31) * (LV - 20) * (LV + 4) * 3.8;
	// Original formula, do not modify. JDOEBOY
	```
- **Level 151–189:**
	```cpp
	// Original (now commented in code):
	// EXP = (LV - 67) * (LV - 20) * (LV - 10) * 6;
	// Extended formula for >150 (JDOEBOY):
	EXP = (LV - 67) * (LV - 20) * (LV - 10) * 6;
	```
- **Level 190–210:**
	```cpp
	// Extended formula for 190–210 (JDOEBOY):
	EXP = (LV - 100) * (LV - 50) * (LV - 10) * 8;
	```
- **Level 211–230:**
	```cpp
	// Extended formula for 211–230 (JDOEBOY):
	EXP = (LV - 120) * (LV - 70) * (LV - 20) * 10;
	```
- **Level 231–255:**
	```cpp
	// Extended formula for 231–255 (JDOEBOY):
	EXP = (LV - 150) * (LV - 100) * (LV - 50) * (LV - 10) * 12;
	```

### Notes
- All new formulas for 151+ are designed to smoothly extend the EXP curve, avoid negative/zero values, and are easy to tune.
- The original formulas for 151+ (if any) are preserved in code comments for possible reversion.
- All Calculation.cpp/.h files must use these formulas and boundaries.

---

---

## 3. MAX_LEVEL Definition
- Define `MAX_LEVEL` as 255 in all relevant headers.
- All Get_NeedRawEXP implementations will cap iLevel at MAX_LEVEL.

---

## 4. Edge Case Testing (Manual)
For each of the following levels, verify:
- EXP required is positive and increases smoothly
- No overflow or negative values
- Level-up occurs at correct EXP threshold

**Test Levels:**
- 1 (minimum)
- 15 (boundary)
- 16 (start of next chunk)
- 60, 61, 113, 114, 150, 151, 189, 190, 210, 211, 230, 231, 255 (all chunk boundaries)
- 254, 255 (max)

**Manual Test Steps:**
1. Set character to each test level
2. Grant just below, exactly, and just above the required EXP
3. Confirm level-up and EXP bar behavior

---

## 5. Implementation Notes
- All three Calculation.cpp/.h files must use identical formulas and boundaries
- Add clear English comments for each chunk, with `JDOEBOY` tag
- Document any future changes in this file and in code comments

---

## 6. Next Steps
- Finalize formulas for 190–255
- Update all Calculation.cpp/.h files
- Perform manual edge case testing
- Review and adjust as needed

---

*Drafted by GitHub Copilot, JDOEBOY, 2025-10-02*
