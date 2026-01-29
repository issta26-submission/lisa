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
//<ID> 101
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations (Initialize)
    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsCIExyY whitePoint;
    cmsUInt32Number clutPoints[3];
    cmsUInt16Number table[32];
    cmsStage * stage = NULL;
    cmsUInt32Number inputCh = 0;
    cmsFloat64Number version = 2.1;

    // step 2: Setup (Configure)
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;
    clutPoints[0] = 3U;
    clutPoints[1] = 3U;
    clutPoints[2] = 3U;
    memset(table, 0, sizeof(table));
    table[0] = (cmsUInt16Number)0;
    table[1] = (cmsUInt16Number)32768;
    table[2] = (cmsUInt16Number)65535;
    hProfile = cmsCreateLab4ProfileTHR(ctx, &whitePoint);

    // step 3: Operate (create stage, query, configure profile)
    stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, 3U, 3U, table);
    inputCh = cmsStageInputChannels(stage);
    cmsSetHeaderModel(hProfile, inputCh);
    cmsSetProfileVersion(hProfile, version);

    // step 4: Validate & Cleanup
    cmsCloseProfile(hProfile);
    (void)ctx;
    (void)stage;
    (void)clutPoints;
    (void)table;
    (void)inputCh;
    (void)whitePoint;
    (void)version;

    // API sequence test completed successfully
    return 66;
}