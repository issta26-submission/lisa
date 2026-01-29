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
//<ID> 11
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1
    cmsContext ctx = NULL;
    cmsHPROFILE prof_abstract = NULL;
    cmsHPROFILE prof_inklimit = NULL;
    cmsUInt32Number flags = 0;
    cmsStage * stage = NULL;
    cmsHANDLE it8 = NULL;
    char patchNameBuffer[128];
    cmsFloat32Number clutTable[24] = {
        0.0f, 0.0f, 0.0f,   0.25f, 0.25f, 0.25f,   0.5f, 0.5f, 0.5f,   0.75f, 0.75f, 0.75f,
        1.0f, 1.0f, 1.0f,   0.2f, 0.4f, 0.6f,      0.3f, 0.6f, 0.9f,   0.1f, 0.2f, 0.3f
    };

    // step 2
    prof_abstract = cmsCreateBCHSWabstractProfileTHR(ctx, 17U, 0.5, 0.6, 0.1, 0.9, 5000U, 6500U);
    prof_inklimit = cmsCreateInkLimitingDeviceLinkTHR(ctx, (cmsColorSpaceSignature)0, 0.85);
    flags = cmsGetHeaderFlags(prof_abstract);

    // step 3
    clutTable[0] = (cmsFloat32Number)((flags & 0xFF) * 0.001f + clutTable[0]);
    stage = cmsStageAllocCLutFloat(ctx, 2U, 3U, 3U, clutTable);

    // step 4
    it8 = cmsIT8Alloc(ctx);
    memset(patchNameBuffer, 0, sizeof(patchNameBuffer));
    (void) cmsIT8GetPatchName(it8, 0, patchNameBuffer);
    cmsStageFree(stage);
    cmsIT8Free(it8);
    // API sequence test completed successfully
    return 66;
}