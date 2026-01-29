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
//<ID> 30
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = (cmsContext) NULL;
    cmsUInt16Number alarmCodes[16];
    memset(alarmCodes, 0, sizeof(alarmCodes));
    cmsFloat64Number inkLimit = 0.85;
    cmsHPROFILE inkProfile = (cmsHPROFILE) NULL;
    cmsNAMEDCOLORLIST * ncl = (cmsNAMEDCOLORLIST *) NULL;
    cmsUInt32Number nameCount = 0;

    // step 2: Setup (Create context, configure alarms, create ink limiting profile)
    ctx = cmsCreateContext(NULL, NULL);
    alarmCodes[0] = 100U;
    alarmCodes[1] = 200U;
    alarmCodes[2] = 300U;
    cmsSetAlarmCodesTHR(ctx, alarmCodes);
    inkProfile = cmsCreateInkLimitingDeviceLinkTHR(ctx, (cmsColorSpaceSignature) 0, inkLimit);

    // step 3: Operate (Allocate a named color list and derive info into alarm codes)
    ncl = cmsAllocNamedColorList(ctx, 3U, 4U, "PFX", "SFX");
    nameCount = cmsNamedColorCount(ncl);
    alarmCodes[3] = (cmsUInt16Number) (nameCount & 0xFFFFU);

    // step 4: Cleanup (Free named color list and close created profile)
    cmsFreeNamedColorList(ncl);
    cmsCloseProfile(inkProfile);

    // API sequence test completed successfully
    return 66;
}