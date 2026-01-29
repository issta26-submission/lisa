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
//<ID> 103
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE hLab = NULL;
    cmsStage * stage = NULL;
    cmsUInt32Number clutPoints[3];
    cmsUInt16Number Table[24];
    cmsCIExyY whitePoint;
    cmsFloat64Number version = 4.4;
    cmsUInt32Number inChannels = 0;

    // step 2: Setup
    clutPoints[0] = 2U;
    clutPoints[1] = 2U;
    clutPoints[2] = 2U;
    Table[0]  = (cmsUInt16Number)0;
    Table[1]  = (cmsUInt16Number)2730;
    Table[2]  = (cmsUInt16Number)5460;
    Table[3]  = (cmsUInt16Number)8190;
    Table[4]  = (cmsUInt16Number)10920;
    Table[5]  = (cmsUInt16Number)13650;
    Table[6]  = (cmsUInt16Number)16380;
    Table[7]  = (cmsUInt16Number)19110;
    Table[8]  = (cmsUInt16Number)21840;
    Table[9]  = (cmsUInt16Number)24570;
    Table[10] = (cmsUInt16Number)27300;
    Table[11] = (cmsUInt16Number)30030;
    Table[12] = (cmsUInt16Number)32760;
    Table[13] = (cmsUInt16Number)35490;
    Table[14] = (cmsUInt16Number)38220;
    Table[15] = (cmsUInt16Number)40950;
    Table[16] = (cmsUInt16Number)43680;
    Table[17] = (cmsUInt16Number)46410;
    Table[18] = (cmsUInt16Number)49140;
    Table[19] = (cmsUInt16Number)51870;
    Table[20] = (cmsUInt16Number)54600;
    Table[21] = (cmsUInt16Number)57330;
    Table[22] = (cmsUInt16Number)60060;
    Table[23] = (cmsUInt16Number)62790;
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;

    // step 3: Core operations
    hLab = cmsCreateLab4ProfileTHR(ctx, &whitePoint);
    cmsSetProfileVersion(hLab, version);
    stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, 3U, 3U, Table);
    inChannels = cmsStageInputChannels(stage);
    cmsSetHeaderModel(hLab, inChannels);

    // step 4: Cleanup
    cmsCloseProfile(hLab);
    (void)ctx;
    (void)stage;
    (void)Table;
    (void)clutPoints;
    (void)whitePoint;
    (void)version;
    (void)inChannels;
    // API sequence test completed successfully
    return 66;
}