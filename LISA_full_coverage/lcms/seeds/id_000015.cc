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
//<ID> 15
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE profile_bchsw = NULL;
    cmsHPROFILE profile_inklim = NULL;
    cmsStage * stage = NULL;
    cmsHANDLE it8 = NULL;
    char patch_name_buf[128];
    cmsUInt32Number headerFlags = 0;
    const cmsFloat32Number clut_table[24] = {
        0.0f, 0.125f, 0.25f,
        0.375f, 0.5f, 0.625f,
        0.75f, 0.875f, 1.0f,
        0.1f, 0.2f, 0.3f,
        0.4f, 0.5f, 0.6f,
        0.7f, 0.8f, 0.9f,
        0.05f, 0.15f, 0.25f,
        0.35f, 0.45f, 0.55f
    };

    // step 2: Setup - create profiles and IT8 handle
    profile_bchsw = cmsCreateBCHSWabstractProfileTHR(ctx, 33U, 1.0, 1.0, 0.0, 1.0, 5000U, 6500U);
    profile_inklim = cmsCreateInkLimitingDeviceLinkTHR(ctx, _cmsICCcolorSpace(0), 0.95);
    it8 = cmsIT8Alloc(ctx);
    memset(patch_name_buf, 0, sizeof(patch_name_buf));

    // step 3: Core operations - allocate CLUT stage, query header flags, read IT8 patch name
    stage = cmsStageAllocCLutFloat(ctx, 2U, 3U, 3U, clut_table);
    headerFlags = cmsGetHeaderFlags(profile_bchsw);
    (void) headerFlags;
    (void) cmsStageData(stage);
    (void) cmsStageInputChannels(stage);
    const char * patch_name = cmsIT8GetPatchName(it8, 0, patch_name_buf);
    (void) patch_name;

    // step 4: Cleanup
    cmsStageFree(stage);
    cmsIT8Free(it8);
    cmsDeleteContext(ctx);

    // API sequence test completed successfully
    return 66;
}