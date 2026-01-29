/*
 * Lightweight C++11 test suite for CheckLabInputGray (from testcms2.c)
 *
 * Notes:
 * - The tests exercise the focal method CheckLabInputGray by invoking it directly,
 *   and also exercise the dependent components that CheckLabInputGray relies on
 *   (Create_GrayLab, cmsCreateLab4Profile, cmsCreateTransform, and CheckGray).
 * - No GTest/GMock are used; a minimal test harness is implemented with non-terminating
 *   assertions (boolean checks) and a final summary.
 * - This file is intended to be compiled with a C++11 compiler and linked against
 *   the Little CMS library and the project’s testcms2.c (which provides the
 *   implementations for the dependent functions).
 * - All tests are self-contained and include explanatory comments.
 */

#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <iomanip>


// Include the project's test helpers and declarations

/*
 * Test 1: CheckLabInputGray_ReturnsTrue
 * Purpose: Verify that the focal method CheckLabInputGray() returns 1 (success)
 *          given the normal operating environment and dependencies.
 * Coverage: True-path of the method under test.
 */
bool Test_CheckLabInputGray_ReturnsTrue()
{
    // Call the focal method directly and expect a successful result.
    cmsInt32Number result = CheckLabInputGray();
    bool ok = (result == 1);
    if (!ok) {
        std::cerr << "[Fail] Test_CheckLabInputGray_ReturnsTrue: CheckLabInputGray() returned "
                  << result << " (expected 1)\n";
    } else {
        std::cout << "[Pass] Test_CheckLabInputGray_ReturnsTrue\n";
    }
    return ok;
}

/*
 * Test 2: CheckGray_Functionality_Via_InternalXForm
 * Purpose: Independently validate the dependent components by recreating the
 *          transformation pipeline used inside CheckLabInputGray and verifying
 *          the integrity of the gray-to-Lab mapping through CheckGray.
 * Coverage: Integration of dependencies (Create_GrayLab, cmsCreateLab4Profile,
 *           cmsCreateTransform, CheckGray, and proper resource cleanup).
 */
bool Test_CheckGray_Functionality_Via_InternalXForm()
{
    // Acquire a Gray-Lab based profile chain
    cmsHPROFILE hGray = Create_GrayLab();
    cmsHPROFILE hLab  = cmsCreateLab4Profile(NULL);
    if (hGray == NULL || hLab == NULL) {
        std::cerr << "[Fail] Test_CheckGray_Functionality_Via_InternalXForm: Unable to create profiles\n";
        if (hGray) cmsCloseProfile(hGray);
        if (hLab)  cmsCloseProfile(hLab);
        return false;
    }

    // Create a transform: Gray 8-bit -> Lab (Double precision)
    cmsHTRANSFORM xform = cmsCreateTransform(hGray, TYPE_GRAY_8, hLab, TYPE_Lab_DBL,
                                             INTENT_RELATIVE_COLORIMETRIC, 0);

    // Release profiles as in the focal function
    cmsCloseProfile(hGray);
    cmsCloseProfile(hLab);

    if (xform == NULL) {
        std::cerr << "[Fail] Test_CheckGray_Functionality_Via_InternalXForm: Unable to create transform\n";
        return false;
    }

    // Validate the key gray-to-Lab mappings used by the focal method
    bool ok = true;
    ok &= CheckGray(xform, 0, 0);
    ok &= CheckGray(xform, 125, 49.019);
    ok &= CheckGray(xform, 200, 78.431);
    ok &= CheckGray(xform, 255, 100.0);

    if (!ok) {
        std::cerr << "[Fail] Test_CheckGray_Functionality_Via_InternalXForm: One or more CheckGray mappings failed\n";
    } else {
        std::cout << "[Pass] Test_CheckGray_Functionality_Via_InternalXForm\n";
    }

    cmsDeleteTransform(xform);
    return ok;
}

/*
 * Test harness: runs all tests and reports a summary.
 * This is a non-terminating harness: all tests are executed even if some fail.
 */
int main(int argc, char* argv[])
{
    // Disable syncing with stdio for performance (optional)
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    struct TestCase {
        const char* name;
        bool (*fn)();
        const char* note;
    };

    std::vector<TestCase> tests = {
        {"CheckLabInputGray_ReturnsTrue", Test_CheckLabInputGray_ReturnsTrue, "Verifies the focal method returns success on valid environment."},
        {"CheckGray_Functionality_Via_InternalXForm", Test_CheckGray_Functionality_Via_InternalXForm,
         "Validates internal dependency chain by recreating the Gray->Lab transform and using CheckGray."}
    };

    int failures = 0;
    std::cout << "Running test suite for CheckLabInputGray and related dependencies...\n";

    for (const auto& t : tests) {
        bool pass = true;
        // Execute test
        pass = t.fn();

        // Report result
        std::cout << std::setw(40) << std::left << t.name << " : "
                  << (pass ? "PASS" : "FAIL");
        if (t.note) {
            std::cout << "  // " << t.note;
        }
        std::cout << "\n";

        if (!pass) ++failures;
    }

    std::cout << "\nSummary: " << (tests.size() - failures) << " / " << tests.size() << " tests passed.\n";

    return (failures == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}