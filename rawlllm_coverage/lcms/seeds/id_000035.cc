#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Use a NULL/default context (allowed by many lcms THR APIs)
    cmsContext ctx = nullptr;

    // Allocate an identity stage for 4 channels
    cmsStage *stage = cmsStageAllocIdentity(ctx, 4);
    if (stage == nullptr) {
        return 1;
    }

    // Create an sRGB profile (thread-aware variant)
    cmsHPROFILE srgb = cmsCreate_sRGBProfileTHR(ctx);
    if (srgb == nullptr) {
        return 2;
    }

    // Create an XYZ profile (no-context variant as requested)
    cmsHPROFILE xyz = cmsCreateXYZProfile();
    if (xyz == nullptr) {
        return 3;
    }

    // Query header flags from both profiles
    cmsUInt32Number flags_srgb = cmsGetHeaderFlags(srgb);
    cmsUInt32Number flags_xyz  = cmsGetHeaderFlags(xyz);

    // Prepare an MLU pointer (NULL here for demonstration) and set an UTF-8 string
    cmsMLU *mlu = nullptr;
    // LanguageCode must be a 3-char array; pass "en" and "US" as requested
    cmsBool ok = cmsMLUsetUTF8(mlu, "en", "US", "Example UTF-8 string");
    (void)ok;               // ignore the result for this test sequence
    (void)flags_srgb;       // silence unused-variable warnings
    (void)flags_xyz;

    // If we reached this point, consider the sequence successful
    return 66;
}