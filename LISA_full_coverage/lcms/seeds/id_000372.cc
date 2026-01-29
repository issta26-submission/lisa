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
//<ID> 372
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE prof_in = cmsCreate_sRGBProfileTHR(ctx);
    cmsHPROFILE prof_out = cmsCreateXYZProfile();
    cmsHPROFILE prof_proof = cmsCreateNULLProfile();
    cmsToneCurve * curves[3];
    cmsUInt16Number tableVals[4];
    tableVals[0] = (cmsUInt16Number)0;
    tableVals[1] = (cmsUInt16Number)21845;
    tableVals[2] = (cmsUInt16Number)43690;
    tableVals[3] = (cmsUInt16Number)65535;
    cmsHTRANSFORM transform = NULL;
    cmsUInt32Number renderingIntent = (cmsUInt32Number)0;
    cmsBool mlu_set_result = 0;
    cmsUInt32Number validation = (cmsUInt32Number)0;

    // step 2: Setup
    curves[0] = cmsBuildTabulatedToneCurve16(ctx, (cmsUInt32Number)4, tableVals);
    curves[1] = cmsBuildTabulatedToneCurve16(ctx, (cmsUInt32Number)4, tableVals);
    curves[2] = cmsBuildTabulatedToneCurve16(ctx, (cmsUInt32Number)4, tableVals);
    renderingIntent = cmsGetHeaderRenderingIntent(prof_in);
    mlu_set_result = cmsMLUsetUTF8(NULL, "eng", "USA", "Test UTF8 string");

    // step 3: Operate & Validate
    transform = cmsCreateProofingTransformTHR(ctx, prof_in, (cmsUInt32Number)0, prof_out, (cmsUInt32Number)0, prof_proof, renderingIntent, renderingIntent, (cmsUInt32Number)0);
    validation = (cmsUInt32Number)renderingIntent ^ (cmsUInt32Number)(mlu_set_result ? 1U : 0U);
    (void)validation;

    // step 4: Cleanup
    cmsDeleteTransform(transform);
    cmsFreeToneCurveTriple(curves);
    cmsCloseProfile(prof_in);
    cmsCloseProfile(prof_out);
    cmsCloseProfile(prof_proof);

    // API sequence test completed successfully
    return 66;
}