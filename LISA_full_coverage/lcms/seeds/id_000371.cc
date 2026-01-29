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
//<ID> 371
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsToneCurve * Curves[3];
    cmsUInt16Number tableVals[2];
    tableVals[0] = 0;
    tableVals[1] = 65535;
    cmsHPROFILE profile_in = NULL;
    cmsHPROFILE profile_out = NULL;
    cmsHPROFILE proof = NULL;
    cmsHTRANSFORM transform = NULL;
    cmsUInt32Number intent = 0;
    char utf8str[64];
    memset(utf8str, 0, sizeof(utf8str));
    cmsMLU * mlu = NULL;
    cmsBool set_ok = 0;
    cmsUInt32Number validation = 0;

    // step 2: Setup
    Curves[0] = cmsBuildTabulatedToneCurve16(ctx, (cmsUInt32Number)2, tableVals);
    Curves[1] = cmsBuildTabulatedToneCurve16(ctx, (cmsUInt32Number)2, tableVals);
    Curves[2] = cmsBuildTabulatedToneCurve16(ctx, (cmsUInt32Number)2, tableVals);
    profile_in = cmsCreate_sRGBProfileTHR(ctx);
    profile_out = cmsCreateXYZProfile();
    proof = cmsCreateNULLProfile();
    transform = cmsCreateProofingTransformTHR(ctx, profile_in, (cmsUInt32Number)0, profile_out, (cmsUInt32Number)0, proof, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);

    // step 3: Operate & Validate
    intent = cmsGetHeaderRenderingIntent(profile_in);
    (void)snprintf(utf8str, sizeof(utf8str), "Intent:%u", (unsigned)intent);
    set_ok = cmsMLUsetUTF8(mlu, "eng", "USA", utf8str);
    validation = (cmsUInt32Number)((cmsUInt32Number)intent ^ (cmsUInt32Number)set_ok);
    (void)validation;

    // step 4: Cleanup
    cmsFreeToneCurveTriple(Curves);
    cmsDeleteTransform(transform);
    cmsCloseProfile(profile_in);
    cmsCloseProfile(profile_out);
    cmsCloseProfile(proof);

    // API sequence test completed successfully
    return 66;
}