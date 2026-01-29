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
//<ID> 266
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Create an sRGB profile (thread-safe variant that accepts a cmsContext)
    cmsHPROFILE hProfile = cmsCreate_sRGBProfileTHR(NULL);
    if (hProfile == NULL) {
        return 1; // failed to create profile
    }

    // Query the profile color space signature
    cmsColorSpaceSignature cs = cmsGetColorSpace(hProfile);

    // Get number of channels for that color space using the required API
    cmsInt32Number channels = cmsChannelsOfColorSpace(cs);

    // Create a transform from the profile to itself (simple no-op transform).
    // Use zero for formats/intents/flags to keep the example minimal and portable.
    cmsHTRANSFORM hTransform = cmsCreateTransformTHR(NULL, hProfile, (cmsUInt32Number)0,
                                                     hProfile, (cmsUInt32Number)0,
                                                     (cmsUInt32Number)0, (cmsUInt32Number)0);
    if (hTransform == NULL) {
        return 2; // failed to create transform
    }

    // Try to read the first tag from the profile and, if it exists and is an MLU,
    // duplicate it using cmsMLUdup. This demonstrates usage of cmsGetTagSignature,
    // cmsReadTag and the required cmsMLUdup call.
    cmsTagSignature sig = cmsGetTagSignature(hProfile, (cmsUInt32Number)0);
    if (sig != (cmsTagSignature)0) {
        void * tag = cmsReadTag(hProfile, sig);
        if (tag != NULL) {
            // Attempt to duplicate as an MLU. If tag is not actually an MLU,
            // cmsMLUdup is expected to return NULL; treat that as non-fatal here.
            cmsMLU * dup = cmsMLUdup((const cmsMLU *) tag);
            // We don't have a public free for cmsMLU in the provided API list,
            // so we won't attempt to free 'dup'. Just check the call executed.
            (void)dup;
        }
    }

    // Clean up the transform
    cmsDeleteTransform(hTransform);

    // Success
    (void)channels; // use variable to avoid unused-warning in some compilers
    return 66;
}