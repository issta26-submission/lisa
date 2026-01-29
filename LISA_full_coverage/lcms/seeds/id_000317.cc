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
//<ID> 317
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsContext ctx = (cmsContext)NULL;
    cmsHPROFILE profile = cmsCreate_sRGBProfileTHR(ctx);
    cmsFloat64Number ver = cmsGetProfileVersion(profile);
    cmsSetProfileVersion(profile, ver);
    cmsColorSpaceSignature cs = cmsGetColorSpace(profile);
    cmsSetColorSpace(profile, cs);
    cmsUInt32Number flags = cmsGetHeaderFlags(profile);
    (void)flags;
    cmsFloat64Number tac = cmsDetectTAC(profile);
    (void)tac;
    cmsCloseProfile(profile);
    return 66;
}