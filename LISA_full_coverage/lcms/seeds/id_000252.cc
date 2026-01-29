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
//<ID> 252
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE profile = cmsCreateXYZProfileTHR(ctx);
    cmsFloat64Number version = cmsGetProfileVersion(profile);
    cmsColorSpaceSignature pcs = cmsGetPCS(profile);
    cmsInt32Number chanCount = cmsChannelsOfColorSpace(pcs);

    // step 2: Configure
    cmsUInt32Number clutPoints[3];
    clutPoints[0] = (cmsUInt32Number)2;
    clutPoints[1] = (cmsUInt32Number)2;
    clutPoints[2] = (cmsUInt32Number)2;
    cmsUInt32Number tableSize = clutPoints[0] * clutPoints[1] * clutPoints[2] * (cmsUInt32Number)chanCount;
    cmsUInt16Number * table = (cmsUInt16Number *) malloc((size_t)tableSize * sizeof(cmsUInt16Number));
    memset(table, 0, (size_t)tableSize * sizeof(cmsUInt16Number));
    if (tableSize > 0) table[0] = (cmsUInt16Number)((cmsUInt32Number)version & 0xFFFFU);

    // step 3: Operate
    cmsStage * stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, (cmsUInt32Number)chanCount, (cmsUInt32Number)chanCount, table);
    cmsUInt32Number outChannels = cmsStageOutputChannels(stage);
    const cmsStage * nextStage = cmsStageNext(stage);

    // step 4: Validate & Cleanup
    (void)outChannels;
    (void)nextStage;
    cmsStageFree(stage);
    free(table);
    cmsCloseProfile(profile);
    cmsDeleteContext(ctx);

    // API sequence test completed successfully
    return 66;
}