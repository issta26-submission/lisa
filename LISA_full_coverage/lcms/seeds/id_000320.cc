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
//<ID> 320
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization (use default context NULL)
    cmsContext ctx = NULL;
    const cmsCIExyY * d50 = cmsD50_xyY();
    cmsHPROFILE srcProfile = cmsCreateLab2ProfileTHR(ctx, d50);
    cmsHPROFILE dstProfile = cmsCreateLab2ProfileTHR(ctx, d50);

    // step 2: Setup (create a multiprofile transform and derive a working context)
    cmsHPROFILE profiles[2];
    profiles[0] = srcProfile;
    profiles[1] = dstProfile;
    cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(ctx, profiles, 2, 0, 0, 0, 0);
    cmsContext transformCtx = cmsDupContext(ctx, NULL);
    cmsHPROFILE nullProfile = cmsCreateProfilePlaceholder(transformCtx);

    // step 3: Core operations (allocate a matrix stage, inspect it, allocate IT8 and query a property)
    cmsFloat64Number matrix[16] = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    cmsFloat64Number offset[4] = { 0.0, 0.0, 0.0, 0.0 };
    cmsStage * stage = cmsStageAllocMatrix(transformCtx, 4, 4, matrix, offset);
    cmsUInt32Number inCh = cmsStageInputChannels(stage);

    cmsHANDLE it8 = cmsIT8Alloc(transformCtx);
    char patchBuffer[128] = { 0 };
    const char * prop = cmsIT8GetProperty(it8, "ORIGIN");

    // step 4: Cleanup (close profiles and delete duplicated context)
    cmsCloseProfile(nullProfile);
    cmsCloseProfile(dstProfile);
    cmsCloseProfile(srcProfile);
    cmsDeleteContext(transformCtx);

    // silence unused warnings for objects we intentionally did not further process
    (void) transform;
    (void) stage;
    (void) inCh;
    (void) it8;
    (void) prop;
    (void) patchBuffer;
    return 66;
}