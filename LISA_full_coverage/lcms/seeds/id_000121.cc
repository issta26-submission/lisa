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
//<ID> 121
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsMLU * mlu = NULL;
    cmsStage * stage = NULL;
    cmsFloat64Number tac = 0.0;
    cmsUInt32Number outChannels = 0U;
    wchar_t wideBuf[64];
    cmsUInt32Number gotWide = 0U;

    // Step 2: Setup
    mlu = cmsMLUalloc(ctx, 1U);
    hProfile = cmsCreateProfilePlaceholder(ctx);
    memset(wideBuf, 0, sizeof(wideBuf));

    // Step 3: Operate and Validate
    tac = cmsDetectTAC(hProfile);
    stage = cmsStageAllocCLutFloat(ctx, 2U, 3U, 3U, NULL);
    outChannels = cmsStageOutputChannels(stage);
    gotWide = cmsMLUgetWide(mlu, "en", "US", wideBuf, (cmsUInt32Number)(sizeof(wideBuf) / sizeof(wchar_t)));
    (void)tac;
    (void)outChannels;
    (void)gotWide;
    (void)wideBuf;

    // Step 4: Cleanup
    cmsMLUfree(mlu);
    cmsCloseProfile(hProfile);
    (void)stage;
    (void)ctx;

    return 66;
    // API sequence test completed successfully
}