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
//<ID> 251
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE profile = cmsCreateProfilePlaceholder(ctx);
    cmsUInt32Number clutPoints[3];
    clutPoints[0] = (cmsUInt32Number)2;
    clutPoints[1] = (cmsUInt32Number)2;
    clutPoints[2] = (cmsUInt32Number)2;
    unsigned short table[8];
    memset(table, 0, sizeof(table));
    cmsStage * stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, (cmsUInt32Number)3, (cmsUInt32Number)3, (const cmsUInt16Number *)table);

    // step 2: Configure
    cmsFloat64Number profVersion = cmsGetProfileVersion(profile);
    cmsColorSpaceSignature pcs = cmsGetPCS(profile);
    cmsInt32Number csChannels = cmsChannelsOfColorSpace(pcs);
    cmsUInt32Number stageOutChannels = cmsStageOutputChannels(stage);
    cmsStage * nextStage = cmsStageNext(stage);

    // step 3: Operate
    (void)profVersion;
    (void)csChannels;
    (void)stageOutChannels;
    (void)nextStage;

    // step 4: Validate & Cleanup
    cmsCloseProfile(profile);
    cmsStageFree(stage);
    cmsDeleteContext(ctx);

    // API sequence test completed successfully
    return 66;
}