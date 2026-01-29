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
//<ID> 32
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations (Initialize)
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsNAMEDCOLORLIST * ncl = (cmsNAMEDCOLORLIST *) NULL;
    cmsHPROFILE inkProfile = (cmsHPROFILE) NULL;
    cmsUInt16Number alarmCodes[16];
    cmsUInt32Number nCount = 0;
    cmsFloat64Number limitValue = 0.0;

    // step 2: Setup (Create named color list and compute ink limit)
    ncl = cmsAllocNamedColorList(ctx, 3U, 4U, "Pref", "Suf");
    nCount = cmsNamedColorCount(ncl);
    limitValue = (cmsFloat64Number) nCount * 0.15 + 0.05;
    inkProfile = cmsCreateInkLimitingDeviceLinkTHR(ctx, (cmsColorSpaceSignature) 0, limitValue);

    // step 3: Operate (Configure alarm codes and apply)
    memset(alarmCodes, 0, sizeof(alarmCodes));
    alarmCodes[0] = (cmsUInt16Number) (((unsigned int)(limitValue * 1000.0)) & 0xFFFFU);
    alarmCodes[1] = (cmsUInt16Number) (nCount & 0xFFFFU);
    cmsSetAlarmCodesTHR(ctx, alarmCodes);

    // step 4: Validate & Cleanup
    (void) cmsCloseProfile(inkProfile);
    cmsFreeNamedColorList(ncl);

    // API sequence test completed successfully
    return 66;
}