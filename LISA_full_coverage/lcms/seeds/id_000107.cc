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
//<ID> 107
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
    cmsUInt16Number table[16];
    cmsUInt32Number inChannels = 0;
    cmsFloat64Number version = 4.5;

    // step 2: Setup (initialize white point, create profile, configure header & version)
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;
    hProfile = cmsCreateLab4ProfileTHR(ctx, &whitePoint);
    cmsSetHeaderModel(hProfile, (cmsUInt32Number)12345U);
    cmsSetProfileVersion(hProfile, version);

    // step 3: Operate (prepare CLUT parameters, allocate CLUT stage, query stage input channels, reconfigure header with result)
    clutPoints[0] = 2U;
    clutPoints[1] = 2U;
    clutPoints[2] = 2U;
    clutPoints[3] = 2U;
    table[0]  = (cmsUInt16Number)0U;
    table[1]  = (cmsUInt16Number)4369U;
    table[2]  = (cmsUInt16Number)8738U;
    table[3]  = (cmsUInt16Number)13107U;
    table[4]  = (cmsUInt16Number)17476U;
    table[5]  = (cmsUInt16Number)21845U;
    table[6]  = (cmsUInt16Number)26214U;
    table[7]  = (cmsUInt16Number)30583U;
    table[8]  = (cmsUInt16Number)34952U;
    table[9]  = (cmsUInt16Number)39321U;
    table[10] = (cmsUInt16Number)43690U;
    table[11] = (cmsUInt16Number)48059U;
    table[12] = (cmsUInt16Number)52428U;
    table[13] = (cmsUInt16Number)56797U;
    table[14] = (cmsUInt16Number)61166U;
    table[15] = (cmsUInt16Number)65535U;
    stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, 4U, 4U, table);
    inChannels = cmsStageInputChannels(stage);
    cmsSetHeaderModel(hProfile, inChannels);

    // step 4: Validate & Cleanup (close profile and clear local references)
    cmsCloseProfile(hProfile);
    stage = NULL;
    (void)ctx;
    (void)whitePoint;
    (void)clutPoints;
    (void)table;
    (void)inChannels;
    (void)version;

    return 66;
    // API sequence test completed successfully
}