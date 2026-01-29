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
//<ID> 253
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsUInt32Number clutPoints[3];
    clutPoints[0] = (cmsUInt32Number)2;
    clutPoints[1] = (cmsUInt32Number)2;
    clutPoints[2] = (cmsUInt32Number)2;
    cmsUInt16Number table[2 * 2 * 2 * 3];
    memset(table, 0, sizeof(table));
    cmsStage * stage = cmsStageAllocCLut16bitGranular((cmsContext)NULL, clutPoints, (cmsUInt32Number)3, (cmsUInt32Number)3, table);
    cmsHPROFILE profile = cmsCreateXYZProfileTHR((cmsContext)NULL);

    // step 2: Configure
    cmsUInt32Number stageOutChannels = cmsStageOutputChannels(stage);
    cmsColorSpaceSignature pcs = cmsGetPCS(profile);
    cmsUInt32Number pcsChannels = cmsChannelsOfColorSpace(pcs);

    // step 3: Operate
    cmsStage * nextStage = cmsStageNext(stage);
    cmsFloat64Number version = cmsGetProfileVersion(profile);
    cmsUInt32Number combinedMetric = stageOutChannels + pcsChannels + (cmsUInt32Number)version;

    // step 4: Validate & Cleanup
    cmsStageFree(stage);
    cmsCloseProfile(profile);
    (void)nextStage;
    (void)combinedMetric;
    // API sequence test completed successfully
    return 66;
}