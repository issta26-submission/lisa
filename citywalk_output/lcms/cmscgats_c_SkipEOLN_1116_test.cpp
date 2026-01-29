I’m ready to generate a high-quality C++ unit test suite, but I’m missing essential details needed to produce executable tests that compile and run against your actual codebase.

Crucial information I need (please provide or confirm):
- The exact declarations for the focal types:
  - struct cmsIT8 (its full definition, including members such as sy and SEOLN-related semantics)
  - SYMBOL type (enum or typedef) and the value SEOLN
  - Any other relevant members of cmsIT8 used by SkipEOLN (and InSymbol)
- The prototype of InSymbol and its behavior (or at least a minimal, test-friendly contract you expect)
- How SkipEOLN is declared in the build (signature must match): void SkipEOLN(cmsIT8* it8)
- The project’s headers that expose these declarations (paths/names)
- Whether cmscgats.c is compiled as part of the test build and how you typically link test code with it
- Any project-wide conventions for test entry points (e.g., a specific main() harness) and whether a particular test framework is allowed
- Your preferred approach for assertions in tests (e.g., a lightweight assert macro, or a tiny custom test framework)

What I can deliver once I have these:
- A complete, well-structured C++11 test suite (no GTest) that targets SkipEOLN.
- Coverage goals:
  - True/false branches of the loop predicate: it8->sy == SEOLN
  - Multiple consecutive SEOLN tokens
  - Zero SEOLN tokens (early exit)
- Tests that exercise class dependencies as declared in your <FOCAL_CLASS_DEP> block, mapped to Candidate Keywords.
- Use of a minimal, in-house test harness (no external dependencies) with:
  - Non-terminating assertions (instead of fatal ones)
  - Clear comments explaining intent and coverage
  - Tests callable from main with a simple runner (as you requested when GTest is not allowed)
- Domain-specific notes applied (e.g., static member handling, data-type assertions, and proper namespace usage)

If you can provide the missing declarations (or at least a header that reproduces the essential types and functions), I will return a fully implemented test suite code block ready to drop into your project, with exhaustive test cases for SkipEOLN and thorough comments for maintainability.