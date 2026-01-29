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
//<ID> 105
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
    cmsCIExyY white;
    cmsUInt32Number model = 0x6D6F646C;
    cmsFloat64Number version = 4.4;
    cmsUInt32Number clutPoints[3];
    cmsUInt16Number table[8];
    cmsStage * stage = NULL;
    cmsUInt32Number inputChannels = 0;

    // step 2: Setup
    white.x = 0.3127;
    white.y = 0.3290;
    white.Y = 1.0;
    hProfile = cmsCreateLab4ProfileTHR(ctx, &white);
    cmsSetProfileVersion(hProfile, version);
    cmsSetHeaderModel(hProfile, model);

    // step 3: Operate & Validate
    clutPoints[0] = 2U;
    clutPoints[1] = 2U;
    clutPoints[2] = 2U;
    table[0] = (cmsUInt16Number)0;
    table[1] = (cmsUInt16Number)32768;
    table[2] = (cmsUInt16Number)32768;
    table[3] = (cmsUInt16Number)65535;
    table[4] = (cmsUInt16Number)0;
    table[5] = (cmsUInt16Number)16384;
    table[6] = (cmsUInt16Number)49152;
    table[7] = (cmsUInt16Number)65535;
    stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, 3U, 4U, table);
    inputChannels = cmsStageInputChannels(stage);

    // step 4: Cleanup
    cmsCloseProfile(hProfile);
    (void)ctx;
    (void)stage;
    (void)inputChannels;
    (void)clutPoints;
    (void)table;
    (void)model;
    (void)version;

    // API sequence test completed successfully
    return 66;
}