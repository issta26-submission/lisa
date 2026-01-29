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
//<ID> 377
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsToneCurve * curves[3];
    cmsUInt16Number vals1[2] = { 0u, 65535u };
    cmsUInt16Number vals2[2] = { 0u, 65535u };
    cmsUInt16Number vals3[2] = { 0u, 65535u };
    curves[0] = cmsBuildTabulatedToneCurve16(ctx, (cmsUInt32Number)2, vals1);
    curves[1] = cmsBuildTabulatedToneCurve16(ctx, (cmsUInt32Number)2, vals2);
    curves[2] = cmsBuildTabulatedToneCurve16(ctx, (cmsUInt32Number)2, vals3);
    cmsHPROFILE profile = cmsCreate_sRGBProfileTHR(ctx);
    cmsUInt32Number intent = cmsGetHeaderRenderingIntent(profile);
    cmsHTRANSFORM transform = NULL;
    cmsMLU * mlu = NULL;
    const char * label = "Example UTF8 Label";

    // step 2: Setup
    transform = cmsCreateProofingTransformTHR(ctx, profile, (cmsUInt32Number)0, profile, (cmsUInt32Number)0, (cmsHPROFILE)NULL, intent, intent, (cmsUInt32Number)0);
    cmsBool set_ok = cmsMLUsetUTF8(mlu, "eng", "USA", label);

    // step 3: Core operations & Validate
    cmsUInt32Number validation = (cmsUInt32Number)((cmsUInt32Number)intent ^ (cmsUInt32Number)set_ok);
    (void)validation;

    // step 4: Cleanup
    cmsDeleteTransform(transform);
    cmsFreeToneCurveTriple(curves);
    cmsCloseProfile(profile);

    // API sequence test completed successfully
    return 66;
}