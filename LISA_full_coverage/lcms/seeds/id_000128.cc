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
//<ID> 128
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations (Initialize)
    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsStage * stage = NULL;
    cmsMLU * mlu = NULL;
    cmsUInt32Number outChannels = 0U;
    cmsUInt32Number got = 0U;
    cmsFloat64Number tacValue = 0.0;
    wchar_t wideBuffer[64];
    static const float clutTable[16] = {
        0.0f, 0.125f, 0.25f, 0.375f,
        0.5f, 0.625f, 0.75f, 0.875f,
        1.0f, 0.875f, 0.75f, 0.625f,
        0.5f, 0.375f, 0.25f, 0.125f
    };

    // step 2: Setup (Create placeholder profile and allocate a CLUT stage)
    hProfile = cmsCreateProfilePlaceholder(ctx);
    stage = cmsStageAllocCLutFloat(ctx, 2U, 3U, 2U, clutTable);

    // step 3: Operate (Query stage channels, allocate MLU and retrieve wide string, detect TAC)
    outChannels = cmsStageOutputChannels(stage);
    mlu = cmsMLUalloc(ctx, outChannels);
    memset(wideBuffer, 0, sizeof(wideBuffer));
    got = cmsMLUgetWide(mlu, "en", "US", wideBuffer, (cmsUInt32Number)(sizeof(wideBuffer) / sizeof(wchar_t)));
    tacValue = cmsDetectTAC(hProfile);

    // step 4: Cleanup (Free allocated resources and silence unused warnings)
    cmsMLUfree(mlu);
    cmsCloseProfile(hProfile);
    (void)ctx;
    (void)stage;
    (void)outChannels;
    (void)got;
    (void)tacValue;
    (void)wideBuffer;

    return 66;
    // API sequence test completed successfully
}