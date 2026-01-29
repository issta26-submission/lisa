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
//<ID> 157
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Create a dedicated context for profile creation
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 1;

    // Create an OkLab profile in the created context
    cmsHPROFILE oklab = cmsCreate_OkLabProfile(ctx);
    if (oklab == NULL) return 2;

    // Create an sRGB profile using the same context (thread-safe variant)
    cmsHPROFILE srgb = cmsCreate_sRGBProfileTHR(ctx);
    if (srgb == NULL) return 3;

    // Query tag counts from both profiles
    cmsInt32Number tags_oklab = cmsGetTagCount(oklab);
    cmsInt32Number tags_srgb  = cmsGetTagCount(srgb);

    // Validate tag counts are non-negative
    if (tags_oklab < 0 || tags_srgb < 0) return 4;

    // Verify that the profiles were created in the same context we supplied
    cmsContext ctx_from_oklab = cmsGetProfileContextID(oklab);
    cmsContext ctx_from_srgb  = cmsGetProfileContextID(srgb);

    if (ctx_from_oklab != ctx || ctx_from_srgb != ctx) return 5;

    // All checks passed
    return 66;
}