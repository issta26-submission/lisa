// Lightweight C++11 unit test suite for TetrahedralInterp16 (cmsintrp.c)
// This test framework is handcrafted (no GoogleTest) and uses
// non-terminating assertions to maximize coverage.
// The tests exercise different branch paths of TetrahedralInterp16 by
// configuring cmsInterpParams and inputs accordingly.

#include <cstring>
#include <vector>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <sstream>


// Include LittleCMS internal/header definitions.
// Adjust the include path as needed in your build environment.

// Forward declaration of the focal function (from cmsintrp.c).
extern "C" void TetrahedralInterp16(CMSREGISTER const cmsUInt16Number Input[],
                                   CMSREGISTER cmsUInt16Number Output[],
                                   CMSREGISTER const cmsInterpParams* p);

// Lightweight non-terminating assertion framework
static std::vector<std::string> gFailures;

#define EXPECT_EQ(a, b, msg) do { \
    if (!((a) == (b))) { \
        std::ostringstream oss; \
        oss << (msg) << " | got: " << (a) << " expected: " << (b); \
        gFailures.push_back(oss.str()); \
    } \
} while (0)

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::ostringstream oss; \
        oss << (msg) << " | condition is false"; \
        gFailures.push_back(oss.str()); \
    } \
} while (0)


// Test 1: Branch path when rx >= ry and ry >= rz (path 1)
// Scenario: rx = 0, ry = 0, rz = 0 (all rests zero) -> ensures deterministic simple path.
// Expect outputs to be sequential lut values: lut[0], lut[1], ...
void test_TetrahedralInterp16_Branch1_PathRxRyRzZero() {
    // Lut table with known values
    const cmsUInt16Number lut[] = { 10, 20, 30, 40, 50, 60, 70, 80 };
    cmsInterpParams p;
    std::memset(&p, 0, sizeof(p));
    p.Table = (void*)lut;
    p.nOutputs = 2;
    p.Domain[0] = p.Domain[1] = p.Domain[2] = 1;
    // opTA values to control X1, Y1, Z1
    p.opta[0] = p.opta[1] = p.opta[2] = 1;

    cmsUInt16Number Input[3] = {0, 0, 0};      // rx = ry = rz = 0
    cmsUInt16Number Output[4] = {0, 0, 0, 0};

    // Call the focal function
    TetrahedralInterp16(Input, Output, &p);

    // Expectation: outputs are lut[0], lut[1] for two outputs
    EXPECT_EQ(Output[0], lut[0], "Branch1: first output should equal lut[0]");
    EXPECT_EQ(Output[1], lut[1], "Branch1: second output should equal lut[1]");
}

// Test 2: Branch path when rx >= ry and rz >= rx but ry < rz (path 2)
// Scenario: rx = 0, ry = 0, rz > 0 (e.g., rz = non-zero via Input[2])
void test_TetrahedralInterp16_Branch2_RzNonZero() {
    const cmsUInt16Number lut[] = { 11, 22, 33, 44, 55, 66, 77, 88 };
    cmsInterpParams p;
    std::memset(&p, 0, sizeof(p));
    p.Table = (void*)lut;
    p.nOutputs = 2;
    p.Domain[0] = p.Domain[1] = p.Domain[2] = 1;
    p.opta[0] = p.opta[1] = p.opta[2] = 1;

    cmsUInt16Number Input[3] = {0, 0, 1}; // rz will be > 0
    cmsUInt16Number Output[4];
    TetrahedralInterp16(Input, Output, &p);

    // Expect the same sequential lut pattern as Test 1 for the first two outputs
    EXPECT_EQ(Output[0], lut[0], "Branch2: first output should equal lut[0]");
    EXPECT_EQ(Output[1], lut[1], "Branch2: second output should equal lut[1]");
}

// Test 3: Branch path when rx < ry and rx >= rz (path 4 in else block)
// Scenario: rx = 0, ry > 0, rz = 0 (Input {0,1,0})
void test_TetrahedralInterp16_Branch3_RxLtRy_RxGeRz() {
    const cmsUInt16Number lut[] = { 15, 25, 35, 45, 55, 65, 75, 85 };
    cmsInterpParams p;
    std::memset(&p, 0, sizeof(p));
    p.Table = (void*)lut;
    p.nOutputs = 2;
    p.Domain[0] = p.Domain[1] = p.Domain[2] = 1;
    p.opta[0] = p.opta[1] = p.opta[2] = 1;

    cmsUInt16Number Input[3] = {0, 1, 0}; // rx=0, ry>0, rz=0
    cmsUInt16Number Output[4];
    TetrahedralInterp16(Input, Output, &p);

    EXPECT_EQ(Output[0], lut[0], "Branch3: first output should equal lut[0]");
    EXPECT_EQ(Output[1], lut[1], "Branch3: second output should equal lut[1]");
}

// Test 4: Branch path when rx < ry and ry >= rz (path 5 in else block)
// Scenario: rx = 0, ry > 0, rz <= ry (e.g., rz = 1)
void test_TetrahedralInterp16_Branch4_RxLtRy_RyGeRz() {
    const cmsUInt16Number lut[] = { 18, 28, 38, 48, 58, 68, 78, 88 };
    cmsInterpParams p;
    std::memset(&p, 0, sizeof(p));
    p.Table = (void*)lut;
    p.nOutputs = 2;
    p.Domain[0] = p.Domain[1] = p.Domain[2] = 1;
    p.opta[0] = p.opta[1] = p.opta[2] = 1;

    cmsUInt16Number Input[3] = {0, 1, 1}; // rx=0, ry>0, rz>0, ry >= rz -> path 5
    cmsUInt16Number Output[4];
    TetrahedralInterp16(Input, Output, &p);

    EXPECT_EQ(Output[0], lut[0], "Branch4: first output should equal lut[0]");
    EXPECT_EQ(Output[1], lut[1], "Branch4: second output should equal lut[1]");
}


// Runner
int main() {
    // Run tests
    test_TetrahedralInterp16_Branch1_PathRxRyRzZero();
    test_TetrahedralInterp16_Branch2_RzNonZero();
    test_TetrahedralInterp16_Branch3_RxLtRy_RxGeRz();
    test_TetrahedralInterp16_Branch4_RxLtRy_RyGeRz();

    // Report results
    if (gFailures.empty()) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "There were " << gFailures.size() << " test failures:\n";
        for (const auto& msg : gFailures) {
            std::cerr << "- " << msg << "\n";
        }
        return 1;
    }
}