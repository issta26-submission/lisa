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
//<ID> 38
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE srcProfile = (cmsHPROFILE) NULL;
    cmsHPROFILE inkLimitProfile = (cmsHPROFILE) NULL;
    cmsNAMEDCOLORLIST * ncl = (cmsNAMEDCOLORLIST *) NULL;
    cmsUInt16Number alarmCodes[16];
    cmsFloat64Number version = 0.0;
    cmsFloat64Number limit = 0.0;

    // step 2: Setup (create profile, compute limit, allocate named color list, prepare alarms)
    srcProfile = cmsCreate_sRGBProfileTHR(ctx);
    version = cmsGetProfileVersion(srcProfile);
    limit = version * 0.1 + 0.05;
    ncl = cmsAllocNamedColorList(ctx, 4U, 3U, "Pfx", "Sfx");
    memset(alarmCodes, 0, sizeof(alarmCodes));
    alarmCodes[0] = (cmsUInt16Number) ((version * 1000.0) > 0 ? (int)(version * 1000.0) : 1);
    alarmCodes[1] = (cmsUInt16Number) ((int)(limit * 1000.0) & 0xFFFF);

    // step 3: Operate & Validate (set alarm codes and create ink limiting device link)
    cmsSetAlarmCodesTHR(ctx, alarmCodes);
    inkLimitProfile = cmsCreateInkLimitingDeviceLinkTHR(ctx, (cmsColorSpaceSignature) 0, limit);

    // step 4: Cleanup (close profiles and free named color list)
    cmsCloseProfile(srcProfile);
    cmsCloseProfile(inkLimitProfile);
    cmsFreeNamedColorList(ncl);

    // API sequence test completed successfully
    return 66;
}