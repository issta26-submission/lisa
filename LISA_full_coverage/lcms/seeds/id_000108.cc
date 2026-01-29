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
//<ID> 108
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsCIExyY whitePoint;
    cmsHPROFILE hProfile = NULL;
    cmsStage * stage = NULL;
    cmsUInt32Number clutPoints[4];
    cmsUInt32Number inputChan = 4;
    cmsUInt32Number outputChan = 3;
    cmsUInt16Number table[48] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
        12,13,14,15,16,17,18,19,20,21,22,23,
        24,25,26,27,28,29,30,31,32,33,34,35,
        36,37,38,39,40,41,42,43,44,45,46,47
    };
    cmsUInt32Number reportedInputs = 0;
    cmsFloat64Number version = 4.4;

    // step 2: Setup
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;
    hProfile = cmsCreateLab4ProfileTHR(ctx, &whitePoint);
    cmsSetHeaderModel(hProfile, 0x6D6F646C);
    cmsSetProfileVersion(hProfile, version);

    // step 3: Operate
    clutPoints[0] = 2;
    clutPoints[1] = 2;
    clutPoints[2] = 2;
    clutPoints[3] = 2;
    stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, inputChan, outputChan, table);
    reportedInputs = cmsStageInputChannels(stage);
    cmsSetHeaderModel(hProfile, reportedInputs);

    // step 4: Validate and Cleanup
    cmsCloseProfile(hProfile);
    (void)ctx;
    (void)whitePoint;
    (void)stage;
    (void)clutPoints;
    (void)table;
    (void)inputChan;
    (void)outputChan;
    (void)reportedInputs;
    (void)version;

    // API sequence test completed successfully
    return 66;
}