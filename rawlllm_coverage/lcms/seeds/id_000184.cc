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
//<ID> 184
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Create a context for threaded APIs
    cmsContext ContextID = cmsCreateContext(nullptr, nullptr);
    if (!ContextID) {
        return 1;
    }

    // Create a NULL profile to query header information
    cmsHPROFILE hProfile = cmsCreateNULLProfileTHR(ContextID);

    // Query profile creation date/time
    struct tm creation = {};
    cmsBool gotTime = cmsGetHeaderCreationDateTime(hProfile, &creation);
    (void) gotTime; // suppress unused variable warning

    // Build a simple gamma tone curve and get its first segment
    cmsToneCurve * curve = cmsBuildGamma(ContextID, 2.2);
    const cmsCurveSegment * seg = nullptr;
    if (curve) {
        seg = cmsGetToneCurveSegment(0, curve);
    }
    (void) seg; // suppress unused variable warning

    // Convert a sample LCh color to Lab
    cmsCIELCh sampleLCh;
    sampleLCh.L = 50.0;
    sampleLCh.C = 20.0;
    sampleLCh.h = 180.0;
    cmsCIELab outLab;
    cmsLCh2Lab(&outLab, &sampleLCh);

    // Allocate an IT8 structure, attempt to get a patch name, then free it
    cmsHANDLE hIT8 = cmsIT8Alloc(ContextID);
    if (hIT8) {
        char buffer[64] = {0};
        const char * name = cmsIT8GetPatchName(hIT8, 0, buffer);
        (void) name; // suppress unused variable warning
        cmsIT8Free(hIT8);
    }

    // Clean up context
    cmsDeleteContext(ContextID);

    return 66;
}