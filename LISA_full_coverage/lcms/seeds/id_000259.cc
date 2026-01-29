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
//<ID> 259
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsCIExyY wp;
    memset(&wp, 0, sizeof(wp));
    wp.x = (cmsFloat64Number)0.3127;
    wp.y = (cmsFloat64Number)0.3290;
    wp.Y = (cmsFloat64Number)1.0;
    cmsHPROFILE profile = cmsCreateLab2Profile(&wp);
    cmsFloat64Number version = cmsGetProfileVersion(profile);
    cmsColorSpaceSignature pcs = cmsGetPCS(profile);
    cmsUInt32Number channels = (cmsUInt32Number)cmsChannelsOfColorSpace(pcs);

    // step 2: Configure
    cmsUInt32Number clutPoints[4];
    clutPoints[0] = (cmsUInt32Number)2;
    clutPoints[1] = (cmsUInt32Number)2;
    clutPoints[2] = (cmsUInt32Number)2;
    clutPoints[3] = (cmsUInt32Number)2;
    cmsStage * stage = cmsStageAllocCLut16bitGranular((cmsContext)NULL, clutPoints, channels, channels, (const cmsUInt16Number *)NULL);

    // step 3: Operate
    cmsStage * next_stage = cmsStageNext(stage);
    cmsUInt32Number out_channels = cmsStageOutputChannels(stage);
    (void)version;
    (void)pcs;
    (void)channels;
    (void)next_stage;
    (void)out_channels;

    // step 4: Validate & Cleanup
    if (stage) cmsStageFree(stage);
    cmsCloseProfile(profile);
    // API sequence test completed successfully
    return 66;
}