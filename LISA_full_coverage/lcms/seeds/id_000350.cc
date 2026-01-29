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
//<ID> 350
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Step 1: Declarations
    cmsHPROFILE profiles[2];
    cmsColorSpaceSignature pcs0;
    cmsFloat64Number prevAdapt;
    cmsHTRANSFORM xform;
    cmsUInt32Number inputFormat;
    cmsFloat64Number validationValue;

    // Step 2: Setup
    profiles[0] = cmsCreate_sRGBProfile();
    profiles[1] = cmsCreateXYZProfile();
    pcs0 = cmsGetPCS(profiles[0]);
    prevAdapt = cmsSetAdaptationStateTHR((cmsContext)NULL, (cmsFloat64Number)1.0);

    // Step 3: Core operations
    xform = cmsCreateMultiprofileTransformTHR((cmsContext)NULL, profiles, (cmsUInt32Number)2, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);
    inputFormat = cmsGetTransformInputFormat(xform);
    validationValue = prevAdapt + (cmsFloat64Number)inputFormat + (cmsFloat64Number)pcs0;
    (void)validationValue;

    // Step 4: Cleanup
    cmsCloseProfile(profiles[0]);
    cmsCloseProfile(profiles[1]);
    cmsDeleteTransform(xform);
    return 66;
    // API sequence test completed successfully
}