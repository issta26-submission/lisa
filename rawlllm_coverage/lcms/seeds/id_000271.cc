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
//<ID> 271
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Use the default context (NULL) for thread-safe THR variants where context is optional.
    cmsContext ctx = NULL;

    // Create two profiles: an sRGB input and a NULL output profile.
    cmsHPROFILE inProfile  = cmsCreate_sRGBProfileTHR(ctx);
    cmsHPROFILE outProfile = cmsCreateNULLProfileTHR(ctx);

    if (inProfile == NULL || outProfile == NULL) {
        if (inProfile)  cmsCloseProfile(inProfile);
        if (outProfile) cmsCloseProfile(outProfile);
        return 1; // early failure
    }

    // Allocate an MLU and store a UTF-8 string into it, then read it back.
    cmsMLU * mlu = cmsMLUalloc(ctx, 1);
    if (mlu) {
        char lang[3]    = { 'e', 'n', 0 }; // language code (3-byte array)
        char country[3] = { 'U', 'S', 0 }; // country code (3-byte array)

        // Store a UTF-8 string
        cmsBool set_ok = cmsMLUsetUTF8(mlu, lang, country, "Test string from MLU");
        (void)set_ok; // silence unused variable if checks are not desired

        // Read it back into a buffer
        char buffer[128] = {0};
        cmsUInt32Number read = cmsMLUgetUTF8(mlu, lang, country, buffer, sizeof(buffer));
        (void)read;
        (void)buffer; // we don't need to use the contents further here

        // Free the MLU
        cmsMLUfree(mlu);
    }

    // Create a transform between the two profiles (using zeroed format/intent/flags for test)
    cmsUInt32Number inputFormat  = (cmsUInt32Number)0;
    cmsUInt32Number outputFormat = (cmsUInt32Number)0;
    cmsUInt32Number intent       = (cmsUInt32Number)0;
    cmsUInt32Number flags        = (cmsUInt32Number)0;

    cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx,
                                                    inProfile, inputFormat,
                                                    outProfile, outputFormat,
                                                    intent, flags);
    (void)transform; // no further use in this test sequence

    // Query a tag presence on the input profile (use tag signature 0 for a benign check)
    cmsBool hasTag = cmsIsTag(inProfile, (cmsTagSignature)0);
    (void)hasTag;

    // Clean up profiles
    cmsCloseProfile(inProfile);
    cmsCloseProfile(outProfile);

    // Return success code as requested
    return 66;
}