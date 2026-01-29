# Raw LLM Coverage Results - Baseline Comparison

This directory contains the experimental results from using a **raw LLM (without LISA framework)** to generate test cases, serving as a baseline to demonstrate LISA's value-add.

## Overview

To isolate LISA's contribution, we conducted experiments where we directly prompted a state-of-the-art LLM to generate fuzz drivers **without** LISA's:
- API sequence generation framework
- Automated repair mechanism  
- Fuzzing-specific rules and constraints
- Few-shot examples

This "vanilla LLM" baseline represents what developers would achieve using off-the-shelf LLM APIs.

## Directory Structure

Each library subdirectory contains:
- `seeds/` - Successfully compiled test cases (if any)
- `log.txt` or similar files documenting generation statistics
- Coverage reports (where applicable)

Libraries evaluated:
- `cJSON/` - JSON parsing library
- `lcms/` - Little-CMS color management
- `libpcap/` - Packet capture library
- `libpng/` - PNG image library
- `re2/` - Regular expression library
- `sqlite3/` - SQLite database
- `zlib/` - Compression library

## Experimental Setup

- **LLM Model**: same model used by LISA
- **Prompt Strategy**: Simple zero-shot prompts requesting fuzz drivers
- **Time Budget**: 3 hours per library (equivalent to LISA)
- **No LISA Components**: No repair, no examples, no specialized framework

## Purpose

This baseline demonstrates:
1. **Success Rate Gap**: How many valid test cases can be generated without LISA's framework
2. **Coverage Gap**: The coverage difference between raw LLM and LISA
3. **Quality Gap**: Code quality and bug-finding effectiveness

## Expected Results

Without LISA's framework, we expect:
- ❌ **Lower compilation success rate** - More syntax errors, missing includes
- ❌ **Lower execution success rate** - More runtime crashes, invalid API usage
- ❌ **Lower coverage** - Less effective API exploration

## Key Insights

The comparison between `rawlllm_coverage/` and `LISA_full_coverage/` demonstrates:
- LISA's automated repair significantly improves compilation success
- Domain-specific rules guide more effective API usage
- Few-shot examples enhance generation quality
- The complete LISA framework is necessary for practical LLM-based fuzzing

## Notes

- This baseline is intentionally "naive" to establish a lower bound
- Results validate that LISA's framework provides substantial value beyond raw LLM capabilities
- The gap motivates the need for specialized frameworks like LISA
