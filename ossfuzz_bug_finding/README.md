# OSS-Fuzz Bug Finding Results

This directory contains the bug-triggering test cases discovered by **OSS-Fuzz** during our comparative bug detection experiments.

## Overview

To provide a fair comparison with LISA's bug detection capability, we ran OSS-Fuzz (Google's continuous fuzzing platform) against the same set of 20 injected historical bugs used in our evaluation.

## Experimental Setup

- **Fuzzing Engine**: libFuzzer with AddressSanitizer (ASan)
- **Time Budget**: 6 hours per library
- **Bug Set**: 20 historical functional bugs across 4 libraries (5 bugs each)


## Directory Structure

Each library subdirectory contains the crash-triggering inputs or test cases:
- `cJSON/` - Bug-triggering cases for cJSON library
- `Little-CMS/` - Bug-triggering cases for Little-CMS library
- `sqlite/` - Bug-triggering cases for SQLite database
- `zlib/` - Bug-triggering cases for zlib compression library



## Results

OSS-Fuzz's bug detection results are compared with:
- **LISA** (10/20 bugs detected, 50%)
- Traditional fuzzing baseline

## Notes

- OSS-Fuzz excels at detecting memory safety bugs (buffer overflows, use-after-free)
- LISA demonstrates complementary strength in detecting functional/logic bugs
- Some bugs require specific API call sequences that random fuzzing may not easily discover
- The combination of fuzzing and LISA-style test generation provides the best coverage

## Limitations

- OSS-Fuzz primarily focuses on crash-inducing bugs
- Functional bugs without memory violations may not be detected
- Coverage-guided fuzzing may not efficiently explore complex API state spaces
