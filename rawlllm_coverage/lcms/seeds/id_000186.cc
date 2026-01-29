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
//<ID> 186
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Create a minimal profile
    cmsHPROFILE hProfile = cmsCreateNULLProfile();
    if (!hProfile) return 1;

    // Query tag count
    cmsInt32Number tagCount = cmsGetTagCount(hProfile);

    // Iterate tags (if any) and fetch signatures
    for (cmsInt32Number i = 0; i < tagCount; ++i) {
        cmsUInt32Number sig = (cmsUInt32Number) cmsGetTagSignature(hProfile, (cmsUInt32Number) i);
        (void) sig; // use sig in a no-op to avoid unused warning
    }

    // Duplicate default context (creates a context to pass to thread-aware APIs)
    cmsContext ctx = cmsDupContext(NULL, NULL);
    if (!ctx) return 1;

    // Build a parametric tone curve (type 2 chosen arbitrarily) with some params
    cmsFloat64Number params[4] = { 2.2, 0.0, 0.0, 0.0 };
    cmsToneCurve * curve = cmsBuildParametricToneCurve(ctx, 2, params);
    if (!curve) {
        cmsDeleteContext(ctx);
        return 1;
    }

    // Allocate an IT8 handle and try saving it to memory
    cmsHANDLE it8 = cmsIT8Alloc(ctx);
    if (!it8) {
        cmsDeleteContext(ctx);
        return 1;
    }

    // First call with NULL buffer to obtain required size
    cmsUInt32Number bytesNeeded = 0;
    cmsBool ok = cmsIT8SaveToMem(it8, NULL, &bytesNeeded);
    (void) ok;

    // Allocate a buffer (fall back to a small buffer if bytesNeeded == 0)
    cmsUInt32Number bufSize = bytesNeeded ? bytesNeeded : 128;
    void * buffer = malloc((size_t) bufSize);
    if (buffer) {
        // Attempt to save into the allocated buffer
        cmsBool ok2 = cmsIT8SaveToMem(it8, buffer, &bytesNeeded);
        (void) ok2;
        free(buffer);
    }

    // Clean up context
    cmsDeleteContext(ctx);

    return 66;
}