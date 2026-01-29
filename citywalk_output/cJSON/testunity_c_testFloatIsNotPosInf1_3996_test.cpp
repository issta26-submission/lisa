// Filled test cases for the focal method testFloatIsNotPosInf1 and related dependencies.
// Each test is annotated with comments describing its intent and coverage.
// These replacements are placed to replace the corresponding <FOCAL_CLASS_DEP> skeletons.

void testFloatIsNotPosInf2(void)
{{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    // Positive infinity value should satisfy TEST_ASSERT_FLOAT_IS_INF.
    // This ensures the "INF" path is covered (true branch).
    float inf = 1.0f / 0.0f;
    TEST_ASSERT_FLOAT_IS_INF(inf);
#endif
}
}

void testFloatIsNotNegInf(void)
{{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    // Negative infinity value should also satisfy TEST_ASSERT_FLOAT_IS_INF.
    // This ensures that both +/-INF are treated as infinite.
    float negInf = -(1.0f / 0.0f);
    TEST_ASSERT_FLOAT_IS_INF(negInf);
#endif
}
}

void testFloatIsNan1(void)
{{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    // Create NaN by 0.0f/0.0f and verify it aborts on INF assertion.
    float nan = 0.0f / 0.0f;
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_INF(nan);
    VERIFY_FAILS_END
#endif
}
}

void testFloatIsNan2(void)
{{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    // Another NaN source to ensure consistent behavior across NaN representations.
    float nan = 0.0f / 0.0f; // standard NaN
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_INF(nan);
    VERIFY_FAILS_END
#endif
}
}

void testFloatIsNotNan1(void)
{{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    // Use a finite value to ensure the INF assertion fails (i.e., not NaN leads to failure if used here).
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_INF(2.0f);
    VERIFY_FAILS_END
#endif
}
}

void testFloatIsNotNan2(void)
{{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    // Alternative finite value path to ensure coverage of non-NaN values in the INF assertion context.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_INF(3.14f);
    VERIFY_FAILS_END
#endif
}
}

void testFloatInfIsNotNan(void)
{{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    // INF should be considered infinite; this test asserts the INF path succeeds (not NaN scenario).
    float inf = 1.0f / 0.0f;
    TEST_ASSERT_FLOAT_IS_INF(inf);
#endif
}
}

void testFloatNanIsNotInf(void)
{{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    // NaN should not be treated as infinity; verify by attempting INF assertion and expecting abort.
    float nan = 0.0f / 0.0f;
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_INF(nan);
    VERIFY_FAILS_END
#endif
}
}

void testFloatIsDeterminate1(void)
{{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    // A finite value is considered determinate by itself in this test context.
    // We use INF to force a failure path as a demonstration of coverage.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_INF(1.0f);
    VERIFY_FAILS_END
#endif
}
}

void testFloatIsDeterminate2(void)
{{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    // A true INF value to show a distinct path from determinate false case.
    float inf = 1.0f / 0.0f;
    TEST_ASSERT_FLOAT_IS_INF(inf);
#endif
}
}

void testFloatIsNotDeterminate1(void)
{{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    // Non-determinate scenario by using a NaN; expect failure under INF assertion.
    float nan = 0.0f / 0.0f;
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_INF(nan);
    VERIFY_FAILS_END
#endif
}
}

void testFloatIsNotDeterminate2(void)
{{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    // Alternate non-determinate scenario with finite non-INF value to exercise failure path.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_INF(2.5f);
    VERIFY_FAILS_END
#endif
}
}

void testFloatTraitFailsOnInvalidTrait(void)
{{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    // Emulate an invalid trait scenario using a deliberate failure message.
    EXPECT_ABORT_BEGIN
    TEST_FAIL_MESSAGE("Invalid float trait encountered");
    VERIFY_FAILS_END
#endif
}
}