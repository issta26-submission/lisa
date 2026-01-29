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
//<ID> 376
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = (cmsContext)NULL;
    cmsHPROFILE profile = cmsCreate_sRGBProfileTHR(ctx);
    cmsToneCurve *curves[3];
    cmsUInt16Number table[2];
    table[0] = (cmsUInt16Number)0;
    table[1] = (cmsUInt16Number)65535;
    cmsMLU *mlu = (cmsMLU *)NULL;
    const char * utf8 = "Example MLU UTF8 string";
    cmsHTRANSFORM transform = (cmsHTRANSFORM)NULL;
    cmsUInt32Number intent = (cmsUInt32Number)0;
    cmsFloat64Number gamma = (cmsFloat64Number)0.0;
    cmsUInt32Number validation = (cmsUInt32Number)0;

    // step 2: Setup
    curves[0] = cmsBuildTabulatedToneCurve16(ctx, (cmsUInt32Number)2, table);
    curves[1] = cmsDupToneCurve(curves[0]);
    curves[2] = cmsDupToneCurve(curves[0]);
    cmsMLUsetUTF8(mlu, "eng", "USA", utf8);
    transform = cmsCreateProofingTransformTHR(ctx, profile, (cmsUInt32Number)0, profile, (cmsUInt32Number)0, (cmsHPROFILE)NULL, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);

    // step 3: Operate & Validate
    intent = cmsGetHeaderRenderingIntent(profile);
    gamma = cmsEstimateGamma(curves[0], (cmsFloat64Number)0.01);
    validation = (cmsUInt32Number)((cmsUInt32Number)intent ^ (cmsUInt32Number)((int)gamma));
    (void)validation;

    // step 4: Cleanup
    cmsFreeToneCurveTriple(curves);
    cmsDeleteTransform(transform);
    cmsCloseProfile(profile);

    // API sequence test completed successfully
    return 66;
}