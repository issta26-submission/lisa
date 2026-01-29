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
//<ID> 351
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE profiles[2];
    profiles[0] = cmsCreate_sRGBProfile();
    profiles[1] = cmsCreateXYZProfile();
    cmsFloat64Number prevAdapt = cmsSetAdaptationStateTHR(ctx, 0.5);

    // step 2: Setup
    cmsUInt32Number nProfiles = 2u;
    cmsUInt32Number InputFormat = 0u;
    cmsUInt32Number OutputFormat = 0u;
    cmsUInt32Number Intent = 0u;
    cmsUInt32Number dwFlags = 0u;
    cmsHTRANSFORM xform = cmsCreateMultiprofileTransformTHR(ctx, profiles, nProfiles, InputFormat, OutputFormat, Intent, dwFlags);

    // step 3: Operate & Validate
    cmsColorSpaceSignature pcs = cmsGetPCS(profiles[0]);
    cmsUInt32Number obtainedInputFormat = cmsGetTransformInputFormat(xform);
    (void)pcs;
    (void)obtainedInputFormat;

    // step 4: Cleanup
    cmsDeleteTransform(xform);
    cmsCloseProfile(profiles[1]);
    cmsCloseProfile(profiles[0]);
    cmsSetAdaptationStateTHR(ctx, prevAdapt);
    cmsDeleteContext(ctx);

    // API sequence test completed successfully
    return 66;
}