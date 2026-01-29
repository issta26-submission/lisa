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
//<ID> 59
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Step 1: Create contexts and basic handles
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsContext dupCtx = cmsDupContext(ctx, NULL);
    cmsHPROFILE hProfile = cmsCreateXYZProfile();
    cmsHANDLE hGBD = cmsGBDAlloc(ctx);
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);

    // Step 2: Create a minimal 2^3 * 3 = 24 entry float CLUT (2 grid points, 3 in, 3 out)
    cmsFloat32Number clutTable[24] = {
        0.0f, 0.0f, 0.0f,
        0.333333f, 0.333333f, 0.333333f,
        0.666667f, 0.666667f, 0.666667f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f,
        0.2f, 0.8f, 0.6f
    };
    cmsStage * stage = cmsStageAllocCLutFloat(ctx, 2U, 3U, 3U, clutTable);

    // Step 3: Inspect stage and IT8 formats (straight-line calls, no branching)
    cmsStageSignature stageSig = cmsStageType(stage);
    cmsUInt32Number stageInCh = cmsStageInputChannels(stage);
    char ** dataFormats = NULL;
    int dfCount = cmsIT8EnumDataFormat(hIT8, &dataFormats);

    // Step 4: Query profile metadata and compute ID
    cmsFloat64Number profileVer = cmsGetProfileVersion(hProfile);
    cmsUInt32Number renderingIntent = cmsGetHeaderRenderingIntent(hProfile);
    cmsBool md5ok = cmsMD5computeID(hProfile);
    cmsContext profCtx = cmsGetProfileContextID(hProfile);

    // Step 5: Clean up stage (available free)
    cmsStageFree(stage);

    // Silence unused-variable warnings for entities we intentionally keep around
    (void) dupCtx;
    (void) hGBD;
    (void) dataFormats;
    (void) dfCount;
    (void) stageSig;
    (void) stageInCh;
    (void) profileVer;
    (void) renderingIntent;
    (void) md5ok;
    (void) profCtx;
    (void) hIT8;

    return 66;
}