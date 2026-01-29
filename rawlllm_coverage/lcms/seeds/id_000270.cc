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
//<ID> 270
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Create a standard sRGB profile
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (!hProfile) return 1;

    // Obtain the profile's context ID
    cmsContext ctx = cmsGetProfileContextID(hProfile);

    // Build a gamma tone curve using the profile context
    cmsToneCurve * gamma = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);
    if (!gamma) {
        // Attempt to set header flags (no-op if not supported) and close profile before exit
        cmsSetHeaderFlags(hProfile, (cmsUInt32Number)0);
        cmsCloseProfile(hProfile);
        return 2;
    }

    // Allocate an IT8 handle and set an uncooked property string on it
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (hIT8) {
        cmsBool ok = cmsIT8SetPropertyUncooked(hIT8, "ExampleKey", "ExampleBufferContents");
        (void)ok; // ignore result for this test sequence
        // Free the IT8 handle
        cmsIT8Free(hIT8);
    }

    // Set some header flags on the profile
    cmsSetHeaderFlags(hProfile, (cmsUInt32Number)0x12345678u);

    // Re-query context (just to exercise the API) and then close the profile
    (void) cmsGetProfileContextID(hProfile);
    cmsBool closed = cmsCloseProfile(hProfile);
    if (!closed) {
        return 3;
    }

    // Successful completion
    return 66;
}