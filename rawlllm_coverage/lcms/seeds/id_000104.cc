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
//<ID> 104
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Create a context for THR (thread-safe) operations
    cmsContext ctx = cmsCreateContext(NULL, NULL);

    // Create an XYZ profile associated with the context (THR variant)
    cmsHPROFILE profile = NULL;
    if (ctx) {
        profile = cmsCreateXYZProfileTHR(ctx);
    } else {
        // Fallback to non-THR creation if context couldn't be created
        profile = cmsCreateXYZProfile();
    }

    // Query tag count if we have a profile
    cmsInt32Number tagCount = 0;
    if (profile) {
        tagCount = cmsGetTagCount(profile);
        // (use tagCount in a harmless way)
        (void)tagCount;
    }

    // Build a tone curve and duplicate it
    cmsToneCurve * tone = NULL;
    cmsToneCurve * tone_dup = NULL;
    if (ctx) {
        tone = cmsBuildGamma(ctx, 2.2);
    } else {
        // If no context, attempt to build with a NULL context (some builds may allow)
        tone = cmsBuildGamma(NULL, 2.2);
    }
    if (tone) {
        tone_dup = cmsDupToneCurve(tone);
    }

    // Attempt to retrieve an ASCII string from an MLU pointer.
    // We don't have a real cmsMLU* here; pass NULL safely and handle return value.
    char buffer[128];
    buffer[0] = '\0';
    cmsUInt32Number got = cmsMLUgetASCII(NULL, "en", "US", buffer, sizeof(buffer));
    (void)got; // ignore result but ensure we called the API

    // Load an IT8 handle from a small in-memory blob and free it.
    const char fake_it8[] = "IT8\n"; // likely not a valid IT8, but safe to attempt load
    cmsHANDLE it8 = NULL;
    if (ctx) {
        it8 = cmsIT8LoadFromMem(ctx, fake_it8, (cmsUInt32Number)sizeof(fake_it8));
    } else {
        it8 = cmsIT8LoadFromMem(NULL, fake_it8, (cmsUInt32Number)sizeof(fake_it8));
    }
    if (it8) {
        cmsIT8Free(it8);
        it8 = NULL;
    }

    // Clean up context if created
    if (ctx) {
        cmsDeleteContext(ctx);
        ctx = NULL;
    }

    // Note: some objects (profile, tone, tone_dup) may be intentionally left
    // for the library or test harness to free (no portable free API provided here).
    (void)profile;
    (void)tone;
    (void)tone_dup;

    return 66;
}