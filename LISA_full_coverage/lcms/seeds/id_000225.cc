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
//<ID> 225
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = (cmsContext)NULL;
    cmsUInt16Number alarms[16] = {0};
    alarms[0] = (cmsUInt16Number)0x0001U;
    alarms[1] = (cmsUInt16Number)0x0002U;

    // step 2: Configure global alarm codes for this (NULL) context
    cmsSetAlarmCodesTHR(ctx, alarms);

    // step 3: Create an sRGB profile using the (NULL) context
    cmsHPROFILE hProfile = cmsCreate_sRGBProfileTHR(ctx);

    // step 4: Inspect header information from the created profile
    cmsUInt32Number origModel = cmsGetHeaderModel(hProfile);
    cmsUInt32Number origManufacturer = cmsGetHeaderManufacturer(hProfile);

    // step 5: Query supported intents (thread-safe variant) using the (NULL) context
    cmsUInt32Number intentCodes[4];
    char *intentDescriptions[4];
    cmsUInt32Number nIntentsReturned = cmsGetSupportedIntentsTHR(ctx, (cmsUInt32Number)4, intentCodes, intentDescriptions);

    // step 6: Allocate a simple pipeline and query some properties
    cmsPipeline * pipeline = cmsPipelineAlloc(ctx, (cmsUInt32Number)3, (cmsUInt32Number)3);
    cmsUInt32Number stageCount = cmsPipelineStageCount(pipeline);
    cmsUInt32Number inputChannels = cmsPipelineInputChannels(pipeline);

    // step 7: Cleanup resources created above
    cmsPipelineFree(pipeline);
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);

    // keep variables referenced to avoid unused-variable warnings in straight-line code
    (void)origManufacturer;
    (void)nIntentsReturned;
    (void)stageCount;
    (void)inputChannels;
    (void)intentDescriptions;
    (void)intentCodes;
    (void)origModel;
    return 66;
}