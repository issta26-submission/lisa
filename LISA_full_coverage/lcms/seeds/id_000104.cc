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
//<ID> 104
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
    cmsCIExyY whitePoint;
    cmsUInt32Number clutPoints[4];
    cmsUInt32Number inputChan = 4;
    cmsUInt32Number outputChan = 3;
    cmsUInt32Number model = 0x4D4F444C; /* arbitrary model tag value */
    cmsFloat64Number version = 4.3;
    cmsUInt16Number table[48];
    cmsUInt32Number queriedInputChannels = 0;

    // step 2: Setup (initialize white point, CLUT grid and data, create profile and configure header)
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;
    clutPoints[0] = 2;
    clutPoints[1] = 2;
    clutPoints[2] = 2;
    clutPoints[3] = 2;
    memset(table, 0, sizeof(table));
    hProfile = cmsCreateLab4ProfileTHR(ctx, &whitePoint);
    cmsSetProfileVersion(hProfile, version);
    cmsSetHeaderModel(hProfile, model);

    // step 3: Operate and Validate (allocate a 16-bit granular CLUT stage and query its input channels)
    stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, inputChan, outputChan, table);
    queriedInputChannels = cmsStageInputChannels(stage);

    // step 4: Cleanup (close profile and finalize)
    cmsCloseProfile(hProfile);

    // API sequence test completed successfully
    (void)ctx;
    (void)stage;
    (void)whitePoint;
    (void)clutPoints;
    (void)table;
    (void)queriedInputChannels;
    (void)model;
    (void)version;
    (void)inputChan;
    (void)outputChan;

    return 66;
}