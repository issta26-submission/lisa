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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsUInt16Number alarms[16];
    cmsHPROFILE srgb = (cmsHPROFILE) NULL;
    cmsHPROFILE inkProfile = (cmsHPROFILE) NULL;
    cmsNAMEDCOLORLIST * ncl = (cmsNAMEDCOLORLIST *) NULL;
    cmsFloat64Number srgbVersion = 0.0;
    cmsFloat64Number limit = 0.0;
    cmsBool closedA = 0;
    cmsBool closedB = 0;
    cmsUInt32Number ncolors = 0;

    alarms[0]  = 100; alarms[1]  = 101; alarms[2]  = 102; alarms[3]  = 103;
    alarms[4]  = 104; alarms[5]  = 105; alarms[6]  = 106; alarms[7]  = 107;
    alarms[8]  = 108; alarms[9]  = 109; alarms[10] = 110; alarms[11] = 111;
    alarms[12] = 112; alarms[13] = 113; alarms[14] = 114; alarms[15] = 115;

    // step 2: Setup / Configure
    cmsSetAlarmCodesTHR(ctx, alarms);
    srgb = cmsCreate_sRGBProfileTHR(ctx);
    srgbVersion = cmsGetProfileVersion(srgb);
    limit = (srgbVersion - (long) srgbVersion) * 0.5 + 0.05;
    inkProfile = cmsCreateInkLimitingDeviceLinkTHR(ctx, _cmsICCcolorSpace(0), limit);
    ncl = cmsAllocNamedColorList(ctx, 3U, 4U, "PFX", "SFX");

    // step 3: Operate / Validate
    ncolors = cmsNamedColorCount(ncl);
    (void) ncolors;
    (void) srgbVersion;
    (void) limit;

    // step 4: Cleanup
    closedA = cmsCloseProfile(srgb);
    closedB = cmsCloseProfile(inkProfile);
    cmsFreeNamedColorList(ncl);
    (void) closedA;
    (void) closedB;

    // API sequence test completed successfully
    return 66;
}