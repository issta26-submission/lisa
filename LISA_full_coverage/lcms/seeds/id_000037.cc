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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = (cmsContext) NULL;
    cmsHPROFILE srcProfile = (cmsHPROFILE) NULL;
    cmsHPROFILE inkProfile = (cmsHPROFILE) NULL;
    cmsNAMEDCOLORLIST * ncl = (cmsNAMEDCOLORLIST *) NULL;
    cmsUInt16Number alarmCodes[16];
    cmsFloat64Number version = 0.0;
    cmsFloat64Number limit = 0.0;
    cmsUInt32Number nColors = 4;
    cmsUInt32Number colorantCount = 3;
    const char * prefix = "Pref";
    const char * suffix = "Suff";

    // step 2: Setup (Create context, source profile and named color list; prepare alarm codes)
    ctx = cmsCreateContext((void *) NULL, (void *) NULL);
    srcProfile = cmsCreate_sRGBProfileTHR(ctx);
    version = cmsGetProfileVersion(srcProfile);
    limit = version / (version + 1.0); /* derive a stable limit from profile version */
    ncl = cmsAllocNamedColorList(ctx, nColors, colorantCount, prefix, suffix);
    memset(alarmCodes, 0, sizeof(alarmCodes));
    alarmCodes[0] = (cmsUInt16Number) 100;
    alarmCodes[1] = (cmsUInt16Number) 200;
    alarmCodes[2] = (cmsUInt16Number) 300;

    // step 3: Operate (Create ink-limiting device-link using computed limit and set alarm codes)
    inkProfile = cmsCreateInkLimitingDeviceLinkTHR(ctx, (cmsColorSpaceSignature) 0, limit);
    cmsSetAlarmCodesTHR(ctx, alarmCodes);

    // step 4: Validate & Cleanup
    cmsFreeNamedColorList(ncl);
    cmsCloseProfile(inkProfile);
    cmsCloseProfile(srcProfile);

    // API sequence test completed successfully
    return 66;
}