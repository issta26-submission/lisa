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
//<ID> 311
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Create a simple sRGB profile
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (!hProfile) {
        // failed to create profile; still treat as fail (not returning success code)
        return 1;
    }

    // Query header model from profile
    cmsUInt32Number headerModel = cmsGetHeaderModel(hProfile);
    (void)headerModel; // silence unused-variable warning

    // Get the first tag signature (index 0) from the profile
    cmsTagSignature tag0 = cmsGetTagSignature(hProfile, 0);

    // Read the tag data (may return NULL if tag not present)
    void * tagData = cmsReadTag(hProfile, tag0);
    if (tagData) {
        // Attempt to treat the returned tag as an MLU and duplicate it safely
        cmsMLU * mlu_orig = static_cast<cmsMLU *>(tagData);
        cmsMLU * mlu_dup = cmsMLUdup(mlu_orig);
        if (mlu_dup) {
            // Free the duplicated MLU using the required API
            cmsMLUfree(mlu_dup);
        }
    }

    // Create a transform using the same profile as input and output with default format/intent
    cmsHTRANSFORM hTransform = cmsCreateTransform(hProfile, 0, hProfile, 0, 0, 0);
    if (hTransform) {
        // Query the output format of the created transform
        cmsUInt32Number outFormat = cmsGetTransformOutputFormat(hTransform);
        (void)outFormat; // silence unused-variable warning
        // Note: no explicit destroy function for cmsHTRANSFORM is guaranteed in the provided API list,
        // so we do not attempt to free it here.
    }

    // All required API calls performed; return success code
    return 66;
}