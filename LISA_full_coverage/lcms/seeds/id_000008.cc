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
//<ID> 8
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Initialize and Configure
    cmsContext ContextID = NULL;
    cmsUInt32Number nLUTPoints = 16;
    cmsFloat64Number Bright = 0.5;
    cmsFloat64Number Contrast = 1.0;
    cmsFloat64Number Hue = 0.0;
    cmsFloat64Number Saturation = 1.0;
    cmsUInt32Number TempSrc = 6500;
    cmsUInt32Number TempDest = 5000;
    cmsHPROFILE profile = cmsCreateBCHSWabstractProfileTHR(ContextID, nLUTPoints, Bright, Contrast, Hue, Saturation, TempSrc, TempDest);
    cmsUInt32Number inputChan = 3;
    cmsUInt32Number outputChan = 3;
    cmsPipeline * pipeline = cmsPipelineAlloc(ContextID, inputChan, outputChan);

    // step 2: Operate
    const cmsUInt32Number maxIntents = 4;
    cmsUInt32Number intentCodes[4];
    char * intentDescriptions[4];
    memset(intentCodes, 0, sizeof(intentCodes));
    memset(intentDescriptions, 0, sizeof(intentDescriptions));
    cmsUInt32Number returned = cmsGetSupportedIntentsTHR(ContextID, maxIntents, intentCodes, intentDescriptions);
    cmsUInt32Number nGridPoints = 2;
    cmsStage * stage = cmsStageAllocCLutFloat(ContextID, nGridPoints, inputChan, outputChan, NULL);

    // step 3: Validate
    cmsStage * firstStage = cmsPipelineGetPtrToFirstStage(pipeline);
    cmsUInt32Number combined = returned + intentCodes[0] + (cmsUInt32Number)(pipeline != NULL) + (cmsUInt32Number)(firstStage != NULL);
    (void)combined; // use computed value to show data flow

    // step 4: Cleanup
    cmsStageFree(stage);
    cmsPipelineFree(pipeline);
    memset(intentDescriptions, 0, sizeof(intentDescriptions));
    // API sequence test completed successfully
    return 66;
}