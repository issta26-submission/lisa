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
//<ID> 368
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Step 1: Declarations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE profile_sRGB = cmsCreate_sRGBProfileTHR(ctx);
    const cmsCIExyY * d50 = cmsD50_xyY();
    cmsHPROFILE profile_Lab = cmsCreateLab4Profile(d50);
    const cmsMLU * descMLU = (const cmsMLU *) cmsReadTag(profile_sRGB, (cmsTagSignature)0x64657363UL); // 'desc'
    char descBuffer[256];
    memset(descBuffer, 0, sizeof(descBuffer));

    // Step 2: Configure
    cmsSetPCS(profile_sRGB, (cmsColorSpaceSignature)0x58595A20UL); // 'XYZ '
    cmsSetPCS(profile_Lab, (cmsColorSpaceSignature)0x4C616220UL);  // 'Lab '

    // Step 3: Operate & Validate
    cmsUInt32Number got = cmsMLUgetUTF8(descMLU, "en", "US", descBuffer, (cmsUInt32Number)sizeof(descBuffer));
    cmsFloat64Number version = cmsGetProfileVersion(profile_sRGB);
    cmsUInt32Number ver32 = (cmsUInt32Number)(version * 100.0);
    cmsUInt32Number validation = got ^ ver32;
    (void)validation;
    (void)descBuffer;

    // Step 4: Cleanup
    cmsUnregisterPlugins();

    // API sequence test completed successfully
    return 66;
}