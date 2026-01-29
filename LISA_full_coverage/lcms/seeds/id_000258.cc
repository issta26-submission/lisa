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
//<ID> 258
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsHPROFILE profile = cmsCreateProfilePlaceholder((cmsContext)0);
    cmsUInt32Number clutPoints[3];
    clutPoints[0] = (cmsUInt32Number)2;
    clutPoints[1] = (cmsUInt32Number)2;
    clutPoints[2] = (cmsUInt32Number)2;
    cmsUInt32Number inputChannels = (cmsUInt32Number)3;
    cmsUInt32Number outputChannels = (cmsUInt32Number)3;
    unsigned short table[24];
    memset(table, 0, sizeof(table));
    cmsStage * stage = cmsStageAllocCLut16bitGranular((cmsContext)0, clutPoints, inputChannels, outputChannels, (const cmsUInt16Number *)table);

    // step 2: Configure
    (void)clutPoints;
    (void)inputChannels;
    (void)outputChannels;
    (void)table;

    // step 3: Operate
    cmsFloat64Number version = cmsGetProfileVersion(profile);
    cmsColorSpaceSignature pcs = cmsGetPCS(profile);
    cmsInt32Number channelsFromSpace = cmsChannelsOfColorSpace(pcs);
    cmsUInt32Number stageOut = cmsStageOutputChannels(stage);
    cmsStage * nextStage = cmsStageNext(stage);

    // step 4: Validate & Cleanup
    (void)version;
    (void)channelsFromSpace;
    (void)stageOut;
    (void)nextStage;
    cmsStageFree(stage);
    cmsCloseProfile(profile);

    // API sequence test completed successfully
    return 66;
}