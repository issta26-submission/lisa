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
//<ID> 65
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Initialize
    cmsHPROFILE hProfile = cmsCreate_sRGBProfileTHR((cmsContext) NULL);

    // step 2: Configure / Inspect
    const cmsCIExyY * d50_xyY = cmsD50_xyY();
    cmsFloat64Number profileVersion = cmsGetProfileVersion(hProfile);
    cmsColorSpaceSignature pcs = cmsGetPCS(hProfile);

    cmsCIEXYZ d50_xyz;
    cmsxyY2XYZ(&d50_xyz, d50_xyY);

    // step 3: Operate / Validate
    cmsFloat64Number tempK = 0;
    cmsBool gotTemp = cmsTempFromWhitePoint(&tempK, d50_xyY);

    // step 4: Cleanup (use available cleanup API)
    cmsUnregisterPlugins();

    (void) hProfile;
    (void) d50_xyY;
    (void) profileVersion;
    (void) pcs;
    (void) d50_xyz;
    (void) tempK;
    (void) gotTemp;

    return 66;
}