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
//<ID> 64
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Create a NULL profile
    cmsHPROFILE hProfile = cmsCreateNULLProfile();
    if (hProfile == NULL) {
        return 0; // failure creating profile
    }

    // Query a tag signature from the profile (index 0)
    cmsTagSignature tag = cmsGetTagSignature(hProfile, 0);

    // Get the context associated with the profile
    cmsContext ctx = cmsGetProfileContextID(hProfile);

    // Allocate an IT8 handle using the profile context (if available)
    cmsHANDLE hIT8 = NULL;
    if (ctx) {
        hIT8 = cmsIT8Alloc(ctx);
    }

    // Try to read a double value from the IT8 structure (if allocated)
    cmsFloat64Number value = 0.0;
    if (hIT8) {
        // Use sample patch/sample names that are plausible; function will return a double
        value = cmsIT8GetDataDbl(hIT8, "Patch1", "R");
    }

    // Query supported intents for the context (ask for up to 8)
    const cmsUInt32Number maxIntents = 8;
    cmsUInt32Number intentCodes[maxIntents];
    char *intentDescriptions[maxIntents];
    for (cmsUInt32Number i = 0; i < maxIntents; ++i) intentDescriptions[i] = NULL;

    cmsUInt32Number foundIntents = cmsGetSupportedIntentsTHR(ctx, maxIntents, intentCodes, intentDescriptions);

    // Create a simple multiprofile transform using the NULL profile
    cmsHPROFILE profiles[1] = { hProfile };
    cmsHTRANSFORM transform = cmsCreateMultiprofileTransform(profiles, 1,
                                                            /*InputFormat*/ 0,
                                                            /*OutputFormat*/ 0,
                                                            /*Intent*/ 0,
                                                            /*dwFlags*/ 0);

    // Query the output format of the created transform (if any)
    cmsUInt32Number outFormat = 0;
    if (transform) {
        outFormat = cmsGetTransformOutputFormat(transform);
    }

    // Minimal use of obtained values to avoid "unused variable" warnings
    // (No-op usage)
    (void)tag;
    (void)value;
    (void)foundIntents;
    (void)outFormat;

    // Cleanup: close the profile
    cmsCloseProfile(hProfile);

    // Success
    return 66;
}