// Unit tests for CAM02COLOR ChromaticAdaptation
// - No GTest, a lightweight test harness is used.
// - Tests focus on the ChromaticAdaptation function's numerical behavior.
// - Domain knowledge: uses the provided types CAM02COLOR and cmsCIECAM02 from the cms library.

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>
#include <cstring>


// Include the internal CMS headers to obtain the correct type definitions.
// The test assumes cmscam02.c (where ChromaticAdaptation is implemented) and
// associated headers are available in the build environment.

// Prototypes for C linkage to ensure correct symbol resolution when linking C++ test code with C code.
// If the header already provides a prototype with C linkage, this is a safety second declaration.
extern "C" CAM02COLOR ChromaticAdaptation(CAM02COLOR clr, cmsCIECAM02* pMod);

// Simple non-terminating assertion helper
static int g_failures = 0;
#define EXPECT(cond, msg) do { if(!(cond)) { std::cerr << "[FAIL] " << msg << std::endl; ++g_failures; } } while(0)

static bool almost_equal(double a, double b, double tol = 1e-6) {
    return std::fabs(a - b) <= tol;
}

// Entry point for tests
int main() {
    // Test 1: D = 0.0 => ChromaticAdaptation should return identical RGB values (RGBc[i] == RGB[i])
    {
        CAM02COLOR clr;
        std::memset(&clr, 0, sizeof(clr));
        clr.RGB[0] = 1.0;
        clr.RGB[1] = 2.0;
        clr.RGB[2] = 3.0;

        // Prepare modulation parameters
        cmsCIECAM02 mod;
        std::memset(&mod, 0, sizeof(mod));
        mod.D = 0.0;
        mod.adoptedWhite.XYZ[0] = 1.0;
        mod.adoptedWhite.XYZ[1] = 2.0; // used in formula as XYZ[1]
        mod.adoptedWhite.XYZ[2] = 3.0;
        mod.adoptedWhite.RGB[0] = 4.0;
        mod.adoptedWhite.RGB[1] = 5.0;
        mod.adoptedWhite.RGB[2] = 6.0;

        CAM02COLOR out = ChromaticAdaptation(clr, &mod);

        // Expected: since D == 0, factor = 1.0 for all i, so out.RGBc[i] == clr.RGB[i]
        EXPECT(almost_equal(out.RGBc[0], clr.RGB[0]), "Test1: RGBc[0] should equal RGB[0] when D=0");
        EXPECT(almost_equal(out.RGBc[1], clr.RGB[1]), "Test1: RGBc[1] should equal RGB[1] when D=0");
        EXPECT(almost_equal(out.RGBc[2], clr.RGB[2]), "Test1: RGBc[2] should equal RGB[2] when D=0");

        // Also ensure input RGB remains unchanged (since function takes clr by value, this is expected)
        EXPECT(almost_equal(clr.RGB[0], 1.0) && almost_equal(clr.RGB[1], 2.0) && almost_equal(clr.RGB[2], 3.0),
               "Test1: Input RGB should remain unchanged after ChromaticAdaptation");
    }

    // Test 2: D = 1.0 => factor = adoptedWhite.XYZ[1] / adoptedWhite.RGB[i]
    {
        CAM02COLOR clr;
        std::memset(&clr, 0, sizeof(clr));
        clr.RGB[0] = 1.0;
        clr.RGB[1] = 2.0;
        clr.RGB[2] = 3.0;

        cmsCIECAM02 mod;
        std::memset(&mod, 0, sizeof(mod));
        mod.D = 1.0;
        mod.adoptedWhite.XYZ[0] = 10.0;
        mod.adoptedWhite.XYZ[1] = 2.0;   // a
        mod.adoptedWhite.RGB[0] = 4.0;    // b0
        mod.adoptedWhite.RGB[1] = 5.0;    // b1
        mod.adoptedWhite.RGB[2] = 6.0;    // b2

        CAM02COLOR out = ChromaticAdaptation(clr, &mod);

        // Expected: out.RGBc[i] = (a / b[i]) * r[i]
        double a = mod.adoptedWhite.XYZ[1];
        double expected0 = (a / mod.adoptedWhite.RGB[0]) * clr.RGB[0];
        double expected1 = (a / mod.adoptedWhite.RGB[1]) * clr.RGB[1];
        double expected2 = (a / mod.adoptedWhite.RGB[2]) * clr.RGB[2];

        EXPECT(almost_equal(out.RGBc[0], expected0), "Test2: RGBc[0] = (a/b0)*R0 when D=1");
        EXPECT(almost_equal(out.RGBc[1], expected1), "Test2: RGBc[1] = (a/b1)*R1 when D=1");
        EXPECT(almost_equal(out.RGBc[2], expected2), "Test2: RGBc[2] = (a/b2)*R2 when D=1");
        // Also verify input RGB unchanged
        EXPECT(almost_equal(clr.RGB[0], 1.0) && almost_equal(clr.RGB[1], 2.0) && almost_equal(clr.RGB[2], 3.0),
               "Test2: Input RGB should remain unchanged after ChromaticAdaptation");
    }

    // Test 3: D = 0.5 => general case
    {
        CAM02COLOR clr;
        std::memset(&clr, 0, sizeof(clr));
        clr.RGB[0] = 1.5;
        clr.RGB[1] = 2.5;
        clr.RGB[2] = 3.5;

        cmsCIECAM02 mod;
        std::memset(&mod, 0, sizeof(mod));
        mod.D = 0.5;
        mod.adoptedWhite.XYZ[0] = 9.0;
        mod.adoptedWhite.XYZ[1] = 2.0; // a
        mod.adoptedWhite.RGB[0] = 4.0;  // b0
        mod.adoptedWhite.RGB[1] = 5.0;  // b1
        mod.adoptedWhite.RGB[2] = 10.0; // b2

        CAM02COLOR out = ChromaticAdaptation(clr, &mod);

        double a = mod.adoptedWhite.XYZ[1];
        double b0 = mod.adoptedWhite.RGB[0];
        double b1 = mod.adoptedWhite.RGB[1];
        double b2 = mod.adoptedWhite.RGB[2];
        double d = mod.D;

        double expected0 = ((a * (d / b0)) + (1.0 - d)) * clr.RGB[0];
        double expected1 = ((a * (d / b1)) + (1.0 - d)) * clr.RGB[1];
        double expected2 = ((a * (d / b2)) + (1.0 - d)) * clr.RGB[2];

        EXPECT(almost_equal(out.RGBc[0], expected0), "Test3: RGBc[0] general case");
        EXPECT(almost_equal(out.RGBc[1], expected1), "Test3: RGBc[1] general case");
        EXPECT(almost_equal(out.RGBc[2], expected2), "Test3: RGBc[2] general case");
        // Also verify input RGB unchanged
        EXPECT(almost_equal(clr.RGB[0], 1.5) && almost_equal(clr.RGB[1], 2.5) && almost_equal(clr.RGB[2], 3.5),
               "Test3: Input RGB should remain unchanged after ChromaticAdaptation");
    }

    // Summary
    if (g_failures == 0) {
        std::cout << "All ChromaticAdaptation tests passed." << std::endl;
    } else {
        std::cerr << g_failures << " ChromaticAdaptation test(s) failed." << std::endl;
    }

    return g_failures;
}