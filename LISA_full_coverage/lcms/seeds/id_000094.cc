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
//<ID> 94
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Step 1: Declarations (use NULL context as allowed by THR variants)
    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsHPROFILE hProfile2 = NULL;
    cmsHTRANSFORM xform = NULL;
    cmsUInt32Number outFormat = 0;
    cmsUInt32Number model = 0;

    // Step 2: Create and modify profiles
    hProfile = cmsCreateXYZProfileTHR(ctx);
    cmsSetEncodedICCversion(hProfile, (cmsUInt32Number)0x04200000); // encoded ICC v4.2.0.0
    cmsSetHeaderRenderingIntent(hProfile, (cmsUInt32Number)0); // perceptual (0) commonly used
    model = cmsGetColorSpace(hProfile);
    cmsSetHeaderModel(hProfile, model);

    hProfile2 = cmsCreateXYZProfile();

    // Step 3: Create a transform between the two profiles and inspect format
    xform = cmsCreateTransform(hProfile, (cmsUInt32Number)0, hProfile2, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);
    outFormat = cmsGetTransformOutputFormat(xform);

    // Step 4: Cleanup
    cmsDeleteTransform(xform);
    cmsCloseProfile(hProfile2);
    cmsCloseProfile(hProfile);

    (void)ctx;
    (void)outFormat;
    (void)model;

    return 66;
}