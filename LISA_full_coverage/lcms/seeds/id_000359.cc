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
//<ID> 359
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE profile_sRGB = cmsCreate_sRGBProfile();
    cmsHPROFILE profile_XYZ = cmsCreateXYZProfile();
    cmsHPROFILE profiles[2];
    profiles[0] = profile_sRGB;
    profiles[1] = profile_XYZ;
    cmsHTRANSFORM transform = NULL;
    cmsColorSpaceSignature pcs = (cmsColorSpaceSignature)0;
    cmsFloat64Number prevAdapt = (cmsFloat64Number)0.0;
    cmsUInt32Number inputFormat = (cmsUInt32Number)0;
    cmsUInt32Number validation = (cmsUInt32Number)0;

    // step 2: Setup
    pcs = cmsGetPCS(profile_sRGB);
    transform = cmsCreateMultiprofileTransformTHR(ctx, profiles, (cmsUInt32Number)2, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);

    // step 3: Operate & Validate
    prevAdapt = cmsSetAdaptationStateTHR(ctx, (cmsFloat64Number)1.0);
    inputFormat = cmsGetTransformInputFormat(transform);
    validation = (cmsUInt32Number)((cmsUInt32Number)pcs ^ inputFormat ^ (cmsUInt32Number)prevAdapt);
    (void)validation;

    // step 4: Cleanup
    cmsSetAdaptationStateTHR(ctx, prevAdapt);
    cmsCloseProfile(profile_sRGB);
    cmsCloseProfile(profile_XYZ);

    // API sequence test completed successfully
    return 66;
}