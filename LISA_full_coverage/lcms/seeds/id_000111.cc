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
//<ID> 111
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsToneCurve *tone = NULL;
    cmsToneCurve *tone_dup = NULL;
    cmsToneCurve *tone_rev = NULL;
    cmsToneCurve *tone_join = NULL;
    cmsHTRANSFORM xform = NULL;
    cmsContext tctx = NULL;
    cmsUInt32Number infmt = 0U;
    cmsUInt32Number outfmt = 0U;
    cmsUInt32Number encVersion = 4U;
    cmsFloat64Number gamma_guess = 1.8;
    cmsUInt32Number flags = 0U;
    cmsBool isMulti = 0;
    cmsUInt16Number eval16 = 0;

    // step 2: Create profile and set encoded version
    hProfile = cmsCreate_sRGBProfile();
    cmsSetEncodedICCversion(hProfile, encVersion);

    // step 3: Build and manipulate tone curves
    {
        const cmsUInt16Number table_vals[3] = { 0, 32768, 65535 };
        tone = cmsBuildTabulatedToneCurve16(ctx, 3U, table_vals);
    }
    tone_dup = cmsDupToneCurve(tone);
    tone_rev = cmsReverseToneCurveEx(5U, tone);
    tone_join = cmsJoinToneCurve(ctx, tone, tone_dup, 256U);
    isMulti = cmsIsToneCurveMultisegment(tone_join);
    eval16 = cmsEvalToneCurve16(tone_join, (cmsUInt16Number)32768);

    // step 4: Create a proofing transform (acts as a transform/device-link like object)
    xform = cmsCreateProofingTransform(hProfile, 0U, hProfile, 0U, (cmsHPROFILE) NULL, 0U, 0U, flags);
    tctx = cmsGetTransformContextID(xform);
    infmt = cmsGetTransformInputFormat(xform);
    outfmt = cmsGetTransformOutputFormat(xform);

    // step 5: Query profile capabilities and properties
    cmsBool intent_ok = cmsIsIntentSupported(hProfile, 0U, 0U);
    cmsFloat64Number detected_gamma = cmsDetectRGBProfileGamma(hProfile, gamma_guess);

    // step 6: Cleanup and close profile/transform (close profile handle; other frees not available in provided API subset)
    cmsCloseProfile(hProfile);
    cmsUnregisterPlugins();

    // keep variables referenced to avoid unused warnings
    (void)ctx;
    (void)tone_rev;
    (void)tone_dup;
    (void)tone_join;
    (void)xform;
    (void)tctx;
    (void)infmt;
    (void)outfmt;
    (void)isMulti;
    (void)eval16;
    (void)intent_ok;
    (void)detected_gamma;
    (void)encVersion;
    (void)gamma_guess;
    (void)flags;

    return 66;
}