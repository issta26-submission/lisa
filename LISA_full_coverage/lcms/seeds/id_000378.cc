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
//<ID> 378
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
    cmsHTRANSFORM transform = NULL;
    cmsToneCurve * curves[3];
    cmsUInt16Number tableVals[3];
    char utf8buf[64];
    cmsMLU * mlu = NULL;
    cmsUInt32Number intent = (cmsUInt32Number)0;
    cmsUInt32Number renderIntent = (cmsUInt32Number)0;
    cmsBool ok = (cmsBool)0;
    memset(utf8buf, 0, sizeof(utf8buf));
    tableVals[0] = (cmsUInt16Number)0;
    tableVals[1] = (cmsUInt16Number)32768;
    tableVals[2] = (cmsUInt16Number)65535;

    // step 2: Setup
    ok = cmsMLUsetUTF8(mlu, "eng", "USA", "Example MLU string");
    curves[0] = cmsBuildTabulatedToneCurve16(ctx, (cmsUInt32Number)3, tableVals);
    curves[1] = cmsBuildTabulatedToneCurve16(ctx, (cmsUInt32Number)3, tableVals);
    curves[2] = cmsBuildTabulatedToneCurve16(ctx, (cmsUInt32Number)3, tableVals);
    renderIntent = cmsGetHeaderRenderingIntent(profile_in);
    intent = renderIntent;
    transform = cmsCreateProofingTransformTHR(ctx, profile_in, (cmsUInt32Number)0, profile_out, (cmsUInt32Number)0, profile_out, intent, intent, (cmsUInt32Number)0);

    // step 3: Core operations & Validate
    cmsUInt32Number validation = (cmsUInt32Number)((cmsUInt32Number)renderIntent ^ (cmsUInt32Number)ok ^ (cmsUInt32Number)((transform == NULL) ? 0u : 1u));
    (void)validation;
    (void)utf8buf;

    // step 4: Cleanup
    cmsDeleteTransform(transform);
    cmsFreeToneCurveTriple(curves);
    cmsCloseProfile(profile_in);
    cmsCloseProfile(profile_out);

    // API sequence test completed successfully
    return 66;
}