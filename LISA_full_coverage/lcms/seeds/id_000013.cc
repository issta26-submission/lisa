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
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE bchProfile = NULL;
    cmsHPROFILE inkProfile = NULL;
    cmsHANDLE hIT8 = NULL;
    cmsStage * clutStage = NULL;
    cmsUInt32Number headerFlags = 0;
    const char * patchName = NULL;
    char patchBuf[64];
    memset(patchBuf, 0, sizeof(patchBuf));

    // step 2: Setup - create profiles and IT8 handle
    bchProfile = cmsCreateBCHSWabstractProfileTHR(ctx, 16U, 50.0, 10.0, 0.0, 1.2, 5000U, 6500U);
    hIT8 = cmsIT8Alloc(ctx);

    // step 3: Core operations - inspect header, create ink-limiting link and CLUT stage, query IT8 patch
    headerFlags = cmsGetHeaderFlags(bchProfile);
    cmsColorSpaceSignature csig = _cmsICCcolorSpace(0);
    inkProfile = cmsCreateInkLimitingDeviceLinkTHR(ctx, csig, (cmsFloat64Number) headerFlags);

    static const cmsFloat32Number clutTable[24] = {
         0.0f, 0.25f, 0.5f,
         0.75f, 1.0f, 0.1f,
         0.2f, 0.3f, 0.4f,
         0.5f, 0.6f, 0.7f,
         0.8f, 0.9f, 1.0f,
         0.05f, 0.15f, 0.25f,
         0.35f, 0.45f, 0.55f,
         0.65f, 0.75f, 0.85f
    };
    clutStage = cmsStageAllocCLutFloat(ctx, 2U, 3U, 3U, clutTable);

    patchName = cmsIT8GetPatchName(hIT8, 0, patchBuf);

    // step 4: Cleanup - free allocated stage and IT8 handle, delete context
    if (clutStage) cmsStageFree(clutStage);
    if (hIT8) cmsIT8Free(hIT8);
    cmsDeleteContext(ctx);

    // API sequence test completed successfully
    (void) bchProfile;
    (void) inkProfile;
    (void) headerFlags;
    (void) patchName;
    return 66;
}