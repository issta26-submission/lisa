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
//<ID> 102
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
    cmsStage * stage = NULL;
    cmsUInt32Number inputChanCount = 0;
    cmsUInt32Number clutPoints[3];
    cmsUInt16Number clutTable[8];
    cmsCIExyY whitePoint;
    cmsFloat64Number version = 4.4;

    // step 2: Setup (initialize arrays and create Lab4 profile)
    memset(clutPoints, 0, sizeof(clutPoints));
    memset(clutTable, 0, sizeof(clutTable));
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;
    hProfile = cmsCreateLab4ProfileTHR(ctx, &whitePoint);
    cmsSetProfileVersion(hProfile, version);

    // step 3: Operate (prepare CLUT descriptor, allocate CLUT stage, query channels, set header model)
    clutPoints[0] = 2U;
    clutPoints[1] = 2U;
    clutPoints[2] = 2U;
    clutTable[0] = (cmsUInt16Number)0;
    clutTable[1] = (cmsUInt16Number)21845;
    clutTable[2] = (cmsUInt16Number)43690;
    clutTable[3] = (cmsUInt16Number)65535;
    clutTable[4] = (cmsUInt16Number)32767;
    clutTable[5] = (cmsUInt16Number)16383;
    clutTable[6] = (cmsUInt16Number)49151;
    clutTable[7] = (cmsUInt16Number)8191;
    stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, 3U, 4U, clutTable);
    inputChanCount = cmsStageInputChannels(stage);
    cmsSetHeaderModel(hProfile, inputChanCount);

    // step 4: Validate and Cleanup
    cmsCloseProfile(hProfile);
    (void)stage;
    (void)clutPoints;
    (void)clutTable;
    (void)whitePoint;
    (void)version;
    (void)inputChanCount;

    // API sequence test completed successfully
    return 66;
}