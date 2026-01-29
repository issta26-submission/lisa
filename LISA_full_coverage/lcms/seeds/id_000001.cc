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
//<ID> 1
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsUInt32Number supportedCodes[4];
    char * supportedDescriptions[4];
    memset(supportedCodes, 0, sizeof(supportedCodes));
    memset(supportedDescriptions, 0, sizeof(supportedDescriptions));
    cmsHPROFILE hProfile = NULL;
    cmsPipeline * pipeline = NULL;
    cmsStage * stage = NULL;
    cmsStage * firstStagePtr = NULL;
    cmsFloat64Number profileVersion = 0.0;

    // step 2: Setup
    cmsUInt32Number nReturned = cmsGetSupportedIntentsTHR(ctx, 4, supportedCodes, supportedDescriptions);
    hProfile = cmsCreateBCHSWabstractProfileTHR(ctx, 17, 0.5, 1.10, 0.0, 1.0, 5000, 6500);

    // step 3: Core operations
    pipeline = cmsPipelineAlloc(ctx, 3, 3);
    static const float clutTable[] = { 0.0f, 0.5f, 1.0f }; // simple small table for CLUT creation
    stage = cmsStageAllocCLutFloat(ctx, 2, 3, 3, clutTable);
    firstStagePtr = cmsPipelineGetPtrToFirstStage(pipeline);
    profileVersion = cmsGetProfileVersion(hProfile);
    cmsSetHeaderFlags(hProfile, 0x01);

    // step 4: Cleanup
    cmsStageFree(stage);
    cmsPipelineFree(pipeline);

    // API sequence test completed successfully
    (void)nReturned;
    (void)firstStagePtr;
    (void)profileVersion;
    (void)supportedDescriptions;
    return 66;
}