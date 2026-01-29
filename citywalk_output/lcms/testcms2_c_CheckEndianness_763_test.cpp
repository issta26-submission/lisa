/* 
   Test Suite A: CheckEndianness (no CMS_USE_BIG_ENDIAN macro defined at compile time)
   Target: compile this translation unit in an environment where CMS_USE_BIG_ENDIAN is NOT defined.
   Purpose: verify that the focal function CheckEndianness returns the expected value when the
            host endianness is taken into account, without triggering the misconfiguration path
            (i.e., the path that calls Die and returns 0).

   Notes:
   - This test relies on a typical little-endian machine (e.g., x86). On such platforms,
     BigEndian will be false and the code path should return 1 (IsOk == true).
   - We avoid invoking the Die path by ensuring the test is run on a little-endian machine
     with CMS_USE_BIG_ENDIAN not defined.
   - The test is self-contained and uses a small runtime endianness detector to establish
     the expected result.
   - This file does not depend on GoogleTest or any external framework; it uses a simple
     inline test harness.

   Dependency hints:
   - Focal method: cmsInt32Number CheckEndianness(void)
   - Focal dependencies and symbols (as shown in the provided class deps block):
     Die(const char*, ...), cmsInt32Number (alias for integral return type), CMS_USE_BIG_ENDIAN macro
   - The test calls the focal function via C linkage.
*/

#include <cstring>
#include <iostream>
#include <testcms2.h>
#include <cassert>
#include <cstdlib>
#include <cstdint>


// Declaration of the focal function from the repository.
// We assume cmsInt32Number is equivalent to int on the target platform.
extern "C" int CheckEndianness(void);

static bool isMachineLittleEndian() {
    uint32_t x = 0x01020304;
    unsigned char *p = reinterpret_cast<unsigned char*>(&x);
    // If the least-significant byte is stored at the lowest address, it's little-endian.
    return p[0] == 0x04;
}

// Basic test harness
static void Run_CheckEndianness_NoMacro_Test() {
    bool machineLittleEndian = isMachineLittleEndian();
    bool machineBigEndian = !machineLittleEndian;

    // When CMS_USE_BIG_ENDIAN is NOT defined, the logic is:
    // BigEndian = (u.c[sizeof(long)-1] == 1);
    // If machineBigEndian: IsOk = !BigEndian  -> IsOk = false -> returns 0
    // If machineLittleEndian: IsOk = !BigEndian -> IsOk = true  -> returns 1
    // Therefore, for the current run:
    // expected = !machineBigEndian
    int expected = machineBigEndian ? 0 : 1;

    int result = CheckEndianness();

    std::cout << "[Test NoMacro] MachineBigEndian=" << (machineBigEndian ? "Yes" : "No")
              << ", CMS_USE_BIG_ENDIAN=Not Defined, Expect=" << expected
              << ", Got=" << result << std::endl;

    if (result != expected) {
        std::cerr << "ERROR: CheckEndianness() did not return the expected value for the NoMacro build." << std::endl;
        std::exit(EXIT_FAILURE);
    } else {
        std::cout << "PASS: CheckEndianness() returned the expected value for NoMacro build." << std::endl;
    }
}

int main() {
    Run_CheckEndianness_NoMacro_Test();
    return 0;
}

/* 
   Rationale and coverage notes:
   - This test covers the true branch of the core predicate when the CMS_USE_BIG_ENDIAN macro is not defined
     and the machine is little-endian, which should yield IsOk = true and return 1.
   - It indirectly exercises the endianness-detection logic used by the focal method (u.l = 1 and
     the BigEndian computation). 
   - It does not attempt to flip CMS_USE_BIG_ENDIAN at runtime (not feasible with the provided code),
     but validates the intended outcome for the common little-endian environment without the macro defined.
*/


/* ------------------------------------------------------------------------- */
/* 
   Test Suite B: CheckEndianness (CMS_USE_BIG_ENDIAN macro defined at compile time)
   Target: compile this translation unit in an environment where CMS_USE_BIG_ENDIAN is defined.
   Purpose: exercise the alternative compilation path where the macro is defined and ensure the
            endianness logic aligns with the compile-time macro behavior.

   Important caveat:
   - On most hosts (little-endian), the focal function will take the path:
       IsOk = BigEndian;
     If the machine is little-endian (BigEndian == false), the function will yield IsOk = false and
     call Die(...) and return 0. This can interfere with automated testing if Die terminates the test program.
   - To minimize disruption, this test focuses on asserting the relationship between machine endianness
     and the function's return value under the CMS_USE_BIG_ENDIAN defined scenario. It will fail
     safely if the rejection path is hit, signaling that the macro-based configuration is not compatible
     with the current hardware endianness.

   Build instructions (outside this code block):
   - Compile with: -DCMS_USE_BIG_ENDIAN
   - Ensure the test binary links against the same test CMS codebase providing CheckEndianness.

   Notes:
   - We implement a runtime check for machine endianness, and derive the expected result under CMS_USE_BIG_ENDIAN == defined.
     For defined macro:
         IsOk = BigEndian
         Return value = (BigEndian)
     So expected = machineBigEndian.
*/

extern "C" int CheckEndianness(void);

static bool isMachineBigEndianRuntime() {
    uint32_t x = 0x01020304;
    unsigned char *p = reinterpret_cast<unsigned char*>(&x);
    return p[0] == 0x01;
}

static void Run_CheckEndianness_WithMacro_Test() {
    bool machineBigEndian = isMachineBigEndianRuntime();
    // When CMS_USE_BIG_ENDIAN is defined, the code sets IsOk = BigEndian.
    // Therefore, the expected result is exactly machineBigEndian.
    int expected = machineBigEndian ? 1 : 0;
    int result = CheckEndianness();

    std::cout << "[Test WithMacro] MachineBigEndian=" << (machineBigEndian ? "Yes" : "No")
              << ", CMS_USE_BIG_ENDIAN=Defined, Expect=" << expected
              << ", Got=" << result << std::endl;

    if (result != expected) {
        std::cerr << "ERROR: CheckEndianness() did not return the expected value for the WithMacro build." << std::endl;
        std::exit(EXIT_FAILURE);
    } else {
        std::cout << "PASS: CheckEndianness() returned the expected value for WithMacro build." << std::endl;
    }
}

int main() {
    Run_CheckEndianness_WithMacro_Test();
    return 0;
}