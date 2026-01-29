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
//<ID> 147
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Declarations
    cmsContext ctx = NULL;
    cmsCIExyY whitePoint;
    cmsHPROFILE hProfile = NULL;
    cmsToneCurve * gamma = NULL;
    cmsToneCurve * revGamma = NULL;
    cmsPipeline * lut = NULL;
    cmsUInt32Number headerFlags = 0;
    cmsFloat32Number eval = 0.0f;
    cmsFloat32Number target[4];
    cmsFloat32Number result[4];
    cmsFloat32Number hint[4];
    cmsUInt32Number stageCount = 0;
    cmsUInt32Number outChannels = 0;

    // Initialize white point
    memset(&whitePoint, 0, sizeof(whitePoint));
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;

    // Create a Lab profile using the white point
    hProfile = cmsCreateLab2ProfileTHR(ctx, &whitePoint);

    // Read and modify header flags
    headerFlags = cmsGetHeaderFlags(hProfile);
    cmsSetHeaderFlags(hProfile, headerFlags | 0x01u);

    // Build and evaluate a gamma tone curve, then create its reverse
    gamma = cmsBuildGamma(ctx, 2.2);
    eval = cmsEvalToneCurveFloat(gamma, 0.5f);
    revGamma = cmsReverseToneCurveEx(256, gamma);
    cmsIsToneCurveMonotonic(gamma);
    cmsIsToneCurveMultisegment(gamma);

    // Allocate a simple pipeline and manipulate/query it
    lut = cmsPipelineAlloc(ctx, 3, 3);
    cmsPipelineSetSaveAs8bitsFlag(lut, 1);
    stageCount = cmsPipelineStageCount(lut);
    outChannels = cmsPipelineOutputChannels(lut);

    // Prepare arrays and evaluate reverse pipeline (must provide non-NULL lut)
    target[0] = 0.1f; target[1] = 0.2f; target[2] = 0.3f; target[3] = 0.0f;
    result[0] = result[1] = result[2] = result[3] = 0.0f;
    hint[0] = hint[1] = hint[2] = hint[3] = 0.0f;
    cmsPipelineEvalReverseFloat(target, result, hint, lut);

    // Silence unused-variable warnings and leave resources to the caller/test harness
    (void)ctx;
    (void)whitePoint;
    (void)hProfile;
    (void)gamma;
    (void)revGamma;
    (void)headerFlags;
    (void)eval;
    (void)stageCount;
    (void)outChannels;
    (void)target;
    (void)result;
    (void)hint;
    (void)lut;

    return 66;
}