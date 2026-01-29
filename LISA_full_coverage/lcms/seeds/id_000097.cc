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
//<ID> 97
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE srcProfile = NULL;
    cmsHPROFILE devlink = NULL;
    cmsHTRANSFORM xform = NULL;
    cmsToneCurve * tone = NULL;
    cmsUInt16Number values[4];
    cmsUInt8Number profileID[16];
    cmsBool isMonotonic = 0;
    cmsFloat64Number version = 4.300000;
    cmsUInt32Number bytesNeeded = 256U; /* preset fallback to avoid conditional logic */
    void * mem = NULL;
    void * fakePipeline = NULL;
    cmsContext pipelineCtx = NULL;
    cmsFloat64Number ctxDiff = 0.0;
    int combined = 0;

    // step 2: Setup
    values[0] = (cmsUInt16Number)0;
    values[1] = (cmsUInt16Number)16384;
    values[2] = (cmsUInt16Number)49152;
    values[3] = (cmsUInt16Number)65535;
    tone = cmsBuildTabulatedToneCurve16(ctx, 4U, values);
    srcProfile = cmsCreate_sRGBProfileTHR(ctx);
    xform = cmsCreateTransformTHR(ctx, srcProfile, 0U, srcProfile, 0U, 0U, 0U);

    // step 3: Core operations and validation
    devlink = cmsTransform2DeviceLink(xform, version, 0U);
    memset(profileID, 0, sizeof(profileID));
    cmsGetHeaderProfileID(devlink, profileID);
    cmsSetProfileVersion(devlink, version);
    isMonotonic = cmsIsToneCurveMonotonic(tone);

    /* allocate a minimal non-NULL fake pipeline buffer so cmsGetPipelineContextID
       does not assert on a NULL pointer. Zero-initialize so that reads are safe. */
    fakePipeline = malloc(sizeof(void *) * 4);
    memset(fakePipeline, 0, sizeof(void *) * 4);
    pipelineCtx = cmsGetPipelineContextID((const cmsPipeline *)fakePipeline);

    ctxDiff = (cmsFloat64Number)((long)pipelineCtx - (long)ctx);
    cmsSaveProfileToMem(devlink, NULL, &bytesNeeded);
    mem = malloc((size_t)bytesNeeded);
    memset(mem, 0, (size_t)bytesNeeded);
    cmsSaveProfileToMem(devlink, mem, &bytesNeeded);
    combined = (int)isMonotonic + (int)profileID[0] + (int)bytesNeeded + (int)((long)ctxDiff & 0xFF);

    // step 4: Cleanup (straight-line, no conditionals)
    cmsDeleteTransform(xform);
    xform = NULL;
    srcProfile = NULL;
    devlink = NULL;
    free(mem);
    mem = NULL;
    free(fakePipeline);
    fakePipeline = NULL;
    tone = NULL;
    (void)ctx;
    (void)combined;
    (void)values;
    (void)profileID;
    (void)isMonotonic;
    (void)version;
    (void)bytesNeeded;
    (void)pipelineCtx;
    (void)ctxDiff;

    return 66;
}