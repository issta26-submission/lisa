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
//<ID> 33
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Initialize
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsUInt16Number alarmCodes[16];
    memset(alarmCodes, 0, sizeof(alarmCodes));
    cmsFloat64Number limitValue = 0.30;
    cmsHPROFILE inkProfile = (cmsHPROFILE) NULL;
    cmsNAMEDCOLORLIST * ncl = (cmsNAMEDCOLORLIST *) NULL;

    // step 2: Configure
    alarmCodes[0] = (cmsUInt16Number) (limitValue * 65535.0);
    alarmCodes[1] = (cmsUInt16Number) ((1.0 - limitValue) * 65535.0);
    alarmCodes[15] = (cmsUInt16Number) (limitValue * 32767.0);
    cmsSetAlarmCodesTHR(ctx, alarmCodes);

    // step 3: Operate
    inkProfile = cmsCreateInkLimitingDeviceLinkTHR(ctx, (cmsColorSpaceSignature) 0, limitValue);
    ncl = cmsAllocNamedColorList(ctx, 4U, 3U, "PFX", "SFX");

    // step 4: Validate & Cleanup
    cmsFreeNamedColorList(ncl);
    cmsCloseProfile(inkProfile);

    // API sequence test completed successfully
    return 66;
}