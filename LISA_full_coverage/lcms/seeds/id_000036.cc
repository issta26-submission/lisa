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
//<ID> 36
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
    cmsUInt16Number alarm[16];
    cmsFloat64Number version = 0.0;
    cmsFloat64Number limit = 0.0;
    int vint = 0;

    // step 2: Setup
    ctx = cmsCreateContext(NULL, NULL);
    srcProfile = cmsCreate_sRGBProfileTHR(ctx);
    version = cmsGetProfileVersion(srcProfile);
    vint = (int) (version * 100.0);
    limit = (cmsFloat64Number) (((vint % 30) + 10) ) / 100.0; /* limit in range 0.10..0.39 */
    memset(alarm, 0, sizeof(alarm));
    alarm[0] = (cmsUInt16Number) (limit * 65535.0);
    alarm[1] = (cmsUInt16Number) ((1.0 - limit) * 65535.0);
    alarm[2] = (cmsUInt16Number) (vint & 0xFFFF);
    alarm[3] = (cmsUInt16Number) 0xA5A5;
    cmsSetAlarmCodesTHR(ctx, alarm);
    inkProfile = cmsCreateInkLimitingDeviceLinkTHR(ctx, (cmsColorSpaceSignature) 0, limit);

    // step 3: Operate
    ncl = cmsAllocNamedColorList(ctx, 3U, 4U, "PFX", "SFX");

    // step 4: Cleanup
    cmsFreeNamedColorList(ncl);
    cmsCloseProfile(inkProfile);
    cmsCloseProfile(srcProfile);

    // API sequence test completed successfully
    return 66;
}