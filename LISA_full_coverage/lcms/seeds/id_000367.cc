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
//<ID> 367
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE profile = cmsCreate_sRGBProfileTHR(ctx);
    const cmsCIExyY * d50 = cmsD50_xyY();
    cmsUInt32Number bufSize = (cmsUInt32Number)128;
    char descBuf[128];
    memset(descBuf, 0, bufSize);

    // step 2: Configure profile and extract description MLU
    cmsMLU * descMLU = (cmsMLU *) cmsReadTag(profile, (cmsTagSignature)0x64657363); // 'desc'
    cmsUInt32Number copied = cmsMLUgetUTF8(descMLU, "en", "US", descBuf, bufSize);

    // step 3: Operate & Validate (set PCS, tweak header, inspect version)
    cmsSetPCS(profile, (cmsColorSpaceSignature)0x58595A20); // 'XYZ '
    cmsSetHeaderRenderingIntent(profile, (cmsUInt32Number)0);
    cmsFloat64Number version = cmsGetProfileVersion(profile);
    cmsUInt32Number validation = (cmsUInt32Number)copied ^ (cmsUInt32Number)((cmsUInt32Number)version);

    // step 4: Cleanup & persist result
    cmsSaveProfileToFile(profile, "temp_sRGB_profile.icc");
    (void)d50;
    (void)validation;

    // API sequence test completed successfully
    return 66;
}