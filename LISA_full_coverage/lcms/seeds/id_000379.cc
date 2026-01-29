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
//<ID> 379
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE profile_in = cmsCreate_sRGBProfileTHR(ctx);
    cmsHPROFILE profile_out = cmsCreateXYZProfile();
    cmsHPROFILE profile_proof = cmsCreateNULLProfile();
    cmsToneCurve * tc[3];
    cmsUInt16Number table[2];
    memset(table, 0, sizeof(table));
    table[0] = (cmsUInt16Number)0;
    table[1] = (cmsUInt16Number)65535;
    tc[0] = cmsBuildTabulatedToneCurve16(ctx, (cmsUInt32Number)2, table);
    tc[1] = cmsBuildTabulatedToneCurve16(ctx, (cmsUInt32Number)2, table);
    tc[2] = cmsBuildTabulatedToneCurve16(ctx, (cmsUInt32Number)2, table);

    // step 2: Setup
    cmsUInt32Number intent = cmsGetHeaderRenderingIntent(profile_in);
    cmsHTRANSFORM transform = cmsCreateProofingTransformTHR(ctx, profile_in, (cmsUInt32Number)0, profile_out, (cmsUInt32Number)0, profile_proof, intent, intent, (cmsUInt32Number)0);
    cmsMLU * mlu = NULL;
    char utf8_scratch[32];
    memset(utf8_scratch, 0, sizeof(utf8_scratch));
    cmsBool mlu_result = cmsMLUsetUTF8(mlu, "eng", "USA", "Sample UTF8");

    // step 3: Operate & Validate
    cmsUInt32Number outFormat = cmsGetTransformOutputFormat(transform);
    cmsUInt32Number validation = (cmsUInt32Number)((intent ^ outFormat) ^ (mlu_result ? 1u : 0u));
    (void)validation;

    // step 4: Cleanup
    cmsDeleteTransform(transform);
    cmsFreeToneCurveTriple(tc);
    cmsCloseProfile(profile_in);
    cmsCloseProfile(profile_out);
    cmsCloseProfile(profile_proof);

    // API sequence test completed successfully
    return 66;
}