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
//<ID> 88
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsToneCurve *tone = NULL;
    cmsToneCurve *rev = NULL;
    cmsPipeline *lut1 = NULL;
    cmsPipeline *lut2 = NULL;
    cmsStage *stage = NULL;
    cmsHTRANSFORM xform = NULL;
    cmsHPROFILE devlink = NULL;
    cmsUInt16Number values[4];
    cmsBool isMulti = 0;
    cmsFloat64Number version = 4.2;

    /* Prepare a simple 16-bit tabulated curve */
    values[0] = (cmsUInt16Number)0;
    values[1] = (cmsUInt16Number)16384;
    values[2] = (cmsUInt16Number)49152;
    values[3] = (cmsUInt16Number)65535;

    /* Create an sRGB profile (thread-aware variant accepts a context) */
    hProfile = cmsCreate_sRGBProfileTHR(ctx);

    /* Build a tabulated tone curve from the 16-bit table */
    tone = cmsBuildTabulatedToneCurve16(ctx, 4U, values);

    /* Query whether the tone curve is multi-segment and create a reversed copy */
    isMulti = cmsIsToneCurveMultisegment(tone);
    rev = cmsReverseToneCurve(tone);

    /* Allocate two simple pipelines and concatenate them */
    lut1 = cmsPipelineAlloc(ctx, 1U, 1U);
    lut2 = cmsPipelineAlloc(ctx, 1U, 1U);
    cmsPipelineCat(lut1, lut2);

    /* Allocate an identity stage and query its context (no branching) */
    stage = cmsStageAllocIdentity(ctx, 1U);
    cmsGetStageContextID(stage);

    /* Create a transform between the same profile (as a simple test) and build a device link */
    xform = cmsCreateTransformTHR(ctx, hProfile, 0U, hProfile, 0U, 0U, 0U);
    devlink = cmsTransform2DeviceLink(xform, version, 0U);

    /* Save the generated device link to a file (best-effort; may fail if environment disallows file IO) */
    cmsSaveProfileToFile(devlink, "devlink.icc");

    /* Cleanup of allocated objects where API provides a free/close */
    cmsPipelineFree(lut1);
    cmsPipelineFree(lut2);
    cmsCloseProfile(devlink);
    cmsCloseProfile(hProfile);

    /* Some objects (tone curves, reversed curves, stage, transform) do not have public frees in the
       provided API list; keep references to avoid unused-variable warnings. */
    (void)tone;
    (void)rev;
    (void)stage;
    (void)xform;
    (void)isMulti;
    (void)values;
    (void)version;

    return 66;
}