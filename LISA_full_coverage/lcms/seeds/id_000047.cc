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
//<ID> 47
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsPipeline * pipeline = cmsPipelineAlloc(NULL, 3U, 3U);
    cmsHPROFILE hProfile = cmsCreateLab4Profile(NULL);
    cmsUInt16Number alarmCodes[16];
    cmsUInt32Number stageCount = 0;
    cmsFloat64Number value = 0.0;
    int row = 0;
    int col = 1;

    // step 2: Setup (no loops or branches; explicit assignments)
    alarmCodes[0] = 100;
    alarmCodes[1] = 101;
    alarmCodes[2] = 102;
    alarmCodes[3] = 103;
    alarmCodes[4] = 104;
    alarmCodes[5] = 105;
    alarmCodes[6] = 106;
    alarmCodes[7] = 107;
    alarmCodes[8] = 108;
    alarmCodes[9] = 109;
    alarmCodes[10] = 110;
    alarmCodes[11] = 111;
    alarmCodes[12] = 112;
    alarmCodes[13] = 113;
    alarmCodes[14] = 114;
    alarmCodes[15] = 115;
    cmsSetAlarmCodes(alarmCodes);
    cmsPipelineSetSaveAs8bitsFlag(pipeline, 1);

    // step 3: Operate and Validate
    stageCount = cmsGetHeaderCreator(hProfile);
    row = (int) stageCount;
    value = (cmsFloat64Number) stageCount * 1.25 + 0.5;

    {
        float in[3]  = { 0.0f, 0.0f, 0.0f };
        float out[3] = { 0.0f, 0.0f, 0.0f };
        cmsPipelineEvalFloat((const cmsFloat32Number *) in, (cmsFloat32Number *) out, pipeline);
        (void)out; // use out to avoid unused warnings
    }

    // step 4: Cleanup
    cmsPipelineFree(pipeline);

    // API sequence test completed successfully
    return 66;
}