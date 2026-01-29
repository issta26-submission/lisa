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
//<ID> 257
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsCIExyY whitePoint;
    memset(&whitePoint, 0, sizeof(whitePoint));
    cmsHPROFILE profile = cmsCreateLab4ProfileTHR(ctx, &whitePoint);
    cmsFloat64Number version = cmsGetProfileVersion(profile);

    // step 2: Configure
    cmsColorSpaceSignature pcs = cmsGetPCS(profile);
    cmsInt32Number channels = cmsChannelsOfColorSpace(pcs);
    cmsPipeline * lut = cmsPipelineAlloc(ctx, (cmsUInt32Number)channels, (cmsUInt32Number)channels);
    cmsUInt32Number clutPoints[4];
    clutPoints[0] = (cmsUInt32Number)2;
    clutPoints[1] = (cmsUInt32Number)2;
    clutPoints[2] = (cmsUInt32Number)2;
    clutPoints[3] = (cmsUInt32Number)2;
    cmsUInt16Number table[256];
    memset(table, 0, sizeof(table));
    cmsStage * stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, (cmsUInt32Number)channels, (cmsUInt32Number)channels, table);

    // step 3: Operate
    cmsUInt32Number outChannels = cmsStageOutputChannels(stage);
    cmsStage * next = cmsStageNext(stage);

    // step 4: Validate & Cleanup
    (void)version;
    (void)pcs;
    (void)channels;
    (void)lut;
    (void)outChannels;
    (void)next;
    cmsStageFree(stage);
    cmsCloseProfile(profile);
    cmsDeleteContext(ctx);

    // API sequence test completed successfully
    return 66;
}