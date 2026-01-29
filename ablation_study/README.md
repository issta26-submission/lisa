# Ablation Study Results

This directory contains the experimental results from the ablation study conducted to evaluate the contribution of each key component in the LISA framework.

## Overview

The ablation study isolates and measures the impact of three critical components:
1. **Examples** - Few-shot learning examples provided to the LLM
2. **Repair** - Automated syntax and compilation error repair mechanism
3. **Rules** - Domain-specific fuzzing rules and constraints

## Directory Structure

Each library subdirectory (cJSON, sqlite, zlib) contains four result files:

- `random_choose.txt` - Baseline: Random API selection without LISA's intelligent guidance
- `without_examples.txt` - LISA without few-shot examples (zero-shot generation)
- `without_repair.txt` - LISA without the automated repair mechanism
- `without_rules.txt` - LISA without domain-specific fuzzing rules

## Experimental Setup

- **Time Budget**: 3 hours per configuration per library
- **Baseline**: Full LISA configuration (with all components enabled)
- **Metric**: Number of successfully compiled and executed test cases

## How to Interpret Results

Each `.txt` file contains the statistics for a specific ablation configuration:
- Total generation attempts
- Compilation success rate
- Execution success rate
- Final seed count

Compare these metrics against the full LISA results (available in `LISA_full_coverage/`) to understand each component's contribution.

## Key Findings

The ablation study demonstrates that:
- **Examples** significantly improve generation quality and reduce compilation failures
- **Repair** mechanism recovers from common syntax errors and improves success rate
- **Rules** guide the LLM towards more effective API usage patterns

Removing any single component results in measurable performance degradation, validating LISA's integrated design.
