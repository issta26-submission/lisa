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
//<ID> 374
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE profile = cmsCreate_sRGBProfileTHR(ctx);
    cmsHPROFILE proof = cmsCreateNULLProfile();
    cmsToneCurve * Curve[3];
    cmsUInt16Number table[2];
    table[0] = (cmsUInt16Number)0;
    table[1] = (cmsUInt16Number)65535;
    cmsMLU * mlu = (cmsMLU *)NULL;
    cmsHTRANSFORM transform = (cmsHTRANSFORM)NULL;
    cmsUInt32Number renderIntent = (cmsUInt32Number)0;
    cmsUInt32Number validation = (cmsUInt32Number)0;

    // step 2: Setup
    Curve[0] = cmsBuildTabulatedToneCurve16(ctx, (cmsUInt32Number)2, table);
    Curve[1] = cmsBuildTabulatedToneCurve16(ctx, (cmsUInt32Number)2, table);
    Curve[2] = cmsBuildTabulatedToneCurve16(ctx, (cmsUInt32Number)2, table);
    cmsSetHeaderRenderingIntent(profile, (cmsUInt32Number)1);
    cmsMLUsetUTF8(mlu, "eng", "USA", "Example Profile");

    // step 3: Operate & Validate
    transform = cmsCreateProofingTransformTHR(ctx, profile, (cmsUInt32Number)0, profile, (cmsUInt32Number)0, proof, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);
    renderIntent = cmsGetHeaderRenderingIntent(profile);
    validation = (cmsUInt32Number)(renderIntent ^ (cmsUInt32Number)(Curve[0] != NULL) ^ (cmsUInt32Number)(transform != NULL));
    (void)validation;

    // step 4: Cleanup
    cmsDeleteTransform(transform);
    cmsFreeToneCurveTriple(Curve);
    cmsCloseProfile(proof);
    cmsCloseProfile(profile);

    // API sequence test completed successfully
    return 66;
}