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
//<ID> 100
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
    cmsUInt32Number clutPoints[1];
    cmsUInt16Number table[4];
    cmsUInt32Number inputChannels = 0;
    cmsFloat64Number version = 2.1;
    cmsUInt32Number model = 0x6D6F646C; /* 'modl' */
    cmsCIExyY whitePoint;
    char scratch[16];

    // step 2: Setup
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;
    hProfile = cmsCreateLab4ProfileTHR(ctx, &whitePoint);
    cmsSetHeaderModel(hProfile, model);
    cmsSetProfileVersion(hProfile, version);
    clutPoints[0] = 4;
    table[0] = (cmsUInt16Number)0;
    table[1] = (cmsUInt16Number)21845;
    table[2] = (cmsUInt16Number)43690;
    table[3] = (cmsUInt16Number)65535;
    memset(scratch, 0, sizeof(scratch));

    // step 3: Core operations
    stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, 1U, 1U, table);
    inputChannels = cmsStageInputChannels(stage);
    cmsSaveProfileToFile(hProfile, "lab4.icc");

    // step 4: Cleanup
    cmsCloseProfile(hProfile);
    (void)ctx;
    (void)stage;
    (void)inputChannels;
    (void)scratch;
    (void)version;
    (void)model;
    (void)clutPoints;
    (void)table;
    // API sequence test completed successfully
    return 66;
}