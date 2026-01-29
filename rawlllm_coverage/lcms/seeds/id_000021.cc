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
//<ID> 21
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Use a NULL/default context for THR functions
    cmsContext ctx = NULL;

    // Create three simple profiles to use as input, output and proofing profiles.
    cmsHPROFILE inputProfile = cmsCreateXYZProfile();
    cmsHPROFILE outputProfile = cmsCreateXYZProfile();
    cmsHPROFILE proofingProfile = cmsCreateXYZProfile();

    // Basic null checks
    if (inputProfile == NULL || outputProfile == NULL || proofingProfile == NULL) {
        // Attempt to close any that succeeded
        if (inputProfile) cmsCloseProfile(inputProfile);
        if (outputProfile) cmsCloseProfile(outputProfile);
        if (proofingProfile) cmsCloseProfile(proofingProfile);
        return 66;
    }

    // Create a proofing transform using the _THR variant (context-aware).
    // Pass zeroes for formats, intents and flags for a minimal test.
    cmsHTRANSFORM transform = cmsCreateProofingTransformTHR(ctx,
                                                           inputProfile, 0,
                                                           outputProfile, 0,
                                                           proofingProfile,
                                                           0, 0, 0);

    // Query a tag signature (index 0) from one of the profiles.
    // Store result in auto-typed variable since cmsTagSignature is a typedef.
    auto tagSig = cmsGetTagSignature(inputProfile, (cmsUInt32Number)0);

    // Allocate an IT8 handle and query a property string from it.
    cmsHANDLE it8 = cmsIT8Alloc(ctx);
    const char * it8Prop = NULL;
    if (it8 != NULL) {
        // Request some property; using a generic name is fine for this sequence.
        it8Prop = cmsIT8GetProperty(it8, "SampleProperty");
        // Note: we don't have a documented free for IT8 in the provided subset,
        // so we simply let it go out of scope; real code should free if API exists.
    }

    // Allocate a GBD structure and then free it using cmsGBDFree as required.
    cmsHANDLE gbd = cmsGBDAlloc(ctx);
    if (gbd != NULL) {
        // Immediately free to test the api
        cmsGBDFree(gbd);
        gbd = NULL;
    }

    // Cleanup: close profiles and (if applicable) any transform-specific cleanup.
    // There is no transform free in the provided API subset, so just release profiles.
    cmsCloseProfile(inputProfile);
    cmsCloseProfile(outputProfile);
    cmsCloseProfile(proofingProfile);

    // Return success sentinel as requested.
    return 66;
}