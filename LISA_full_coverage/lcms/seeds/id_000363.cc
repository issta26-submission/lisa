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
//<ID> 363
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE profile = cmsCreate_sRGBProfileTHR(ctx);
    cmsUInt32Number pcs_val = (cmsUInt32Number)0;
    cmsUInt32Number descTag = (cmsUInt32Number) 'desc';
    cmsMLU * mlu = NULL;
    char utf8buf[128];
    memset(utf8buf, 0, sizeof(utf8buf));
    const cmsCIExyY * d50 = cmsD50_xyY();
    cmsUInt32Number got = 0;

    // step 2: Setup
    cmsSetPCS(profile, (cmsColorSpaceSignature) pcs_val);
    mlu = (cmsMLU *) cmsReadTag(profile, (cmsTagSignature) descTag);

    // step 3: Core operations
    got = cmsMLUgetUTF8(mlu, "en", "US", utf8buf, (cmsUInt32Number) sizeof(utf8buf));
    (void) got;
    cmsSetHeaderModel(profile, (cmsUInt32Number) (d50->x * 1000.0));
    cmsSaveProfileToFile(profile, "temp_profile.icc");
    cmsContext profCtx = cmsGetProfileContextID(profile);
    (void) profCtx;

    // step 4: Cleanup
    cmsCloseProfile(profile);

    // API sequence test completed successfully
    return 66;
}