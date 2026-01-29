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
//<ID> 375
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE profile_in = NULL;
    cmsHPROFILE profile_out = NULL;
    cmsHPROFILE profile_proof = NULL;
    cmsHTRANSFORM transform = NULL;
    cmsToneCurve * curves[3];
    cmsUInt16Number tableVals[2];
    cmsUInt32Number renderingIntent = (cmsUInt32Number)0;

    // step 2: Setup
    tableVals[0] = (cmsUInt16Number)0;
    tableVals[1] = (cmsUInt16Number)65535;
    curves[0] = cmsBuildTabulatedToneCurve16(ctx, 2, tableVals);
    curves[1] = cmsBuildTabulatedToneCurve16(ctx, 2, tableVals);
    curves[2] = cmsBuildTabulatedToneCurve16(ctx, 2, tableVals);
    profile_in = cmsCreate_sRGBProfileTHR(ctx);
    profile_out = cmsCreateXYZProfile();
    profile_proof = cmsCreateNULLProfile();
    renderingIntent = cmsGetHeaderRenderingIntent(profile_in);

    // step 3: Core operations & Validate
    transform = cmsCreateProofingTransformTHR(ctx, profile_in, (cmsUInt32Number)0, profile_out, (cmsUInt32Number)0, profile_proof, renderingIntent, (cmsUInt32Number)0, (cmsUInt32Number)0);
    cmsMLUsetUTF8(NULL, "eng", "USA", "Test MLU UTF8 string");
    (void)renderingIntent;
    (void)transform;

    // step 4: Cleanup
    cmsDeleteTransform(transform);
    cmsFreeToneCurveTriple(curves);
    cmsCloseProfile(profile_in);
    cmsCloseProfile(profile_out);
    cmsCloseProfile(profile_proof);

    // API sequence test completed successfully
    return 66;
}