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
//<ID> 34
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations (Initialize)
    cmsContext ctx = (cmsContext) NULL;
    cmsHPROFILE baseProfile = (cmsHPROFILE) NULL;
    cmsHPROFILE inkLimitProfile = (cmsHPROFILE) NULL;
    cmsNAMEDCOLORLIST * namedList = (cmsNAMEDCOLORLIST *) NULL;
    cmsUInt16Number alarmCodes[16] = {
        0x0001, 0x0002, 0x0003, 0x0004,
        0x0005, 0x0006, 0x0007, 0x0008,
        0x0009, 0x000A, 0x000B, 0x000C,
        0x000D, 0x000E, 0x000F, 0x0010
    };
    cmsFloat64Number profileVersion = 0.0;
    cmsFloat64Number inkLimit = 0.0;
    cmsUInt32Number manufacturer = 0;
    cmsUInt32Number namedCount = 0;
    char scratch[64];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Setup (Create context, set alarms, create base profile)
    ctx = cmsCreateContext(NULL, NULL);
    cmsSetAlarmCodesTHR(ctx, alarmCodes);
    baseProfile = cmsCreate_sRGBProfileTHR(ctx);
    profileVersion = cmsGetProfileVersion(baseProfile);
    manufacturer = cmsGetHeaderManufacturer(baseProfile);
    inkLimit = (profileVersion - (cmsFloat64Number)((int) profileVersion)) * 0.5 + 0.01;
    inkLimitProfile = cmsCreateInkLimitingDeviceLinkTHR(ctx, (cmsColorSpaceSignature) 0, inkLimit);
    namedList = cmsAllocNamedColorList(ctx, 2U, 3U, "PFX", "SFX");

    // step 3: Operate (Use retrieved profile info to validate and query named list)
    (void) snprintf(scratch, sizeof(scratch), "ver=%.6g man=0x%08x limit=%.6g", profileVersion, (unsigned) manufacturer, inkLimit);
    namedCount = cmsNamedColorCount(namedList);
    (void) namedCount;

    // step 4: Validate & Cleanup
    cmsFreeNamedColorList(namedList);
    cmsCloseProfile(inkLimitProfile);
    cmsCloseProfile(baseProfile);

    // API sequence test completed successfully
    return 66;
}