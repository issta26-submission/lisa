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
//<ID> 106
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsStage *stage = NULL;
    cmsUInt32Number clutPoints[4];
    cmsUInt16Number table[64];
    cmsUInt32Number inputChannelsFromStage = 0;
    cmsUInt32Number model = 0x6D6F646C;
    cmsFloat64Number version = 4.5;
    cmsCIExyY white;

    // step 2: Setup
    clutPoints[0] = 2U; clutPoints[1] = 2U; clutPoints[2] = 2U; clutPoints[3] = 2U;
    white.x = 0.3127;
    white.y = 0.3290;
    white.Y = 1.0;
    memset(table, 0, sizeof(table));
    hProfile = cmsCreateLab4ProfileTHR(ctx, &white);
    cmsSetHeaderModel(hProfile, model);
    cmsSetProfileVersion(hProfile, version);

    // step 3: Operate / Validate
    stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, 4U, 4U, table);
    inputChannelsFromStage = cmsStageInputChannels(stage);
    cmsSetHeaderModel(hProfile, model + inputChannelsFromStage);

    // step 4: Cleanup
    cmsCloseProfile(hProfile);
    (void)ctx;
    (void)stage;
    (void)clutPoints;
    (void)table;
    (void)inputChannelsFromStage;
    (void)version;
    (void)white;
    (void)model;
    return 66;
    // API sequence test completed successfully
}