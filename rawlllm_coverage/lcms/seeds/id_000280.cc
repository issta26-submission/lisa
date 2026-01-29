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
//<ID> 280
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Create a context for thread-safe operations
    cmsContext ctx = cmsCreateContext(NULL, NULL);

    // Set alarm codes (all zeros for this test)
    cmsUInt16Number alarmCodes[16] = {0};
    cmsSetAlarmCodesTHR(ctx, alarmCodes);

    // Create an input (sRGB) and an output (XYZ) profile
    cmsHPROFILE inProfile = cmsCreate_sRGBProfileTHR(ctx);
    cmsHPROFILE outProfile = cmsCreateXYZProfile();

    // If profile creation failed, clean up and return non-success
    if (inProfile == NULL || outProfile == NULL) {
        if (inProfile) cmsCloseProfile(inProfile);
        if (outProfile) cmsCloseProfile(outProfile);
        return 1;
    }

    // Create a transform using the THR variant.
    // Use zero for formats/intents/flags as placeholders for this sequence test.
    cmsUInt32Number inputFormat = 0;
    cmsUInt32Number outputFormat = 0;
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;

    cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, inProfile, inputFormat,
                                                   outProfile, outputFormat,
                                                   intent, flags);

    // Load an IT8 structure from a small in-memory buffer, then free it.
    const char dummyIT8[] = "IT8-DUMMY";
    cmsHANDLE hIT8 = NULL;
    // If the loader API is available it will return a handle; ignore failures for this test.
    hIT8 = cmsIT8LoadFromMem(ctx, dummyIT8, (cmsUInt32Number)sizeof(dummyIT8));
    if (hIT8) {
        cmsIT8Free(hIT8);
        hIT8 = NULL;
    }

    // Close profiles
    cmsCloseProfile(inProfile);
    cmsCloseProfile(outProfile);

    // Note: transform cleanup API isn't listed, so we leave it as-is.

    // Indicate success per specification
    return 66;
}