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
//<ID> 39
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
    cmsFloat64Number version = 0.0;
    cmsFloat64Number limit = 0.0;
    cmsUInt16Number alarmCodes[16] = {
        1000, 1001, 1002, 1003,
        1004, 1005, 1006, 1007,
        1008, 1009, 1010, 1011,
        1012, 1013, 1014, 1015
    };
    char prefix[16];
    char suffix[16];
    cmsNAMEDCOLORLIST * namedList = (cmsNAMEDCOLORLIST *) NULL;

    // step 2: Setup (create profile and prepare strings and alarm codes)
    srcProfile = cmsCreate_sRGBProfileTHR(ctx);
    version = cmsGetProfileVersion(srcProfile);
    limit = version * 0.01 + 0.05;
    memset(prefix, 0, sizeof(prefix));
    memset(suffix, 0, sizeof(suffix));
    prefix[0] = 'P'; prefix[1] = 'r'; prefix[2] = 'e'; prefix[3] = 'f';
    suffix[0] = 'S'; suffix[1] = 'u'; suffix[2] = 'f'; suffix[3] = 'f';

    // step 3: Operate (configure context, create ink-limiting profile and allocate named color list)
    cmsSetAlarmCodesTHR(ctx, alarmCodes);
    inkLimitProfile = cmsCreateInkLimitingDeviceLinkTHR(ctx, (cmsColorSpaceSignature) 0, limit);
    namedList = cmsAllocNamedColorList(ctx, 2U, 3U, prefix, suffix);

    // step 4: Cleanup (free named color list and close profiles)
    cmsFreeNamedColorList(namedList);
    cmsCloseProfile(inkLimitProfile);
    cmsCloseProfile(srcProfile);

    // API sequence test completed successfully
    return 66;
}