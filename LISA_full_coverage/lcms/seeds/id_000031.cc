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
//<ID> 31
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations (Initialize)
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE srgbProfile = (cmsHPROFILE) NULL;
    cmsHPROFILE inkLimitProfile = (cmsHPROFILE) NULL;
    cmsNAMEDCOLORLIST * namedList = (cmsNAMEDCOLORLIST *) NULL;
    cmsUInt16Number alarmCodes[16];
    cmsFloat64Number profileVersion = 0.0;
    cmsFloat64Number limitValue = 0.0;

    // step 2: Setup (Create profiles and named color list)
    srgbProfile = cmsCreate_sRGBProfileTHR(ctx);
    profileVersion = cmsGetProfileVersion(srgbProfile);
    limitValue = profileVersion / 10.0 + 0.01;
    inkLimitProfile = cmsCreateInkLimitingDeviceLinkTHR(ctx, (cmsColorSpaceSignature) 0, limitValue);
    namedList = cmsAllocNamedColorList(ctx, 2U, 3U, "PFX", "SFX");

    // step 3: Operate (Prepare alarm codes and apply them)
    alarmCodes[0]  = (cmsUInt16Number) (limitValue * 1000.0);
    alarmCodes[1]  = (cmsUInt16Number) (limitValue * 900.0);
    alarmCodes[2]  = (cmsUInt16Number) (limitValue * 800.0);
    alarmCodes[3]  = (cmsUInt16Number) (limitValue * 700.0);
    alarmCodes[4]  = (cmsUInt16Number) (limitValue * 600.0);
    alarmCodes[5]  = (cmsUInt16Number) (limitValue * 500.0);
    alarmCodes[6]  = (cmsUInt16Number) (limitValue * 400.0);
    alarmCodes[7]  = (cmsUInt16Number) (limitValue * 300.0);
    alarmCodes[8]  = (cmsUInt16Number) (limitValue * 200.0);
    alarmCodes[9]  = (cmsUInt16Number) (limitValue * 150.0);
    alarmCodes[10] = (cmsUInt16Number) (limitValue * 120.0);
    alarmCodes[11] = (cmsUInt16Number) (limitValue * 110.0);
    alarmCodes[12] = (cmsUInt16Number) (limitValue * 105.0);
    alarmCodes[13] = (cmsUInt16Number) (limitValue * 102.0);
    alarmCodes[14] = (cmsUInt16Number) (limitValue * 101.0);
    alarmCodes[15] = (cmsUInt16Number) (limitValue * 100.0);
    cmsSetAlarmCodesTHR(ctx, alarmCodes);
    cmsFreeNamedColorList(namedList);

    // step 4: Cleanup (Close profiles)
    (void) cmsCloseProfile(inkLimitProfile);
    (void) cmsCloseProfile(srgbProfile);

    // API sequence test completed successfully
    return 66;
}