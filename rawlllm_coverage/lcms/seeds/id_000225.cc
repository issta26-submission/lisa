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
//<ID> 225
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Create a context for operations that require one
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);

    // Build a simple gamma tone curve and then free it to exercise cmsFreeToneCurve
    cmsToneCurve * curve = nullptr;
    if (ctx) {
        curve = cmsBuildGamma(ctx, 2.2);
        if (curve) {
            cmsFreeToneCurve(curve);
            curve = nullptr;
        }
    }

    // Try to open a profile from file. If that fails, create a NULL profile.
    cmsHPROFILE profile = cmsOpenProfileFromFile("example.icc", "r");
    if (!profile) {
        profile = cmsCreateNULLProfile();
    }

    // If profile was obtained, set some header attributes and query a tag signature
    if (profile) {
        cmsUInt64Number flags = (cmsUInt64Number)0x1ULL; // example flag
        cmsSetHeaderAttributes(profile, flags);

        // Query the first tag signature (index 0). The returned value is not used further here.
        cmsTagSignature tag = cmsGetTagSignature(profile, 0u);
        (void)tag;
    }

    return 66;
}